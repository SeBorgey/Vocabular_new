#ifndef SUBTITLESELECTIONDIALOG_H
#define SUBTITLESELECTIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <QLabel>
#include <QGroupBox>
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsVideoItem>
#include <QMediaMetaData>
#include <QString>
#include "subtitleextractor.h"

class SubtitleSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    SubtitleSelectionDialog(const QString &videoPath, QList<SubtitleTrack>& subtitleTracks,
                            const QStringList& audioTracks, QWidget *parent = nullptr);

    QString getRussianSubtitlePath() const { return mRussianSubtitlePath; }
    QString getEnglishSubtitlePath() const { return mEnglishSubtitlePath; }
signals:
    void subtitlesAndAudioSelected(const QString& russianPath, const QString& englishPath,
                                   SubtitleTrack& russianTrack, SubtitleTrack& englishTrack,
                                   int audioTrackIndex);
private slots:
    void onRussianFileSelect();
    void onEnglishFileSelect();
    void onRussianTrackSelect(int row);
    void onEnglishTrackSelect(int row);
    void onAccept();
    void onAudioTrackSelect(int row);

private:
    void setupUI();
    void loadSubtitleTracks();
    void loadAudioTracks();
    QGroupBox *mAudioGroup;
    QRadioButton *mAudioTrackRadio;
    QListWidget *mAudioTrackList;
    QLineEdit *mAudioSelectedTrack;
    int mSelectedAudioTrackIndex;

    QString mVideoPath;
    QString mRussianSubtitlePath;
    QString mEnglishSubtitlePath;
    SubtitleTrack mRussianSelectedTrack;
    SubtitleTrack mEnglishSelectedTrack;
    QList<SubtitleTrack> mTracks;
    QStringList mAudioTracks;

    QRadioButton *mRussianFileRadio;
    QRadioButton *mRussianTrackRadio;
    QRadioButton *mEnglishFileRadio;
    QRadioButton *mEnglishTrackRadio;
    QPushButton *mRussianFileButton;
    QPushButton *mEnglishFileButton;
    QListWidget *mRussianTrackList;
    QListWidget *mEnglishTrackList;
    QLineEdit *mRussianSelectedSub;
    QLineEdit *mEnglishSelectedSub;
};


#endif // SUBTITLESELECTIONDIALOG_H
