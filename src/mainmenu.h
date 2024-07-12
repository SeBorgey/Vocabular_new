#ifndef MAINMENU_H
#define MAINMENU_H

#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "learnui.h"
#include "mainvocabulary.h"
#include "googledrivemanager.h"

#ifndef Q_OS_ANDROID
#include "mainwindow.h"
#include "wordeditor.h"
#endif

class MainMenu : public QGroupBox
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void on_pushButtonLearn_clicked();
    void on_pushButtonImport_clicked();
    void on_pushButtonExport_clicked();

private:
#ifndef Q_OS_ANDROID
    MainWindow *mainWindow;
    WordEditor *wordEditor;
#endif
    LearnUI *learnui;
    QString localFilePath;
    MainVocabulary *main_vocab;
    GoogleDriveManager *googleDriveManager;
    QPushButton *pushButtonLearn;
    QPushButton *pushButtonImport;
    QPushButton *pushButtonExport;
    QVBoxLayout *mainLayout;

    void setupUi();
    QPushButton *pushButtonSync;
    void syncWithGoogleDrive();

#ifndef Q_OS_ANDROID
    void centerWindow();
    QPushButton *pushButtonWatch;
    QPushButton *pushButtonEdit;

private slots:
    void on_pushButtonWatch_clicked();
    void on_pushButtonEdit_clicked();
#endif
private slots:
    void on_pushButtonSync_clicked();
    void onAuthorizationFinished(bool success);
    void onFileExistsChecked(bool exists);
    void onFileModifiedTimeReceived(const QDateTime &driveModifiedTime);
    void onDownloadFinished(bool success);
    void onUploadFinished(bool success);

};

#endif // MAINMENU_H
