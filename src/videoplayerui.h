#ifndef VIDEOPLAYERUI_H
#define VIDEOPLAYERUI_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
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

class VideoPlayerUI : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayerUI(QWidget *parent = nullptr);

    QAction *openAction;
    QPushButton *playPauseButton;
    QPushButton *stopButton;
    QPushButton *volumeButton;
    QSlider *volumeSlider;
    QSlider *durationSlider;
    QLabel *currentTimeLabel;
    QLabel *totalTimeLabel;
    QGraphicsView *videoView;
    QGraphicsVideoItem *videoItem;
    QPushButton *subtitleButton;
    QPushButton *addWordButton;
    MultiSelectLabel *englishSubtitleEdit;
    MultiSelectLabel *russianSubtitleEdit;
    QLineEdit *fileNameEdit;
    QLineEdit *englishSubsEdit;
    QLineEdit *russianSubsEdit;
    QLineEdit *audioEdit;
    QPushButton *seekForwardButton;
    QPushButton *seekBackwardButton;
    QPushButton *manualSubsButton;
    QGroupBox *groupBox_Video;
    QHBoxLayout *videoLayout;

    void updateVideoSize();
protected:
    void resizeEvent(QResizeEvent *event);
private:
    void createWidgets();
    void setupLayout();
    void createMenuAndToolbar(QMainWindow *mainWindow);
    void updateFontSizes();
public:
    void enterFullScreen();
    void exitFullScreen();
    bool isFullScreen;
private:

    QRect previousGeometry;
    Qt::WindowFlags originalFlags;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void fullscreenToggled();
private:
    QLayout* originalLayout;
    int originalIndex;
private:
    QList<QWidget*> originalWidgetOrder;
};

#endif // VIDEOPLAYERUI_H
