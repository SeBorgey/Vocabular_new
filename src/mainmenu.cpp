#include "mainmenu.h"
#include <QScreen>
#include <QApplication>

MainMenu::MainMenu(QWidget *parent)
    : QGroupBox(parent)
{
    setupUi();
    main_vocab = new MainVocabulary();
    mainWindow = nullptr;
    learnui = nullptr;
    wordEditor = nullptr;

    this->setFixedWidth(400);
    this->adjustSize();
    centerWindow();
}

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

void MainMenu::on_pushButtonLearn_clicked()
{
    if (!learnui) {
        learnui = new LearnUI();
        learnui->set_vocab(main_vocab);
    }
    learnui->run_ui();
    learnui->show();
}

void MainMenu::on_pushButtonEdit_clicked()
{
    if (!wordEditor) {
        wordEditor = new WordEditor(main_vocab);
    }
    wordEditor->show();
}

MainMenu::~MainMenu()
{
    delete main_vocab;
    delete mainWindow;
    delete learnui;
    delete wordEditor;
}

void MainMenu::setupUi()
{
    this->setWindowTitle("Vocabular");

    mainLayout = new QVBoxLayout(this);

    pushButtonWatch = new QPushButton("Watch", this);
    pushButtonLearn = new QPushButton("Learn", this);
    pushButtonEdit = new QPushButton("Edit Words", this);
    pushButtonImport = new QPushButton("Import Words", this);
    pushButtonExport = new QPushButton("Export Words", this);

    mainLayout->addWidget(pushButtonWatch);
    mainLayout->addWidget(pushButtonLearn);
    mainLayout->addWidget(pushButtonEdit);
    mainLayout->addWidget(pushButtonImport);
    mainLayout->addWidget(pushButtonExport);

    setLayout(mainLayout);

    connect(pushButtonWatch, &QPushButton::clicked, this, &MainMenu::on_pushButtonWatch_clicked);
    connect(pushButtonLearn, &QPushButton::clicked, this, &MainMenu::on_pushButtonLearn_clicked);
    connect(pushButtonEdit, &QPushButton::clicked, this, &MainMenu::on_pushButtonEdit_clicked);
    connect(pushButtonImport, &QPushButton::clicked, this, &MainMenu::on_pushButtonImport_clicked);
    connect(pushButtonExport, &QPushButton::clicked, this, &MainMenu::on_pushButtonExport_clicked);
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
    QString filePath = QFileDialog::getSaveFileName(this, "Save Words File", "MyWords.txt", "Text Files (*.txt)");
    if (filePath.isEmpty()) return;

    if (QFile::copy("MyWords.txt", filePath)) {
        QMessageBox::information(this, "Success", "Words exported successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Unable to export words.");
    }
}
