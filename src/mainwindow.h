#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include <QtMultimedia/QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QtMultimediaWidgets>
#include <QMediaPlayer>
#include <QRegularExpression>
#include "videoplayerui.h"
#include "videoplayercontroller.h"
#include "subtitlemanager.h"
#include "main_vocabulary.h"
#include "subtitleextractor.h"
#include <QKeyEvent>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setVocabulary(Main_vocabulary* vocab);

protected:
    bool event(QEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void onOpenFileTriggered();
    void onPlayPauseClicked();
    void onStopClicked();
    void onVolumeClicked();
    void onVolumeChanged(int value);
    void onSeekForwardClicked();
    void onSeekBackwardClicked();
    void onSubtitleClicked();
    void onAddWordClicked();
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onDurationSliderValueChanged(int value);
    void onSubtitlesAndAudioSelected(const QString& russianPath, const QString& englishPath,
                             SubtitleTrack& russianTrack, SubtitleTrack& englishTrack,
                             int audioTrackIndex);
    void onSetEngAudio();
    void toggleFullScreen();

private:
    enum{
        Hover,
        NotHover
    } cursorSubAt = NotHover;
    VideoPlayerUI *ui;
    VideoPlayerController *playerController;
    SubtitleManager *subtitleManager;
    Main_vocabulary *mainVocab;
    SubtitleExtractor extractor;
    QString currentFileName;
    QString englishSubsPath;
    QString russianSubsPath;
    bool isPaused;
    bool isMuted;
    QString russianText;
    QString englishText;
    void setupConnections();
    void updateDurationDisplay(qint64 duration);
    bool tryFindSubtitles();
    void showSubtitleSelectionDialog();
    void activateSubs();
};

#endif // MAINWINDOW_H
