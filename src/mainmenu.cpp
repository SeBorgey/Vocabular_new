#include "mainmenu.h"
#include "ui_mainmenu.h"


MainMenu::MainMenu(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::MainMenu)
    , mainwindow(new VideoPlayer)
    , learnui(new LearnUI)
    , main_vocab(new Main_vocabulary)
{
    ui->setupUi(this);
    learnui->set_vocab(main_vocab);
    mainwindow->set_vocab(main_vocab);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_pushButtonWatch_clicked()
{
    mainwindow->show();
}


void MainMenu::on_pushButtonLearn_clicked()
{
    learnui->show();
}

