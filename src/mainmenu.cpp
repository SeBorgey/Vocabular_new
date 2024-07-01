#include "mainmenu.h"
#include "ui_mainmenu.h"


MainMenu::MainMenu(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    main_vocab = new Main_vocabulary();
    mainWindow = nullptr;
    learnui = nullptr;
}

MainMenu::~MainMenu()
{
    delete ui;
    delete main_vocab;
    delete mainWindow;
    delete learnui;
}

void MainMenu::on_pushButtonWatch_clicked()
{
    if (!mainWindow) {
        mainWindow = new MainWindow();
        mainWindow->setVocabulary(main_vocab);
    }
    mainWindow->show();
}


void MainMenu::on_pushButtonLearn_clicked()
{
    if (!learnui) {
        learnui = new LearnUI();
        learnui->set_vocab(main_vocab);
    }
    learnui->show();

}

