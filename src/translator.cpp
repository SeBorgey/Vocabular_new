#include <QObject>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include <QRegularExpression>
#include "translator.h"


Translator::Translator(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Translator::onNetworkReply);
}

void Translator::translate(const QString &word, bool isEnglish)
{
    QString url;
    if (isEnglish) {
        url = QString("https://www.multitran.com/m.exe?l1=1&l2=2&s=%1").arg(word);
    } else {
        url = QString("https://www.multitran.com/m.exe?l1=2&l2=1&s=%1").arg(word);
    }

    QNetworkRequest request(url);
    manager->get(request);
}

void Translator::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QString content = reply->readAll();
        bool isEnglish = reply->url().toString().contains("l1=1");
        QStringList translations = parseTranslations(content, isEnglish);
        emit translationReady(translations);
    } else {
        emit errorOccurred(reply->errorString());
    }

    reply->deleteLater();
}

QStringList Translator::parseTranslations(const QString &content, bool isEnglish)
{
    QStringList translations;
    QRegularExpression re(isEnglish ? R"(<a href=[^>]*>([^<]+)</a>)" : R"(<a href=[^>]*2>([^<]+)</a>)");
    QRegularExpressionMatchIterator i = re.globalMatch(content);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        translations.append(match.captured(1));
        if (translations.size() > 300) break;
    }

    return translations;
}
