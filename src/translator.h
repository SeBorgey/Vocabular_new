#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <QObject>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include <QRegularExpression>

class Translator : public QObject
{
    Q_OBJECT

public:
    explicit Translator(QObject *parent = nullptr);
    void translate(const QString &word, bool isEnglish);

signals:
    void translationReady(const QStringList &translations);
    void errorOccurred(const QString &error);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QStringList parseTranslations(const QString &content, bool isEnglish);
};

#endif // TRANSLATOR_H
