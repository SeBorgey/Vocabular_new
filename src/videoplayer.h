#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include <QtMultimedia/QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QtMultimediaWidgets>
#include <QMediaPlayer>
#include <QRegularExpression>
#include "backend.h"
#include "main_vocabulary.h"
#include "multiselectlabel.h"
#include "subtitleextractor.h"
#include "subtitleselectiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class VideoPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    void set_vocab(Main_vocabulary* vocab);

    ~VideoPlayer();

private slots:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 duration);
    void on_actionOpen_triggered();
    void onSubtitlesSelected(const QString& russianPath, const QString& englishPath, SubtitleTrack& russianTrack, SubtitleTrack& englishTrack);
    void on_horizontalSlider_Duration_valueChanged(int value);


    void on_pushButton_Play_Pause_clicked();

    void on_pushButton_Stop_clicked();

    void on_pushButton_Volume_clicked();

    void on_horizontalSlider_Volume_valueChanged(int value);

    void on_pushButton_Seek_Forward_clicked();

    void on_pushButton_Seek_Backward_clicked();

    void on_actionenglish_triggered();

    void on_actionrussian_triggered();

    void on_pushButtonSub_clicked();

    void on_pushButtonAdd_clicked();
protected:
    bool event(QEvent *event);
private:
    SubtitleExtractor extractor;
    QString englishSubsPath;
    QString russianSubsPath;
    bool tryFindSubOut();
    bool tryFindSubIn();
    QString FileName;
    Main_vocabulary *main_vocab;
    MultiSelectLabel* lineEditEnglish_new;
    MultiSelectLabel* lineEditRussian_new;
    QLineEdit* lineEditFileName;
    QLineEdit* lineEditEnglishSubs;
    QLineEdit* lineEditRussianSubs;
    QPushButton* pushButtonManualSubs;
    BackEnd *russiansubs = nullptr;
    BackEnd *englishsubs = nullptr;
    Ui::MainWindow *ui;
    QMediaPlayer *Player;
    QAudioOutput *audioOutput;
    // QVideoWidget *Video;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsVideoItem *item;
    qint64 mDuration;
    bool IS_Pause = true;
    bool IS_Muted = false;
    QString russianText;
    QString englishText;
    void updateDuration(qint64 duration);
    void showSubtitleSelectionDialog();
    void activateSubs();
};
#endif // VIDEOPLAYER_H
