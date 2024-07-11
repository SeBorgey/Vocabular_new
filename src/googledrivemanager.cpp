#include "googledrivemanager.h"
#include <QtNetwork>
#include <QtCore>
#include <QDesktopServices>
#include <QOAuthHttpServerReplyHandler>

GoogleDriveManager::GoogleDriveManager(QObject *parent) : QObject(parent),settings("MIPT", "Vocabular")
{
    qDebug() << "Settings file:" << settings.fileName();
    oauth2.setScope(SCOPE);

    oauth2.setAuthorizationUrl(QUrl(AUTH_URL + "?access_type=offline&prompt=consent&approval_prompt=force"));

    oauth2.setClientIdentifier(CLIENT_ID);
    oauth2.setClientIdentifierSharedKey(CLIENT_SECRET);
    oauth2.setAccessTokenUrl(QUrl(TOKEN_URL));
    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    oauth2.setReplyHandler(replyHandler);
    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,[=](QUrl url) {
                   QUrlQuery query(url);

                   query.addQueryItem("prompt", "consent");      // Param required to get data everytime
                   query.addQueryItem("access_type", "offline"); // Needed for Refresh Token (as AccessToken expires shortly)
                   url.setQuery(query);
                   QDesktopServices::openUrl(url);
               });
    connect(&oauth2, &QOAuth2AuthorizationCodeFlow::granted, this, &GoogleDriveManager::onAuthorizationFinished);

    QString storedRefreshToken = settings.value("refresh_token").toString();
        qDebug() << "Stored refresh token:" << (storedRefreshToken.isEmpty() ? "empty" : "exists");
        if (!storedRefreshToken.isEmpty()) {
            oauth2.setRefreshToken(storedRefreshToken);
        }

}
void GoogleDriveManager::refreshAccessToken()
{
    qDebug() << "Refreshing access token...";
    if (oauth2.refreshToken().isEmpty()) {
        qDebug() << "No refresh token available, starting new authorization";
        authorize();
        return;
    }
    qDebug() << "Refreshing access token...";
    QUrl url(TOKEN_URL);
    QUrlQuery query;
    query.addQueryItem("client_id", CLIENT_ID);
    query.addQueryItem("client_secret", CLIENT_SECRET);
    query.addQueryItem("refresh_token", oauth2.refreshToken());
    query.addQueryItem("grant_type", "refresh_token");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = networkManager.post(request, query.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObject = jsonResponse.object();
            QString newAccessToken = jsonObject["access_token"].toString();
            oauth2.setToken(newAccessToken);
            qDebug() << "Access token refreshed successfully";
            emit authorizationFinished(true);
        } else {
            qDebug() << "Error refreshing token:" << reply->errorString();
            clearStoredAuth();
            authorize();
        }
        reply->deleteLater();
    });
}

void GoogleDriveManager::onTokenChanged(const QString &token)
{
    if (!token.isEmpty()) {
        emit authorizationFinished(true);
    }
}
void GoogleDriveManager::clearStoredAuth()
{
    qDebug() << "Clearing stored auth data";
    settings.remove("refresh_token");
    settings.sync();
    qDebug() << "Auth data cleared, refresh token is now:" << settings.value("refresh_token").toString();
    oauth2.setRefreshToken(QString());
    oauth2.setToken(QString());
}
void GoogleDriveManager::authorize()
{

    qDebug() << "Authorizing...";
    QString storedRefreshToken = settings.value("refresh_token").toString();
    qDebug() << "Stored refresh token:" << (storedRefreshToken.isEmpty() ? "empty" : "exists");

    if (storedRefreshToken.isEmpty()) {
        qDebug() << "No refresh token, starting new authorization";
        qDebug() << "Authorization URL:" << oauth2.authorizationUrl().toString();
        connect(&oauth2, &QOAuth2AuthorizationCodeFlow::error, [this](const QString &error, const QString &errorDescription, const QUrl &uri) {
            qDebug() << "OAuth Error:" << error << errorDescription << uri;
            clearStoredAuth();
            emit authorizationFinished(false);
        });
        oauth2.grant();
    } else {
        qDebug() << "Refresh token found, refreshing access token";
        oauth2.setRefreshToken(storedRefreshToken);
        refreshAccessToken();
    }
}

void GoogleDriveManager::onAuthorizationFinished()
{
    qDebug() << "Authorization finished";

    qDebug() << "Current access token:" << oauth2.token();

    qDebug() << "Current refresh token:" << oauth2.refreshToken();

    QVariantMap response = oauth2.extraTokens();
    qDebug() << "Full response from Google:";
    for(auto it = response.begin(); it != response.end(); ++it) {
        qDebug() << it.key() << ":" << it.value().toString();
    }

    QString refreshToken = oauth2.refreshToken();
    qDebug() << "Saving refresh token:" << (refreshToken.isEmpty() ? "empty" : refreshToken);
    settings.setValue("refresh_token", refreshToken);
    settings.sync();
    qDebug() << "Refresh token saved:" << settings.value("refresh_token").toString();
    emit authorizationFinished(true);
}

QString GoogleDriveManager::getAccessToken()
{
    if (oauth2.token().isEmpty()) {
        if (!oauth2.refreshToken().isEmpty()) {
            refreshAccessToken();
        } else {
            authorize();
        }
        QEventLoop loop;
        connect(this, &GoogleDriveManager::authorizationFinished, &loop, &QEventLoop::quit);
        loop.exec();
    }
    return oauth2.token();
}

bool GoogleDriveManager::isAuthorized()
{
    bool hasToken = !oauth2.token().isEmpty();
    bool hasRefreshToken = !oauth2.refreshToken().isEmpty();
    qDebug() << "isAuthorized: hasToken =" << hasToken << ", hasRefreshToken =" << hasRefreshToken;
    return hasToken || hasRefreshToken;
}

void GoogleDriveManager::checkFileExists(const QString& fileName)
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

void GoogleDriveManager::downloadFile(const QString& fileName, const QString& savePath)
{
    QString fileId = getFileId(fileName);
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
