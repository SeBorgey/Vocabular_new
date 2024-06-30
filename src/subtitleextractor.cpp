#include "subtitleextractor.h"
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <QTextStream>

void SubtitleExtractor::setFileName(const QString &fileName){
    mkvFilePath = fileName;
    russianSubtitlePath = "";
    englishSubtitlePath = "";
    tracks.clear();
}

QList<SubtitleTrack> SubtitleExtractor::getSubtitleTracks() {
    QProcess process;
    process.start("mkvinfo", QStringList() << mkvFilePath);
    process.waitForFinished(-1);

    QString output = process.readAllStandardOutput();
    QTextStream stream(&output);
    QStringList buffer;
    bool captureMode = false;
    int captureCount = 0;

    while (!stream.atEnd()) {
        QString line = stream.readLine();

        if (captureMode) {
            buffer.append(line);
            captureCount++;
            if (captureCount == 6) { // 5 строк после "Track type: subtitles"
                SubtitleTrack track = parseSubtitleTrack(buffer);
                if (track.number != -1) {
                    tracks.append(track);
                }
                buffer.clear();
                captureMode = false;
            }
        } else {
            buffer.append(line);
            if (buffer.size() > 3) {
                buffer.removeFirst();
            }
            if (line.contains("Track type: subtitles")) {
                captureMode = true;
                captureCount = 0;
            }
        }
    }

    return tracks;
}

SubtitleTrack SubtitleExtractor::parseSubtitleTrack(const QStringList& buffer) {
    SubtitleTrack track;
    track.number = -1;

    for (const QString& line : buffer) {
        if (line.contains("Track number:")) {
            QRegularExpression re("track ID for mkvmerge & mkvextract: (\\d+)");
            QRegularExpressionMatch match = re.match(line);
            if (match.hasMatch()) {
                track.number = match.captured(1).toInt();
            }
        } else if (line.contains("Name:")) {
            track.name = line.split(":").last().trimmed();
        } else if (line.contains("Language:")) {
            track.language = line.split(":").last().trimmed();
        }
    }

    return track;
}
void SubtitleExtractor::extractSubtitleTrack(const SubtitleTrack &track) {
    QFileInfo fileInfo(mkvFilePath);
    QString baseName = fileInfo.completeBaseName();
    QString outputDir = fileInfo.dir().absolutePath();
    QString language = track.language;
    QString outputPath = QString("%1/%2_%3.srt").arg(outputDir, baseName, language);

    QProcess process;
    process.start("mkvextract", QStringList() << "tracks" << mkvFilePath
                                              << QString("%1:%2").arg(track.number).arg(outputPath));
    process.waitForFinished(-1);

    if (process.exitCode() == 0) {
        if (language.toLower().contains("rus")) {
            russianSubtitlePath = outputPath;
        } else if (language.toLower().contains("eng")) {
            englishSubtitlePath = outputPath;
        }
    } else {
        qDebug() << "Error extracting subtitle track:" << process.errorString();
    }
}

void SubtitleExtractor::extractRussianAndEnglishSubtitles() {
    for (const SubtitleTrack& track : tracks) {
        if (track.language.toLower().contains("rus")) {
            extractSubtitleTrack(track);
        } else if (track.language.toLower().contains("eng")) {
            extractSubtitleTrack(track);
        }
    }
}


void SubtitleExtractor::deleteExtractedSubtitles() {
    if (!russianSubtitlePath.isEmpty()) {
        QFile::remove(russianSubtitlePath);
        russianSubtitlePath = "";
    }
    if (!englishSubtitlePath.isEmpty()) {
        QFile::remove(englishSubtitlePath);
        englishSubtitlePath = "";
    }
}
