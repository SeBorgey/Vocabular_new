#include "mainmenu.h"
#include <QScreen>
#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>

MainMenu::MainMenu(QWidget *parent)
    : QGroupBox(parent)
{
    setupUi();
    main_vocab = new MainVocabulary();
    learnui = nullptr;


    googleDriveManager = new GoogleDriveManager(this);
    localFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/MyWords.txt";

    connect(googleDriveManager, &GoogleDriveManager::authorizationFinished, this, &MainMenu::onAuthorizationFinished);
    connect(googleDriveManager, &GoogleDriveManager::downloadFinished, this, &MainMenu::onDownloadFinished);
    connect(googleDriveManager, &GoogleDriveManager::uploadFinished, this, &MainMenu::onUploadFinished);
    connect(googleDriveManager, &GoogleDriveManager::fileExistsChecked, this, &MainMenu::onFileExistsChecked, Qt::SingleShotConnection);
    connect(googleDriveManager, &GoogleDriveManager::fileModifiedTimeReceived, this, &MainMenu::onFileModifiedTimeReceived);
#ifndef Q_OS_ANDROID
    mainWindow = nullptr;    
    wordEditor = nullptr;

    this->setFixedWidth(400);
    this->adjustSize();
    centerWindow();
#endif
}

void MainMenu::on_pushButtonLearn_clicked()
{
    if (!QFile::exists(localFilePath)) {
        QMessageBox::critical(this, "Error", "Vocabulary does not exist.");
        return;
    }
    if (!learnui) {
        learnui = new LearnUI();
        learnui->set_vocab(main_vocab);
    }
    learnui->run_ui();
    learnui->show();
}

MainMenu::~MainMenu()
{
    delete main_vocab;
    delete learnui;
    delete googleDriveManager;
#ifndef Q_OS_ANDROID
    delete mainWindow;
    delete wordEditor;
#endif
}

void MainMenu::setupUi()
{
    this->setWindowTitle("Vocabular");

    mainLayout = new QVBoxLayout(this);

    pushButtonLearn = new QPushButton("Learn", this);    
    pushButtonImport = new QPushButton("Import Words", this);
    pushButtonExport = new QPushButton("Export Words", this);
    pushButtonSync = new QPushButton("Sync with Google Drive", this);

#ifndef Q_OS_ANDROID
    pushButtonWatch = new QPushButton("Watch", this);
    pushButtonEdit = new QPushButton("Edit Words", this);

    connect(pushButtonWatch, &QPushButton::clicked, this, &MainMenu::on_pushButtonWatch_clicked);
    connect(pushButtonEdit, &QPushButton::clicked, this, &MainMenu::on_pushButtonEdit_clicked);

    mainLayout->addWidget(pushButtonWatch);
    mainLayout->addWidget(pushButtonEdit);
#endif

#ifdef Q_OS_ANDROID
    QFont font;
    font.setPointSize(30);
    pushButtonLearn->setFont(font);
    pushButtonImport->setFont(font);
    pushButtonExport->setFont(font);    
    pushButtonSync->setFont(font);

    pushButtonSync->setMinimumSize(0, 150);
    pushButtonLearn->setMinimumSize(0, 150);
    pushButtonImport->setMinimumSize(0, 150);
    pushButtonExport->setMinimumSize(0, 150);
#endif
    mainLayout->addWidget(pushButtonLearn);
    mainLayout->addWidget(pushButtonImport);
    mainLayout->addWidget(pushButtonExport);
    mainLayout->addWidget(pushButtonSync);

    setLayout(mainLayout);

    connect(pushButtonSync, &QPushButton::clicked, this, &MainMenu::on_pushButtonSync_clicked);
    connect(pushButtonLearn, &QPushButton::clicked, this, &MainMenu::on_pushButtonLearn_clicked);    
    connect(pushButtonImport, &QPushButton::clicked, this, &MainMenu::on_pushButtonImport_clicked);
    connect(pushButtonExport, &QPushButton::clicked, this, &MainMenu::on_pushButtonExport_clicked);


}

void MainMenu::on_pushButtonSync_clicked()
{
    pushButtonSync->setText("Wait");
    pushButtonSync->setEnabled(false);
    qDebug() << "Sync button clicked";
    if (!googleDriveManager->isAuthorized()) {
        qDebug() << "Not authorized, starting authorization process";
        connect(googleDriveManager, &GoogleDriveManager::authorizationFinished,
                this, &MainMenu::onAuthorizationFinished, Qt::SingleShotConnection);
        googleDriveManager->authorize();
    } else {
        qDebug() << "Already authorized, starting sync";
        syncWithGoogleDrive();
    }
}

void MainMenu::onAuthorizationFinished(bool success)
{
    qDebug() << "Authorization finished, success =" << success;
    if (success) {
        syncWithGoogleDrive();
    } else {
        QMessageBox::warning(this, "Authorization Failed", "Failed to authorize with Google Drive. Please try again.");
    }
}

void MainMenu::syncWithGoogleDrive()
{
    googleDriveManager->checkFileExists("MyWords.txt");
}

void MainMenu::onFileExistsChecked(bool exists)
{
    QFileInfo localFileInfo(localFilePath);

    if (exists) {
        if (localFileInfo.exists()) {
            googleDriveManager->getFileModifiedTime("MyWords.txt");
        } else {
            googleDriveManager->downloadFile("MyWords.txt", localFilePath);
        }
    } else {
        if (localFileInfo.exists()) {
            googleDriveManager->uploadFile(localFilePath);
        } else {
            QMessageBox::information(this, "Sync", "No file to sync.");
        }
    }
}

void MainMenu::onFileModifiedTimeReceived(const QDateTime& driveModifiedTime)
{
    QFileInfo localFileInfo(localFilePath);
    QDateTime localModifiedTime = localFileInfo.lastModified();

    if (driveModifiedTime > localModifiedTime) {
        googleDriveManager->downloadFile("MyWords.txt", localFilePath);
    } else if (localModifiedTime > driveModifiedTime) {
        googleDriveManager->uploadFile(localFilePath);
    } else {
        QMessageBox::information(this, "Sync", "Files are already in sync.");
    }
}

void MainMenu::onDownloadFinished(bool success)
{
    if (success) {
        QMessageBox::information(this, "Sync", "File successfully downloaded from Google Drive.");
        main_vocab->clear();
        main_vocab->getAllWords();
    } else {
        QMessageBox::critical(this, "Sync Error", "Failed to download file from Google Drive.");
    }
    pushButtonSync->setText("Sync with Google Drive");
    pushButtonSync->setEnabled(true);
}

void MainMenu::onUploadFinished(bool success)
{
    if (success) {
        QMessageBox::information(this, "Sync", "File successfully uploaded to Google Drive.");
    } else {
        QMessageBox::critical(this, "Sync Error", "Failed to upload file to Google Drive.");
    }
    pushButtonSync->setText("Sync with Google Drive");
    pushButtonSync->setEnabled(true);
}

void MainMenu::on_pushButtonImport_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Words File", "", "Text Files (*.txt)");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Unable to open the file.");
        return;
    }
#ifdef Q_OS_ANDROID
    main_vocab->clear();
#endif
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        auto s = line.split('|');
        if (s.size() < 4) continue;

        QString english = QString(s[0]);
        QString russian = QString(s[1]);
        int times = s[2].toInt();
        QDateTime last = QDateTime::fromString(s[3].trimmed(), "yyyy.MM.dd HH:mm:ss");

        main_vocab->importWord(english, russian, times, last);
    }

    file.close();
    main_vocab->saveWords();
    QMessageBox::information(this, "Success", "Words imported successfully.");
}

void MainMenu::on_pushButtonExport_clicked()
{
#ifdef Q_OS_ANDROID
    QMessageBox::critical(this, "Error", "Root is needed for this function");
    return;
#endif

    if (!QFile::exists(localFilePath)) {
        QMessageBox::critical(this, "Error", "Source file 'MyWords.txt' does not exist.");
        return;
    }
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)+ "/MyWords.txt";
    QString downloadPath = QFileDialog::getSaveFileName(this, "Save Words File", initialPath, "Text Files (*.txt)");
    if (downloadPath.isEmpty()) {
        QMessageBox::critical(this, "Error", "Unable to get location.");
        return;
    }

    QString destFilePath = downloadPath;

    if (QFile::exists(destFilePath)) {
        if (!QFile::remove(destFilePath)) {
            QMessageBox::critical(this, "Error", "Unable to remove existing file.");
            return;
        }
    }

    if (QFile::copy(localFilePath, destFilePath)) {
        QMessageBox::information(this, "Success", "Words exported successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to copy file.");
    }
}
#ifndef Q_OS_ANDROID
void MainMenu::centerWindow()
{
    QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

void MainMenu::on_pushButtonWatch_clicked()
{
    if (!mainWindow) {
        mainWindow = new MainWindow();
        mainWindow->setVocabulary(main_vocab);
    }
    mainWindow->showMaximized();
}

void MainMenu::on_pushButtonEdit_clicked()
{
    if (!wordEditor) {
        wordEditor = new WordEditor(main_vocab);
    }
    wordEditor->show();
}
#endif
