#ifndef SUBTITLEEXTRACTOR_H
#define SUBTITLEEXTRACTOR_H

#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <QTextStream>

struct SubtitleTrack {
    QString name;
    QString language;
    int number;
};

class SubtitleExtractor {
public:
    QString russianSubtitlePath;
    QString englishSubtitlePath;

    QList<SubtitleTrack> getSubtitleTracks();
    QList<SubtitleTrack> tracks;
    void extractSubtitleTrack(const SubtitleTrack& track);

    void extractRussianAndEnglishSubtitles();
    void deleteExtractedSubtitles();

    void setFileName(const QString &fileName);
private:
    QString mkvFilePath;
    SubtitleTrack parseSubtitleTrack(const QStringList &buffer);
};

#endif // SUBTITLEEXTRACTOR_H
