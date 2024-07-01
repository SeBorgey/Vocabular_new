#include "videoplayerui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QMainWindow>

VideoPlayerUI::VideoPlayerUI(QWidget *parent)
    : QWidget(parent)
{
    createWidgets();
    setupLayout();
    if (auto mainWindow = qobject_cast<QMainWindow*>(parent)) {
        createMenuAndToolbar(mainWindow);
    }
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
    videoView = new QGraphicsView(this);
    videoItem = new QGraphicsVideoItem();


    fileNameEdit = new QLineEdit(this);
    englishSubsEdit = new QLineEdit(this);
    russianSubsEdit = new QLineEdit(this);
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

    englishSubtitleEdit = new MultiSelectLabel(this);
    russianSubtitleEdit = new MultiSelectLabel(this);
    englishSubtitleEdit->setGeometry(QRect(40, 40, 731, 61));
    russianSubtitleEdit->setGeometry(QRect(40, 130, 731, 61));
    englishSubtitleEdit->setVisible(false);
    russianSubtitleEdit->setVisible(false);


    subtitleButton = new QPushButton(this);
    subtitleButton->setText(tr("Subtitle"));
    subtitleButton->setGeometry(QRect(50, 320, 701, 71));
    QFont font;
    font.setBold(true);
    subtitleButton->setFont(font);
    subtitleButton->setStyleSheet(QString::fromUtf8("background-color: transparent; border: none; color: white; font-size: 20px; font-weight: bold; "));

    subtitleButton->setAttribute(Qt::WA_Hover);
    auto shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0,0);
    shadow->setColor(Qt::black);
    subtitleButton->setGraphicsEffect(shadow);

    addWordButton = new QPushButton(tr("Add"), this);
    addWordButton->setVisible(false);
    addWordButton->setGeometry(QRect(300, 210, 201, 61));
}

void VideoPlayerUI::setupLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *videoLayout = new QHBoxLayout();
    videoLayout->addWidget(videoView);
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
    controlLayout->addStretch();
    controlLayout->addWidget(volumeButton);
    controlLayout->addWidget(volumeSlider);
    mainLayout->addLayout(controlLayout);

    QHBoxLayout *subtitleLayout = new QHBoxLayout();
    subtitleLayout->addWidget(subtitleButton);
    mainLayout->addLayout(subtitleLayout);

    QHBoxLayout *wordAddLayout = new QHBoxLayout();
    wordAddLayout->addWidget(englishSubtitleEdit);
    wordAddLayout->addWidget(russianSubtitleEdit);
    wordAddLayout->addWidget(addWordButton);
    mainLayout->addLayout(wordAddLayout);

    setLayout(mainLayout);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(videoItem);
    videoView->setScene(scene);
    videoView->setAlignment(Qt::AlignCenter);
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
    toolBar->addWidget(englishSubsEdit);
    toolBar->addWidget(russianSubsEdit);

    manualSubsButton = new QPushButton(tr("Manual"), toolBar);
    toolBar->addWidget(manualSubsButton);

    mainWindow->addToolBar(toolBar);
}
