#ifndef VIDEOPLAYERCONTROLLER_H
#define VIDEOPLAYERCONTROLLER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsVideoItem>
#include <QMediaMetaData>
class VideoPlayerController : public QObject
{
    Q_OBJECT

public:
    explicit VideoPlayerController(QObject *parent = nullptr);

    void setVideoOutput(QGraphicsVideoItem *videoItem);
    void setSource(const QString &filePath);
    void play();
    void pause();
    void stop();
    void setVolume(int volume);
    void setMuted(bool muted);
    void setPosition(qint64 position);
    void seekForward();
    void seekBackward();
    qint64 position() const;
    qint64 duration() const;
    QStringList audioTracks;
    void getAudioTracks();
    void setAudioTrack(int index);
signals:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void audioTracksGot();

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
};

#endif // VIDEOPLAYERCONTROLLER_H
