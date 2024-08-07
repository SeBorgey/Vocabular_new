#include "videoplayerui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QMainWindow>
#include <QDebug>

VideoPlayerUI::VideoPlayerUI(QWidget *parent)
    : QWidget(parent), isFullScreen(false)
{
    createWidgets();
    setupLayout();
    if (auto mainWindow = qobject_cast<QMainWindow*>(parent)) {
        createMenuAndToolbar(mainWindow);
    }
    updateVideoSize();
    updateFontSizes();
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

void VideoPlayerUI::createWidgets()
{
    openAction = new QAction(tr("Open"), this);
    playPauseButton = new QPushButton(this);
    stopButton = new QPushButton(this);
    volumeButton = new QPushButton(this);
    volumeSlider = new QSlider(Qt::Horizontal, this);
    durationSlider = new QSlider(Qt::Horizontal, this);
    currentTimeLabel = new QLabel("00:00:00", this);
    totalTimeLabel = new QLabel("00:00:00", this);
    groupBox_Video = new QGroupBox(this);
    videoView = new QGraphicsView(groupBox_Video);
    videoItem = new QGraphicsVideoItem();
    fullscreenButton = new QPushButton(this);
    fullscreenButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    groupBox_Video->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    videoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    videoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    videoView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    videoView->setAlignment(Qt::AlignLeft);
    videoItem->setAspectRatioMode(Qt::KeepAspectRatio);
    videoItem->setSize(groupBox_Video->size());
    videoItem->setPos(0, 0);

    fileNameEdit = new QLineEdit(this);
    englishSubsEdit = new QLineEdit(this);
    russianSubsEdit = new QLineEdit(this);
    audioEdit = new QLineEdit(this);
    seekForwardButton = new QPushButton(this);
    seekBackwardButton = new QPushButton(this);

    playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    volumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    seekForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    seekBackwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));

    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(30);

    fileNameEdit->setPlaceholderText(tr("File name"));
    englishSubsEdit->setPlaceholderText(tr("English subs"));
    russianSubsEdit->setPlaceholderText(tr("Russian subs"));
    audioEdit->setPlaceholderText("Audiotrack");

    englishSubtitleEdit = new MultiSelectTextEdit(groupBox_Video);
    russianSubtitleEdit = new MultiSelectTextEdit(groupBox_Video);

    subtitleButton = new CustomLabel(groupBox_Video);
    QFont font;
    font.setBold(true);
    subtitleButton->setFont(font);
    subtitleButton->setStyleSheet(QString::fromUtf8("background-color: transparent; border: none; color: white; font-weight: bold; "));

    subtitleButton->setAttribute(Qt::WA_Hover);
    auto shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0,0);
    shadow->setColor(Qt::black);
    subtitleButton->setGraphicsEffect(shadow);

    addWordButton = new QPushButton(tr("Add"), groupBox_Video);
    addWordButton->setVisible(false);
    addWordButton->setFixedSize(200, 60);

    QGraphicsScene *scene = new QGraphicsScene(videoView);
    videoView->setScene(scene);
    scene->addItem(videoItem);

    englishSubtitleEdit->setVisible(false);
    russianSubtitleEdit->setVisible(false);
    addWordButton->setVisible(false);

    englishSubtitleEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    russianSubtitleEdit->setContextMenuPolicy(Qt::CustomContextMenu);
}

void VideoPlayerUI::setupLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    videoLayout = new QHBoxLayout();

    videoLayout->addWidget(groupBox_Video);
    mainLayout->addLayout(videoLayout);

    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLayout->addWidget(currentTimeLabel);
    timeLayout->addWidget(durationSlider);
    timeLayout->addWidget(totalTimeLabel);
    mainLayout->addLayout(timeLayout);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->addWidget(seekBackwardButton);
    controlLayout->addWidget(playPauseButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(seekForwardButton);
    controlLayout->addWidget(fullscreenButton);
    controlLayout->addStretch();
    controlLayout->addWidget(volumeButton);
    controlLayout->addWidget(volumeSlider);
    mainLayout->addLayout(controlLayout);

    setLayout(mainLayout);

    QVBoxLayout *innerLayout = new QVBoxLayout(groupBox_Video);

    innerLayout->addStretch(1);

    QHBoxLayout *englishSubLayout = new QHBoxLayout();
    englishSubLayout->addStretch(1);
    englishSubLayout->addWidget(englishSubtitleEdit);
    englishSubLayout->addStretch(1);
    innerLayout->addLayout(englishSubLayout);

    innerLayout->addSpacing(10);
    QHBoxLayout *russianSubLayout = new QHBoxLayout();
    russianSubLayout->addStretch(1);
    russianSubLayout->addWidget(russianSubtitleEdit);
    russianSubLayout->addStretch(1);
    innerLayout->addLayout(russianSubLayout);

    QHBoxLayout *addButtonLayout = new QHBoxLayout();
    addButtonLayout->addStretch(1);
    addButtonLayout->addWidget(addWordButton);
    addButtonLayout->addStretch(1);
    innerLayout->addLayout(addButtonLayout);

    innerLayout->addStretch(2);

    QHBoxLayout *subtitleButtonLayout = new QHBoxLayout();
    subtitleButtonLayout->addStretch(1);
    subtitleButtonLayout->addWidget(subtitleButton);
    subtitleButtonLayout->addStretch(1);
    innerLayout->addLayout(subtitleButtonLayout);

    groupBox_Video->setLayout(innerLayout);
}

void VideoPlayerUI::createMenuAndToolbar(QMainWindow *mainWindow)
{
    QMenuBar *menuBar = new QMenuBar(mainWindow);
    QMenu *fileMenu = menuBar->addMenu(tr("Select video File"));
    fileMenu->addAction(openAction);

    QAction *englishAction = new QAction(tr("English"), this);
    QAction *russianAction = new QAction(tr("Russian"), this);
    fileMenu->addAction(englishAction);
    fileMenu->addAction(russianAction);

    mainWindow->setMenuBar(menuBar);

    QToolBar *toolBar = new QToolBar(mainWindow);
    toolBar->addAction(openAction);
    toolBar->addWidget(fileNameEdit);
    toolBar->addWidget(audioEdit);
    toolBar->addWidget(englishSubsEdit);
    toolBar->addWidget(russianSubsEdit);

    manualSubsButton = new QPushButton(tr("Manual"), toolBar);
    toolBar->addWidget(manualSubsButton);

    mainWindow->addToolBar(toolBar);
}

void VideoPlayerUI::updateVideoSize()
{
    videoView->setGeometry(0, 0, groupBox_Video->width(), groupBox_Video->height());

    videoView->scene()->setSceneRect(0, 0, videoView->width(), videoView->height());

    videoItem->setSize(groupBox_Video->size());

    videoView->update();
}
//todo: это костыль, который немного баг с возвращением из полноэкранного режима
//но чтобы его полностью исправить, надо подумать
void VideoPlayerUI::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateVideoSize();
    updateFontSizes();

    subtitleButton->forceEnterEvent();
    subtitleButton->setFocus();
    subtitleButton->update();
}

void VideoPlayerUI::updateFontSizes()
{
    int windowWidth = groupBox_Video->width();
    int windowHeight = groupBox_Video->height();
    int baseFontSize = qMin(windowWidth, windowHeight) / 20;

    QFont subtitleFont = englishSubtitleEdit->font();
    subtitleFont.setPointSize(baseFontSize);
    englishSubtitleEdit->setFont(subtitleFont);
    russianSubtitleEdit->setFont(subtitleFont);

    QFont buttonFont = subtitleButton->font();
    buttonFont.setPointSize(baseFontSize);
    subtitleButton->setFont(buttonFont);

    int subtitleHeight = windowHeight / 5;
    englishSubtitleEdit->setFixedHeight(subtitleHeight);
    russianSubtitleEdit->setFixedHeight(subtitleHeight);
    subtitleButton->setFixedHeight(subtitleHeight);

    int subtitleWidth = windowWidth*0.9;
    englishSubtitleEdit->setFixedWidth(subtitleWidth);
    russianSubtitleEdit->setFixedWidth(subtitleWidth);
    subtitleButton->setFixedWidth(subtitleWidth);

}
bool VideoPlayerUI::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == groupBox_Video && isFullScreen) {
        if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
            QWidget* mainWindow = window();
            QCoreApplication::sendEvent(mainWindow, event);
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void VideoPlayerUI::enterFullScreen()
{
    if (!isFullScreen) {
        previousGeometry = groupBox_Video->geometry();
        originalFlags = groupBox_Video->windowFlags();

        groupBox_Video->setParent(nullptr);
        groupBox_Video->setWindowFlags(originalFlags | Qt::Window);

        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

        groupBox_Video->setGeometry(screenGeometry);
        groupBox_Video->showFullScreen();
        groupBox_Video->installEventFilter(this);
        isFullScreen = true;
        groupBox_Video->setFocusPolicy(Qt::StrongFocus);
        groupBox_Video->setFocus();
        updateVideoSize();
        updateFontSizes();
    }
}

void VideoPlayerUI::exitFullScreen()
{
    if (isFullScreen) {
        resize(geometry().width()-1, geometry().height());
        groupBox_Video->removeEventFilter(this);
        groupBox_Video->setWindowFlags(originalFlags);
        groupBox_Video->setParent(this);

        QPoint globalPos = mapToGlobal(previousGeometry.topLeft());
        groupBox_Video->move(mapFromGlobal(globalPos));
        groupBox_Video->resize(previousGeometry.size());
        videoLayout->addWidget(groupBox_Video);
        groupBox_Video->show();
        isFullScreen = false;
        setFocusPolicy(Qt::StrongFocus);
        setFocus();
           updateVideoSize();
        updateFontSizes();        
    }
}
