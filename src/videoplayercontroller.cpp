#include "videoplayercontroller.h"

VideoPlayerController::VideoPlayerController(QObject *parent)
    : QObject(parent)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    connect(player, &QMediaPlayer::durationChanged, this, &VideoPlayerController::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &VideoPlayerController::positionChanged);
    connect(player, &QMediaPlayer::tracksChanged, this, &VideoPlayerController::getAudioTracks);
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
    setPosition(position() + 5000); // 5 seconds forward
}

void VideoPlayerController::seekBackward()
{
    setPosition(position() - 5000); // 5 seconds backward
}

qint64 VideoPlayerController::position() const
{
    return player->position();
}

qint64 VideoPlayerController::duration() const
{
    return player->duration();
}

void VideoPlayerController::getAudioTracks() {
    auto tracks = player->audioTracks();
    for (int i = 0; i < tracks.size(); ++i) {
        QString language = tracks[i].stringValue(QMediaMetaData::Language);
        QString title = tracks[i].stringValue(QMediaMetaData::Title);

        QString itemText;
        if (!title.isEmpty()) {
            if (!language.isEmpty()) {
                itemText = QString("%1 (%2)").arg(title, language);
            } else {
                itemText = title;
            }
        } else if (!language.isEmpty()) {
            itemText = QString("Audio Track %1 (%2)").arg(i + 1).arg(language);
        } else {
            itemText = QString("Audio Track %1").arg(i + 1);
        }
        audioTracks.append(itemText);
    }
    emit audioTracksGot();
}

void VideoPlayerController::setAudioTrack(int index){
    player->setActiveAudioTrack(index);
}
