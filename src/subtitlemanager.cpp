#include "subtitlemanager.h"

SubtitleManager::SubtitleManager(QObject *parent)
    : QObject(parent), englishSubtitles(nullptr), russianSubtitles(nullptr)
{
}

SubtitleManager::~SubtitleManager()
{
    delete englishSubtitles;
    delete russianSubtitles;
}

void SubtitleManager::loadSubtitles(const QString &englishPath, const QString &russianPath)
{
    delete englishSubtitles;
    delete russianSubtitles;

    englishSubtitles = new BackEnd(englishPath);
    russianSubtitles = new BackEnd(russianPath);
}

QString SubtitleManager::getEnglishSubtitle(qint64 position)
{
    if (englishSubtitles && englishSubtitles->readed) {
        return englishSubtitles->getSubtitleText(static_cast<double>(position));
    }
    return QString();
}

QString SubtitleManager::getRussianSubtitle(qint64 position)
{
    if (russianSubtitles && russianSubtitles->readed) {
        return russianSubtitles->getSubtitleText(static_cast<double>(position));
    }
    return QString();
}
