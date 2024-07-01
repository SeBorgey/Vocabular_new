#include "videoplayercontroller.h"

VideoPlayerController::VideoPlayerController(QObject *parent)
    : QObject(parent)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    connect(player, &QMediaPlayer::durationChanged, this, &VideoPlayerController::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &VideoPlayerController::positionChanged);
}

void VideoPlayerController::setVideoOutput(QGraphicsVideoItem *videoItem)
{
    player->setVideoOutput(videoItem);
}

void VideoPlayerController::setSource(const QString &filePath)
{
    player->setSource(QUrl::fromLocalFile(filePath));
}

void VideoPlayerController::play()
{
    player->play();
}

void VideoPlayerController::pause()
{
    player->pause();
}

void VideoPlayerController::stop()
{
    player->stop();
}

void VideoPlayerController::setVolume(int volume)
{
    audioOutput->setVolume(volume / 100.0);
}

void VideoPlayerController::setMuted(bool muted)
{
    audioOutput->setMuted(muted);
}

void VideoPlayerController::setPosition(qint64 position)
{
    player->setPosition(position);
}

void VideoPlayerController::seekForward()
{
    setPosition(position() + 20000); // 20 seconds forward
}

void VideoPlayerController::seekBackward()
{
    setPosition(position() - 20000); // 20 seconds backward
}

qint64 VideoPlayerController::position() const
{
    return player->position();
}

qint64 VideoPlayerController::duration() const
{
    return player->duration();
}
