#include "learnui.h"
#include "ui_learnui.h"

LearnUI::LearnUI(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::LearnUI)
{
    ui->setupUi(this);
}

LearnUI::~LearnUI()
{
    delete ui;
}

void LearnUI::on_pushButtonDecline_clicked()
{

}


void LearnUI::on_pushButtonAccept_clicked()
{

}


void LearnUI::on_pushButtonWord_clicked()
{

}

