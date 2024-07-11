#include "googledrivemanager.h"
#include <QtNetwork>
#include <QtCore>
#include <QDesktopServices>
#include <QOAuthHttpServerReplyHandler>

GoogleDriveManager::GoogleDriveManager(QObject *parent) : QObject(parent)
{
    oauth2.setScope(SCOPE);
    oauth2.setAuthorizationUrl(QUrl(AUTH_URL));
    oauth2.setClientIdentifier(CLIENT_ID);
    oauth2.setClientIdentifierSharedKey(CLIENT_SECRET);
    oauth2.setAccessTokenUrl(QUrl(TOKEN_URL));
    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
            oauth2.setReplyHandler(replyHandler);
    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);
    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::granted, this, &GoogleDriveManager::onAuthorizationFinished);

    QString storedRefreshToken = settings.value("refresh_token").toString();
    if (!storedRefreshToken.isEmpty()) {
        oauth2.setRefreshToken(storedRefreshToken);
        refreshAccessToken();
    }
}
void GoogleDriveManager::refreshAccessToken()
{
    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::tokenChanged, this, &GoogleDriveManager::onTokenChanged);
    oauth2.refreshAccessToken();
}

void GoogleDriveManager::onTokenChanged(const QString &token)
{
    if (!token.isEmpty()) {
        emit authorizationFinished();
    }
}
void GoogleDriveManager::clearStoredAuth()
{
    settings.remove("refresh_token");
    oauth2.setRefreshToken(QString());
    oauth2.setToken(QString());
}
void GoogleDriveManager::authorize()
{
    if (oauth2.refreshToken().isEmpty()) {
        oauth2.grant();
    } else {
        refreshAccessToken();
    }
}

void GoogleDriveManager::onAuthorizationFinished()
{
    settings.setValue("refresh_token", oauth2.refreshToken());
    emit authorizationFinished();
}

QString GoogleDriveManager::getAccessToken()
{
    if (oauth2.token().isEmpty()) {
        if (!oauth2.refreshToken().isEmpty()) {
            refreshAccessToken();
        } else {
            authorize();
        }
    }
    return oauth2.token();
}

bool GoogleDriveManager::isAuthorized()
{
    return !oauth2.token().isEmpty();
}

void GoogleDriveManager::checkFileExists(const QString& fileName)
{
    qDebug()<<"checkFileExists";
    QString accessToken = getAccessToken();
    QUrl url("https://www.googleapis.com/drive/v3/files");
    QUrlQuery query;
    query.addQueryItem("q", QString("name='%1' and trashed=false").arg(fileName));
    query.addQueryItem("fields", "files(id)");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());

    QNetworkReply *reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObject = jsonResponse.object();
            QJsonArray files = jsonObject["files"].toArray();
            bool exists = !files.isEmpty();
            emit fileExistsChecked(exists);
        } else {
            emit fileExistsChecked(false);
        }
        reply->deleteLater();
    });
}

void GoogleDriveManager::getFileModifiedTime(const QString& fileName)
{
    QString fileId = getFileId(fileName);
    if (fileId.isEmpty()) {
        emit fileModifiedTimeReceived(QDateTime());
        return;
    }

    QString accessToken = getAccessToken();
    QUrl url(QString("https://www.googleapis.com/drive/v3/files/%1").arg(fileId));
    QUrlQuery query;
    query.addQueryItem("fields", "modifiedTime");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());

    QNetworkReply *reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObject = jsonResponse.object();
            QString modifiedTimeStr = jsonObject["modifiedTime"].toString();
            QDateTime modifiedTime = QDateTime::fromString(modifiedTimeStr, Qt::ISODate);
            emit fileModifiedTimeReceived(modifiedTime);
        } else {
            emit fileModifiedTimeReceived(QDateTime());
        }
        reply->deleteLater();
    });
}

QString GoogleDriveManager::getFileId(const QString& fileName)
{
    QString accessToken = getAccessToken();
    QUrl url("https://www.googleapis.com/drive/v3/files");
    QUrlQuery query;
    query.addQueryItem("q", QString("name='%1' and trashed=false").arg(fileName));
    query.addQueryItem("fields", "files(id)");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());

    QNetworkReply *reply = networkManager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray files = jsonObject["files"].toArray();
        if (!files.isEmpty()) {
            return files[0].toObject()["id"].toString();
        }
    }

    reply->deleteLater();
    return QString();
}

void GoogleDriveManager::refreshToken()
{
    QUrl url(TOKEN_URL);
    QUrlQuery query;
    query.addQueryItem("client_id", CLIENT_ID);
    query.addQueryItem("client_secret", CLIENT_SECRET);
    query.addQueryItem("refresh_token", oauth2.refreshToken());
    query.addQueryItem("grant_type", "refresh_token");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = networkManager.post(request, query.toString(QUrl::FullyEncoded).toUtf8());
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObject = jsonResponse.object();
            QString newAccessToken = jsonObject["access_token"].toString();
            oauth2.setToken(newAccessToken);
        }
        reply->deleteLater();
    });
}
QString GoogleDriveManager::getFileIdByName(const QString& fileName)
{
    QString accessToken = getAccessToken();
    QUrl url("https://www.googleapis.com/drive/v3/files");
    QUrlQuery query;
    query.addQueryItem("q", "name='" + fileName + "' and trashed=false");
    query.addQueryItem("fields", "files(id)");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());

    QNetworkReply *reply = networkManager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray files = jsonObject["files"].toArray();
        if (!files.isEmpty()) {
            return files[0].toObject()["id"].toString();
        }
    } else {
        qDebug() << "Error getting file ID:" << reply->errorString();
    }

    reply->deleteLater();
    return QString();
}
void GoogleDriveManager::downloadFile(const QString& fileName, const QString& savePath)
{
    QString fileId = getFileIdByName(fileName);
    QString accessToken = getAccessToken();
    QUrl url("https://www.googleapis.com/drive/v3/files/" + fileId + "?alt=media");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());

    QNetworkReply *reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, &GoogleDriveManager::onDownloadFinished);
}

void GoogleDriveManager::onDownloadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Download successful";
        QFile file("MyWords.txt");
        if (file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
            emit downloadFinished(true);
        } else {
            qDebug() << "Failed to open file for writing:" << file.errorString();
            emit downloadFinished(false);
        }
    } else {
        qDebug() << "Download error:" << reply->errorString();
        qDebug() << "HTTP status code:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << "Response content:" << reply->readAll();
        emit downloadFinished(false);
    }
    reply->deleteLater();
}
void GoogleDriveManager::uploadFile(const QString& filePath)
{
    QString accessToken = getAccessToken();
    QUrl url("https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart");
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::RelatedType);

    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json; charset=UTF-8"));
    QJsonObject json;
    json["name"] = QFileInfo(filePath).fileName();
    jsonPart.setBody(QJsonDocument(json).toJson());

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/octet-stream"));
    QFile *file = new QFile(filePath);
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(jsonPart);
    multiPart->append(filePart);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(accessToken).toUtf8());

    QNetworkReply *reply = networkManager.post(request, multiPart);
    multiPart->setParent(reply);
    connect(reply, &QNetworkReply::finished, this, &GoogleDriveManager::onUploadFinished);
}

void GoogleDriveManager::onUploadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error() == QNetworkReply::NoError) {
        emit uploadFinished(true);
    } else {
        emit uploadFinished(false);
    }
    reply->deleteLater();
}
