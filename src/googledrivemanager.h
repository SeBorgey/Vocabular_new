#ifndef GOOGLEDRIVEMANAGER_H
#define GOOGLEDRIVEMANAGER_H

#include <QObject>
#include <QtNetwork>
#include <QtCore>
#include <QOAuth2AuthorizationCodeFlow>
class GoogleDriveManager : public QObject
{
    Q_OBJECT

public:
    explicit GoogleDriveManager(QObject *parent = nullptr);

    void authorize();
    bool isAuthorized();
    void downloadFile(const QString& fileName, const QString& savePath);
    void uploadFile(const QString& filePath);
    void checkFileExists(const QString& fileName);
    void getFileModifiedTime(const QString& fileName);

signals:
    void authorizationFinished();
    void downloadFinished(bool success);
    void uploadFinished(bool success);
    void fileExistsChecked(bool exists);
    void fileModifiedTimeReceived(const QDateTime& modifiedTime);

private slots:
    void onAuthorizationFinished();
    void onDownloadFinished();
    void onUploadFinished();

private:
    void refreshToken();
    QString getAccessToken();
    QString getFileId(const QString& fileName);

    QOAuth2AuthorizationCodeFlow oauth2;
    QNetworkAccessManager networkManager;
    QSettings settings;

    const QString CLIENT_ID = "282120141386-h4shmc6u1qdj707urpkrkvh3qp4g1i11.apps.googleusercontent.com";
    const QString CLIENT_SECRET = "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";
    const QString SCOPE = "https://www.googleapis.com/auth/drive.file";
    const QString AUTH_URL = "https://accounts.google.com/o/oauth2/auth";
    const QString TOKEN_URL = "https://oauth2.googleapis.com/token";
    QString getFileIdByName(const QString &fileName);
public:
    // ...
    void clearStoredAuth();

private slots:
    // ...
    void onTokenChanged(const QString &token);

private:
    // ...
    void refreshAccessToken();
};

#endif // GOOGLEDRIVEMANAGER_H
