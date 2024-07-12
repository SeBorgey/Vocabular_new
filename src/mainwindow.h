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
#include "mainvocabulary.h"
#include "subtitleextractor.h"
#include "hovereventfilter.h"
#include <QKeyEvent>
#include "customlabel.h"
#include "translator.h"
struct TranslationContext {
    QString originalWord;
    bool isEnglish;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setVocabulary(MainVocabulary* vocab);

protected:
    void keyPressEvent(QKeyEvent *event) override;

    bool event(QEvent *event);
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

    void handleHoverEnteredSubtitleButton();
    void handleHoverLeftSubtitleButton();
private:
    TranslationContext translationContext;
    enum{
        Hover,
        NotHover
    } cursorSubAt = NotHover;
    VideoPlayerUI *ui;
    VideoPlayerController *playerController;
    SubtitleManager *subtitleManager;
    MainVocabulary *mainVocab;
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
    HoverEventFilter *hoverFilter;
    QMenu *createScrollableMenu(const QStringList &items);
private:
    Translator *translator;
    QMenu *translationMenu;
    void showTranslationMenu(const QPoint &pos, bool isEnglish);

private slots:
    void onTranslationReady(const QStringList &translations);
    void onTranslationError(const QString &error);
    void addWordToDictionary(const QString &word, const QString &translation);

};

#endif // MAINWINDOW_H
