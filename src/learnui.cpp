#include "learnui.h"
#include "ui_learnui.h"

LearnUI::LearnUI(QWidget *parent)
    : QGroupBox(parent)
    , ui(new Ui::LearnUI)
{
    ui->setupUi(this);
}

void LearnUI::set_vocab(Main_vocabulary * vocab)
{
    main_vocab = vocab;

}

void LearnUI::run_ui(){
    current_learning = new Current_learning(main_vocab);
    current_learning->choose_words_for_learning();
    current=current_learning->get_current_word();
    ui->pushButtonWord->setText(current.first);
    ui->pushButtonAccept->setEnabled(false);
    ui->pushButtonDecline->setEnabled(false);
}

LearnUI::~LearnUI()
{
    delete ui;
}

void LearnUI::on_pushButtonDecline_clicked()
{
    ui->pushButtonAccept->setEnabled(false);
    ui->pushButtonDecline->setEnabled(false);
    current_learning->next();
    current = current_learning->get_current_word();
    ui->pushButtonWord->setText(current.first);
    ui->pushButtonWord->setEnabled(true);
}


void LearnUI::on_pushButtonAccept_clicked()
{
    ui->pushButtonAccept->setEnabled(false);
    ui->pushButtonDecline->setEnabled(false);
    current_learning->accept();
    current = current_learning->get_current_word();
    ui->pushButtonWord->setText(current.first);
    ui->pushButtonWord->setEnabled(true);
}


void LearnUI::on_pushButtonWord_clicked()
{
    ui->pushButtonWord->setEnabled(false);
    ui->pushButtonWord->setText(current.second);
    ui->pushButtonAccept->setEnabled(true);
    ui->pushButtonDecline->setEnabled(true);
}

