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

    mainLayout->addWidget(pushButtonWatch);
    mainLayout->addWidget(pushButtonLearn);
    mainLayout->addWidget(pushButtonEdit);

    setLayout(mainLayout);

    connect(pushButtonWatch, &QPushButton::clicked, this, &MainMenu::on_pushButtonWatch_clicked);
    connect(pushButtonLearn, &QPushButton::clicked, this, &MainMenu::on_pushButtonLearn_clicked);
    connect(pushButtonEdit, &QPushButton::clicked, this, &MainMenu::on_pushButtonEdit_clicked);
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
