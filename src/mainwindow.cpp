#include "mainwindow.h"
#include <QFileDialog>
#include <QStyle>
#include <QApplication>
#include "subtitleselectiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isPaused(true), isMuted(false)
{
    ui = new VideoPlayerUI(this);
    setCentralWidget(ui);

    playerController = new VideoPlayerController(this);
    subtitleManager = new SubtitleManager(this);

    setupConnections();

    playerController->setVideoOutput(ui->videoItem);
    setWindowTitle("Watch");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete playerController;
    delete subtitleManager;
}

void MainWindow::setVocabulary(Main_vocabulary* vocab)
{
    mainVocab = vocab;
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
    return QMainWindow::event(event);
}

void MainWindow::setupConnections()
{
    connect(ui->openAction, &QAction::triggered, this, &MainWindow::onOpenFileTriggered);
    connect(ui->playPauseButton, &QPushButton::clicked, this, &MainWindow::onPlayPauseClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(ui->volumeButton, &QPushButton::clicked, this, &MainWindow::onVolumeClicked);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);
    connect(ui->seekForwardButton, &QPushButton::clicked, this, &MainWindow::onSeekForwardClicked);
    connect(ui->seekBackwardButton, &QPushButton::clicked, this, &MainWindow::onSeekBackwardClicked);
    connect(ui->subtitleButton, &QPushButton::clicked, this, &MainWindow::onSubtitleClicked);
    connect(ui->addWordButton, &QPushButton::clicked, this, &MainWindow::onAddWordClicked);
    connect(ui->durationSlider, &QSlider::valueChanged, this, &MainWindow::onDurationSliderValueChanged);
    connect(ui->manualSubsButton, &QPushButton::clicked, this, &MainWindow::showSubtitleSelectionDialog);

    connect(playerController, &VideoPlayerController::durationChanged, this, &MainWindow::onDurationChanged);
    connect(playerController, &VideoPlayerController::positionChanged, this, &MainWindow::onPositionChanged);

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
    QString englishWord = ui->englishSubtitleEdit->selectedText();
    QString russianWord = ui->russianSubtitleEdit->selectedText();
    if (mainVocab) {
        mainVocab->add_word(russianWord, englishWord);
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
        ui->durationSlider->setValue(position / 1000);
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
