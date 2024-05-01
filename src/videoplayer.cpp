#include "videoplayer.h"
#include "./ui_mainwindow.h"
#include "QDebug"


VideoPlayer::VideoPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Player = new QMediaPlayer();
    ui->setupUi(this);
    audioOutput = new QAudioOutput;
    Player->setAudioOutput(audioOutput);
    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButton_Seek_Backward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pushButton_Seek_Forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(100);
    ui->horizontalSlider_Volume->setValue(30);
    ui->horizontalSlider_Duration->setValue(0);
    audioOutput->setVolume(ui->horizontalSlider_Volume->value());
    connect(Player, &QMediaPlayer::durationChanged,this,&VideoPlayer::durationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &VideoPlayer::positionChanged);
    ui->horizontalSlider_Duration->setRange(0,Player->duration()/1000);
    ui->pushButtonAdd->setVisible(false);




    lineEditEnglish_new = new MultiSelectLabel(ui->groupBox_Video);
    lineEditEnglish_new->setObjectName("lineEditEnglish_new");
    lineEditEnglish_new->setGeometry(QRect(40, 40, 731, 61));
    lineEditRussian_new = new MultiSelectLabel(ui->groupBox_Video);
    lineEditRussian_new->setObjectName("lineEditRussian_new");
    lineEditRussian_new->setGeometry(QRect(40, 130, 731, 61));


    lineEditEnglish_new->setVisible(false);
    lineEditRussian_new->setVisible(false);
    ui->pushButtonSub->setAttribute(Qt::WA_Hover);
    auto shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0,0);
    shadow->setColor(Qt::black);
    ui->pushButtonSub->setGraphicsEffect(shadow);
    view = new QGraphicsView(ui->groupBox_Video);
    view->setGeometry(0, 0, ui->groupBox_Video->width(), ui->groupBox_Video->height());
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    view->setAlignment(Qt::AlignCenter);

    scene = new QGraphicsScene(view);
    item = new QGraphicsVideoItem();

    item->setAspectRatioMode(Qt::KeepAspectRatio);
    item->setSize(QSizeF(view->size().width(), view->size().height()));

    view->setScene(scene);
    scene->addItem(item);
}

void VideoPlayer::set_vocab(Main_vocabulary* vocab)
{
    main_vocab = vocab;
}

VideoPlayer::~VideoPlayer()
{
    delete ui;
}

bool VideoPlayer::event(QEvent * e)
{
    if(e->type()==QEvent::HoverMove)
    {
        QWidget* widget = QApplication::widgetAt(QCursor::pos());
        if(ui->pushButtonSub==widget){
            ui->pushButtonSub->setText(russianText);
        }else{
            ui->pushButtonSub->setText(englishText);
        }
    }
    return QWidget::event(e);
}

void VideoPlayer::durationChanged(qint64 duration)
{
    mDuration = duration/1000;
    ui->horizontalSlider_Duration->setMaximum(mDuration);
}

void VideoPlayer::positionChanged(qint64 duration)
{
    if (!ui->horizontalSlider_Duration->isSliderDown()){
        ui->horizontalSlider_Duration->setValue(duration/1000);
    }
    updateDuration(duration/1000);
    if (englishsubs!=nullptr)
    if (englishsubs->readed)
    englishText=englishsubs->getSubtitleText(static_cast<double>(duration));
    if (russiansubs!=nullptr)
    if (russiansubs->readed)
    russianText = russiansubs->getSubtitleText(static_cast<double>(duration));
    ui->pushButtonSub->setText(englishText);
}


void VideoPlayer::on_actionOpen_triggered()
{
    QString FileName=QFileDialog::getOpenFileName(this,tr("Select Video File"), tr("MP4 Files (*.mp4)"));
    Player->setVideoOutput(item);
    Player->setSource(QUrl(FileName));
    view->show();
    ui->pushButtonAdd->raise();
    lineEditEnglish_new->raise();
    lineEditRussian_new->raise();
    ui->pushButtonSub->raise();
}


void VideoPlayer::on_horizontalSlider_Duration_valueChanged(int value)
{
    Player->setPosition(value*1000);
}





void VideoPlayer::on_pushButton_Play_Pause_clicked()
{
    if (IS_Pause == true){
        IS_Pause = false;
        Player->play();
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else{
        IS_Pause = true;
        Player->pause();
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    ui->pushButtonAdd->setVisible(false);
    lineEditEnglish_new->setVisible(false);
    lineEditRussian_new->setVisible(false);
}


void VideoPlayer::on_pushButton_Stop_clicked()
{
    Player->stop();
}


void VideoPlayer::on_pushButton_Volume_clicked()
{
    if (IS_Muted == false){
        IS_Muted = true;
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        audioOutput->setMuted(true);
    }
    else{
        IS_Muted = false;
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        audioOutput->setMuted(false);
    }
}


void VideoPlayer::on_horizontalSlider_Volume_valueChanged(int value)
{
    audioOutput->setVolume(value);
}


void VideoPlayer::on_pushButton_Seek_Forward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value()+20);
    Player->setPosition(ui->horizontalSlider_Duration->value()*1000);
}


void VideoPlayer::on_pushButton_Seek_Backward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value()-20);
    Player->setPosition(ui->horizontalSlider_Duration->value()*1000);
}

void VideoPlayer::updateDuration(qint64 duration)
{
    if (duration || mDuration){
        QTime currentTime((duration/3600)%60,(duration/60)%60,duration%60,(duration*1000)%1000);
        QTime totalTime((mDuration/3600)%60,(mDuration/60)%60,mDuration%60,(mDuration*1000)%1000);
        QString format = "";
        if (mDuration>3600) format = "hh:mm:ss";
        else format = "mm:ss";
        ui->label_current_Time->setText(currentTime.toString(format));
        ui->label_Total_Time->setText(totalTime.toString(format));
    }
}

void VideoPlayer::on_actionenglish_triggered()
{
    QString FileName=QFileDialog::getOpenFileName(this,tr("Select English subs"), tr("SRT Files (*.srt)"));
    englishsubs = new BackEnd(FileName);
}


void VideoPlayer::on_actionrussian_triggered()
{
    QString FileName=QFileDialog::getOpenFileName(this,tr("Select Russian subs"), tr("SRT Files (*.srt)"));
    russiansubs = new BackEnd(FileName);
}


void VideoPlayer::on_pushButtonSub_clicked()
{
    ui->pushButtonAdd->setVisible(true);
    lineEditEnglish_new->setVisible(true);
    lineEditRussian_new->setVisible(true);
    lineEditEnglish_new->setText(englishText);
    lineEditRussian_new->setText(russianText);
}


void VideoPlayer::on_pushButtonAdd_clicked()
{
    ui->pushButtonAdd->setVisible(false);
    lineEditEnglish_new->setVisible(false);
    lineEditRussian_new->setVisible(false);
    QString EnglishWord = lineEditEnglish_new->selectedText();
    QString RussianWord = lineEditRussian_new->selectedText();
    main_vocab->add_word(RussianWord, EnglishWord);
}

