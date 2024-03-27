#include "mainmenu.h"
#include "ui_mainmenu.h"


MainMenu::MainMenu(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::MainMenu)
    , mainwindow(new VideoPlayer)
    , learnui(new LearnUI)
{
    ui->setupUi(this);
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

