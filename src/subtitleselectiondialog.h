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
#include "subtitleextractor.h"
class SubtitleSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubtitleSelectionDialog(const QString& videoPath, QList<SubtitleTrack>& tracks, QWidget *parent = nullptr);

    QString getRussianSubtitlePath() const { return mRussianSubtitlePath; }
    QString getEnglishSubtitlePath() const { return mEnglishSubtitlePath; }
signals:
    void subtitlesSelected(const QString& russianPath, const QString& englishPath,
                           SubtitleTrack& russianTrack, SubtitleTrack& englishTrack);
private slots:
    void onRussianFileSelect();
    void onEnglishFileSelect();
    void onRussianTrackSelect(int row);
    void onEnglishTrackSelect(int row);
    void onAccept();

private:
    void setupUI();
    void loadSubtitleTracks();

    QString mVideoPath;
    QString mRussianSubtitlePath;
    QString mEnglishSubtitlePath;
    SubtitleTrack mRussianSelectedTrack;
    SubtitleTrack mEnglishSelectedTrack;
    QList<SubtitleTrack> mTracks;

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
