#ifndef SUBTITLEMANAGER_H
#define SUBTITLEMANAGER_H

#include <QObject>
#include "backend.h"

class SubtitleManager : public QObject
{
    Q_OBJECT

public:
    explicit SubtitleManager(QObject *parent = nullptr);
    ~SubtitleManager();

    void loadSubtitles(const QString &englishPath, const QString &russianPath);
    QString getEnglishSubtitle(qint64 position);
    QString getRussianSubtitle(qint64 position);

private:
    BackEnd *englishSubtitles;
    BackEnd *russianSubtitles;
};

#endif // SUBTITLEMANAGER_H
