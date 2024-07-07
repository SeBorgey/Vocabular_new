#include "mainmenu.h"
#include <QScreen>
#include <QApplication>

MainMenu::MainMenu(QWidget *parent)
    : QGroupBox(parent)
{
    setupUi();
    mainVocab = new MainVocabulary();
    mainWindow = nullptr;
    learnui = nullptr;

    this->setFixedWidth(400);

    this->adjustSize();

    centerWindow();
}

void MainMenu::setupUi()
{
    this->setWindowTitle("Vocabular");

    mainLayout = new QVBoxLayout(this);

    pushButtonWatch = new QPushButton("Watch", this);
    pushButtonLearn = new QPushButton("Learn", this);

    mainLayout->addWidget(pushButtonWatch);
    mainLayout->addWidget(pushButtonLearn);

    // mainLayout->addWidget(new QPushButton("New Button", this));

    setLayout(mainLayout);

    connect(pushButtonWatch, &QPushButton::clicked, this, &MainMenu::on_pushButtonWatch_clicked);
    connect(pushButtonLearn, &QPushButton::clicked, this, &MainMenu::on_pushButtonLearn_clicked);
}

void MainMenu::centerWindow()
{
    QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

MainMenu::~MainMenu()
{
    delete mainVocab;
    delete mainWindow;
    delete learnui;
}

void MainMenu::on_pushButtonWatch_clicked()
{
    if (!mainWindow) {
        mainWindow = new MainWindow();
        mainWindow->setVocabulary(mainVocab);
    }
    mainWindow->showMaximized();
}

void MainMenu::on_pushButtonLearn_clicked()
{
    if (!learnui) {
        learnui = new LearnUI();
        learnui->set_vocab(mainVocab);
    }
    learnui->run_ui();
    learnui->show();
}
