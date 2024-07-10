#include "mainwindow.h"
#include <QFileDialog>
#include <QStyle>
#include <QApplication>
#include "subtitleselectiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isPaused(true), isMuted(false), hoverFilter(new HoverEventFilter(this))
{
    ui = new VideoPlayerUI(this);
    setCentralWidget(ui);

    playerController = new VideoPlayerController(this);
    subtitleManager = new SubtitleManager(this);

    setupConnections();

    playerController->setVideoOutput(ui->videoItem);
    setWindowTitle("Watch");
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    translator = new Translator(this);
        translationMenu = new QMenu(this);

        connect(translator, &Translator::translationReady, this, &MainWindow::onTranslationReady);
        connect(translator, &Translator::errorOccurred, this, &MainWindow::onTranslationError);
}

void MainWindow::showTranslationMenu(const QPoint &pos, bool isEnglish)
{
    QString selectedText = isEnglish ? ui->englishSubtitleEdit->textCursor().selectedText()
                                     : ui->russianSubtitleEdit->textCursor().selectedText();
    if (!selectedText.isEmpty()) {
        translator->translate(selectedText, isEnglish);

        translationMenu->clear();
        QAction *loadingAction = translationMenu->addAction("Loading translations...");
        loadingAction->setEnabled(false);

        translationMenu->popup(pos);
    }
}

void MainWindow::onTranslationReady(const QStringList &translations)
{
    translationMenu->clear();
    for (const QString &translation : translations) {
        QAction *action = translationMenu->addAction(translation);
        connect(action, &QAction::triggered, this, [this, translation]() {
            QString selectedText = ui->englishSubtitleEdit->textCursor().selectedText();
            if (selectedText.isEmpty()) {
                selectedText = ui->russianSubtitleEdit->textCursor().selectedText();
            }
            addWordToDictionary(selectedText, translation);
        });
    }

    if (translations.isEmpty()) {
        QAction *noTranslationAction = translationMenu->addAction("No translations found");
        noTranslationAction->setEnabled(false);
    }
}

void MainWindow::onTranslationError(const QString &error)
{
    translationMenu->clear();
    QAction *errorAction = translationMenu->addAction("Error: " + error);
    errorAction->setEnabled(false);
}

void MainWindow::addWordToDictionary(const QString &word, const QString &translation)
{
    if (mainVocab) {
        mainVocab->addWord(word, translation);
        qDebug() << "Added to dictionary:" << word << "-" << translation;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete playerController;
    delete subtitleManager;
}

void MainWindow::setVocabulary(MainVocabulary* vocab)
{
    mainVocab = vocab;
}

void MainWindow::setupConnections()
{
    connect(ui->subtitleButton, &CustomLabel::clicked, this, &MainWindow::onSubtitleClicked);
    connect(ui->subtitleButton, &CustomLabel::hoverEntered, this, [this]() {
        ui->subtitleButton->setText(russianText);
        cursorSubAt = Hover;
    });
    connect(ui->subtitleButton, &CustomLabel::hoverLeft, this, [this]() {
        ui->subtitleButton->setText(englishText);
        cursorSubAt = NotHover;
    });

    connect(ui, &VideoPlayerUI::fullscreenToggled, this, &MainWindow::toggleFullScreen);
    connect(ui->openAction, &QAction::triggered, this, &MainWindow::onOpenFileTriggered);
    connect(ui->playPauseButton, &QPushButton::clicked, this, &MainWindow::onPlayPauseClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(ui->volumeButton, &QPushButton::clicked, this, &MainWindow::onVolumeClicked);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);
    connect(ui->seekForwardButton, &QPushButton::clicked, this, &MainWindow::onSeekForwardClicked);
    connect(ui->seekBackwardButton, &QPushButton::clicked, this, &MainWindow::onSeekBackwardClicked);
    connect(ui->addWordButton, &QPushButton::clicked, this, &MainWindow::onAddWordClicked);
    connect(ui->durationSlider, &QSlider::valueChanged, this, &MainWindow::onDurationSliderValueChanged);
    connect(ui->manualSubsButton, &QPushButton::clicked, this, &MainWindow::showSubtitleSelectionDialog);
    connect(ui->fullscreenButton, &QPushButton::clicked, this, &MainWindow::toggleFullScreen);
    connect(playerController, &VideoPlayerController::durationChanged, this, &MainWindow::onDurationChanged);
    connect(playerController, &VideoPlayerController::positionChanged, this, &MainWindow::onPositionChanged);

    connect(ui->englishSubtitleEdit, &QTextEdit::customContextMenuRequested,
               this, [this](const QPoint &pos) { showTranslationMenu(pos, true); });
    connect(ui->russianSubtitleEdit, &QTextEdit::customContextMenuRequested,
               this, [this](const QPoint &pos) { showTranslationMenu(pos, false); });
}

void MainWindow::handleHoverEnteredSubtitleButton()
{
    ui->subtitleButton->setText(russianText);
    cursorSubAt = Hover;
}

void MainWindow::handleHoverLeftSubtitleButton()
{
    ui->subtitleButton->setText(englishText);
    cursorSubAt = NotHover;
}

void MainWindow::onOpenFileTriggered()
{
    currentFileName = QFileDialog::getOpenFileName(this, tr("Select Video File"));
    if (!currentFileName.isEmpty()) {
        playerController->setSource(currentFileName);
        connect(playerController, &VideoPlayerController::audioTracksGot, this, &MainWindow::onSetEngAudio);
        ui->fileNameEdit->setText(QFileInfo(currentFileName).fileName());
        ui->englishSubsEdit->clear();
        ui->russianSubsEdit->clear();
        extractor.setFileName(currentFileName);        
    }
}

void MainWindow::onSetEngAudio(){
    QStringList audio = playerController->audioTracks;
    for (int i=0; i<audio.size(); i++){
        if (audio[i].toLower().contains("eng")){
            playerController->setAudioTrack(i);
            ui->audioEdit->setText(audio[i]);
            break;
        }
    }
    disconnect(playerController, &VideoPlayerController::audioTracksGot, this, &MainWindow::onSetEngAudio);
    if (!tryFindSubtitles()) {
        showSubtitleSelectionDialog();
    }
}

bool MainWindow::tryFindSubtitles()
{
    QFileInfo videoFileInfo(currentFileName);
    QString videoFileName = videoFileInfo.completeBaseName();
    QDir parentDir = videoFileInfo.dir();
    parentDir.cdUp();

    QDirIterator it(parentDir.path(), QStringList() << "*.srt", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName().toLower();

        if (fileName.contains(videoFileName.toLower())) {
            QString fullPath = fileInfo.absoluteFilePath();
            if ((fullPath.toLower().contains("rus") || fileName.toLower().contains("rus")) &&
                russianSubsPath.isEmpty()) {
                russianSubsPath = fullPath;
            } else if ((fullPath.toLower().contains("eng") || fileName.toLower().contains("eng")) &&
                       englishSubsPath.isEmpty()) {
                englishSubsPath = fullPath;
            }

            if (!russianSubsPath.isEmpty() && !englishSubsPath.isEmpty()) {
                ui->englishSubsEdit->setText(englishSubsPath);
                ui->russianSubsEdit->setText(russianSubsPath);
                activateSubs();
                return true;
            }
        }
    }

    QList<SubtitleTrack> tracks = extractor.getSubtitleTracks();
    bool russianFound = false;
    bool englishFound = false;

    for (const SubtitleTrack& track : tracks) {
        if (track.language.toLower().contains("rus") && !russianFound) {
            extractor.extractSubtitleTrack(track);
            russianFound = true;
            ui->russianSubsEdit->setText(track.name);
            russianSubsPath = extractor.russianSubtitlePath;
        } else if (track.language.toLower().contains("eng") && !englishFound) {
            extractor.extractSubtitleTrack(track);
            englishFound = true;
            ui->englishSubsEdit->setText(track.name);
            englishSubsPath = extractor.englishSubtitlePath;
        }

        if (russianFound && englishFound) {
            activateSubs();
            return true;
        }
    }

    return false;
}


void MainWindow::showSubtitleSelectionDialog()
{
    SubtitleSelectionDialog dialog(currentFileName, extractor.tracks, playerController->audioTracks, this);
    connect(&dialog, &SubtitleSelectionDialog::subtitlesAndAudioSelected,
            this, &MainWindow::onSubtitlesAndAudioSelected);
    dialog.exec();
}

void MainWindow::onSubtitlesAndAudioSelected(const QString& russianPath, const QString& englishPath,
                                             SubtitleTrack& russianTrack, SubtitleTrack& englishTrack,
                                             int audioTrackIndex)
{
    if (!russianPath.isEmpty()) {
        russianSubsPath = russianPath;
        ui->russianSubsEdit->setText(russianSubsPath);
    } else if (russianTrack.number != -1) {
        russianTrack.language = "rus";
        extractor.extractSubtitleTrack(russianTrack);
        russianSubsPath = extractor.russianSubtitlePath;
        ui->russianSubsEdit->setText(russianTrack.name);
    }

    if (!englishPath.isEmpty()) {
        englishSubsPath = englishPath;
        ui->englishSubsEdit->setText(englishSubsPath);
    } else if (englishTrack.number != -1) {
        englishTrack.language = "eng";
        extractor.extractSubtitleTrack(englishTrack);
        englishSubsPath = extractor.englishSubtitlePath;
        ui->englishSubsEdit->setText(englishTrack.name);
    }

    activateSubs();
    if (audioTrackIndex >= 0) {
        playerController->setAudioTrack(audioTrackIndex);
    }
}
void MainWindow::activateSubs()
{
    subtitleManager->loadSubtitles(englishSubsPath, russianSubsPath);
}

void MainWindow::onPlayPauseClicked()
{
    if (isPaused) {
        playerController->play();
        ui->playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        playerController->pause();
        ui->playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    isPaused = !isPaused;
    ui->addWordButton->setVisible(false);
    ui->englishSubtitleEdit->setVisible(false);
    ui->russianSubtitleEdit->setVisible(false);
}

void MainWindow::onStopClicked()
{
    playerController->stop();
    ui->playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    isPaused = true;
}

void MainWindow::onVolumeClicked()
{
    isMuted = !isMuted;
    playerController->setMuted(isMuted);
    ui->volumeButton->setIcon(style()->standardIcon(isMuted ? QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume));
}

void MainWindow::onVolumeChanged(int value)
{
    playerController->setVolume(value);
}

void MainWindow::onSeekForwardClicked()
{
    playerController->seekForward();
}

void MainWindow::onSeekBackwardClicked()
{
    playerController->seekBackward();
}
void MainWindow::onSubtitleClicked()
{
    ui->addWordButton->setVisible(true);
    ui->englishSubtitleEdit->setVisible(true);
    ui->russianSubtitleEdit->setVisible(true);
    ui->englishSubtitleEdit->setText(englishText);
    ui->russianSubtitleEdit->setText(russianText);
}

void MainWindow::onAddWordClicked()
{
    ui->addWordButton->setVisible(false);
    ui->englishSubtitleEdit->setVisible(false);
    ui->russianSubtitleEdit->setVisible(false);
    QString englishWord = ui->englishSubtitleEdit->getSelectedText();
    QString russianWord = ui->russianSubtitleEdit->getSelectedText();
    if (mainVocab) {
        mainVocab->addWord(russianWord, englishWord);
    }
}

void MainWindow::onDurationChanged(qint64 duration)
{
    ui->durationSlider->setMaximum(duration / 1000);
    updateDurationDisplay(duration);
}

void MainWindow::onPositionChanged(qint64 position)
{
    if (!ui->durationSlider->isSliderDown()) {
        disconnect(ui->durationSlider, &QSlider::valueChanged, this, &MainWindow::onDurationSliderValueChanged);
        ui->durationSlider->setValue(position / 1000);
        connect(ui->durationSlider, &QSlider::valueChanged, this, &MainWindow::onDurationSliderValueChanged);
    }
    updateDurationDisplay(position);
    englishText = subtitleManager->getEnglishSubtitle(playerController->position());
    russianText = subtitleManager->getRussianSubtitle(playerController->position());
    if (cursorSubAt==NotHover)
        ui->subtitleButton->setText(englishText);
    else if (cursorSubAt == Hover)
        ui->subtitleButton->setText(russianText);
}

void MainWindow::onDurationSliderValueChanged(int value)
{
    playerController->setPosition(value * 1000);
}

void MainWindow::updateDurationDisplay(qint64 duration)
{
    QTime currentTime((duration / 3600000) % 60, (duration / 60000) % 60, (duration / 1000) % 60);
    QTime totalTime((playerController->duration() / 3600000) % 60, (playerController->duration() / 60000) % 60, (playerController->duration() / 1000) % 60);

    QString format = playerController->duration() > 3600000 ? "hh:mm:ss" : "mm:ss";
    ui->currentTimeLabel->setText(currentTime.toString(format));
    ui->totalTimeLabel->setText(totalTime.toString(format));
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Space:
        onPlayPauseClicked();
        break;
    case Qt::Key_Right:
        onSeekForwardClicked();
        break;
    case Qt::Key_Left:
        onSeekBackwardClicked();
        break;
    case Qt::Key_Up:
        ui->volumeSlider->setValue(ui->volumeSlider->value() + 5);
        break;
    case Qt::Key_Down:
        ui->volumeSlider->setValue(ui->volumeSlider->value() - 5);
        break;
    case Qt::Key_M:
        onVolumeClicked();
        break;
    case Qt::Key_F:
        toggleFullScreen();
        break;
    case Qt::Key_Escape:
        toggleFullScreen();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }

}


bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::HoverMove)
    {
        QWidget* widget = QApplication::widgetAt(QCursor::pos());
        if (ui->subtitleButton == widget) {
            ui->subtitleButton->setText(russianText);
            cursorSubAt=Hover;
        } else {
            ui->subtitleButton->setText(englishText);
            cursorSubAt=NotHover;
        }
    }
    else if (event->type() == QEvent::MouseButtonPress){
        QWidget* widget = QApplication::widgetAt(QCursor::pos());
        if (ui->subtitleButton == widget){
            onSubtitleClicked();
        }
        else if (ui->addWordButton == widget){
            onAddWordClicked();
        }
    }
    return QMainWindow::event(event);
}

void MainWindow::toggleFullScreen()
{
    if (ui->isFullScreen) {
        ui->exitFullScreen();
        setFocusPolicy(Qt::StrongFocus);
        setFocus();
        this->show();
        ui->subtitleButton->raise();
    } else {
        ui->enterFullScreen();
        ui->groupBox_Video->setFocusPolicy(Qt::StrongFocus);
        ui->groupBox_Video->setFocus();
        this->hide();
    }
}
