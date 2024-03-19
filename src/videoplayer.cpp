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
    ui->lineEditEnglish->setVisible(false);
    ui->lineEditRussian->setVisible(false);
    ui->pushButtonSub->setAttribute(Qt::WA_Hover);
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
    if (englishsubs->readed)
    englishText=englishsubs->getSubtitleText(static_cast<double>(duration)/1000);
    if (russiansubs->readed)
    russianText = russiansubs->getSubtitleText(static_cast<double>(duration)/1000);
    ui->pushButtonSub->setText(englishText);
}


void VideoPlayer::on_actionOpen_triggered()
{
    QString FileName=QFileDialog::getOpenFileName(this,tr("Select Video File"), tr("MP4 Files (*.mp4)"));

    Video = new QVideoWidget();
    Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height()-10);
    Video->setParent(ui->groupBox_Video);
    Player->setVideoOutput(Video);
    Player->setSource(QUrl(FileName));
    Video->setVisible(true);
    Video->show();
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
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value()-20);
    Player->setPosition(ui->horizontalSlider_Duration->value()*1000);
}


void VideoPlayer::on_pushButton_Seek_Backward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value()+20);
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
    ui->lineEditEnglish->setVisible(true);
    ui->lineEditRussian->setVisible(true);
    ui->lineEditEnglish->setText(englishText);
    ui->lineEditRussian->setText(russianText);
}


void VideoPlayer::on_pushButtonAdd_clicked()
{
    ui->pushButtonAdd->setVisible(false);
    ui->lineEditEnglish->setVisible(false);
    ui->lineEditRussian->setVisible(false);
    QString EnglishWord = ui->lineEditEnglish->selectedText();
    QString RussianWord = ui->lineEditRussian->selectedText();
    //todo: здесь прописать добавление в словарь
}

