#include "learnui.h"
#include <QFont>

LearnUI::LearnUI(QWidget *parent)
    : QGroupBox(parent)
{
    setupUi();
}

void LearnUI::setupUi()
{
    this->setGeometry(0, 0, 875, 725);
    this->setWindowTitle("Learn");

    mainLayout = new QVBoxLayout(this);

    pushButtonWord = new QPushButton("Word", this);
    pushButtonWord->setMinimumSize(0, 500);
    QFont font;
    font.setPointSize(30);
    pushButtonWord->setFont(font);

    buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(0);

    pushButtonDecline = new QPushButton("Decline", this);
    pushButtonDecline->setMinimumSize(0, 200);
    QFont buttonFont;
    buttonFont.setPointSize(14);
    buttonFont.setBold(true);
    pushButtonDecline->setFont(buttonFont);
    pushButtonDecline->setStyleSheet("background-color: rgb(198, 70, 0); color: rgb(0, 0, 0);");

    pushButtonAccept = new QPushButton("Accept", this);
    pushButtonAccept->setMinimumSize(0, 200);
    pushButtonAccept->setFont(buttonFont);
    pushButtonAccept->setStyleSheet("background-color: rgb(38, 162, 105); color: rgb(0, 0, 0);");

    buttonLayout->addWidget(pushButtonDecline);
    buttonLayout->addWidget(pushButtonAccept);

    mainLayout->addWidget(pushButtonWord);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(pushButtonDecline, &QPushButton::clicked, this, &LearnUI::on_pushButtonDecline_clicked);
    connect(pushButtonAccept, &QPushButton::clicked, this, &LearnUI::on_pushButtonAccept_clicked);
    connect(pushButtonWord, &QPushButton::clicked, this, &LearnUI::on_pushButtonWord_clicked);
}

void LearnUI::set_vocab(MainVocabulary * vocab)
{
    mainVocab = vocab;
}

void LearnUI::run_ui(){
    currentLearning = new CurrentLearning(mainVocab);
    currentLearning->chooseWordsForLearning();
    current = currentLearning->getCurrentWord();
    pushButtonWord->setText(current.first);
    pushButtonAccept->setEnabled(false);
    pushButtonDecline->setEnabled(false);
}

LearnUI::~LearnUI()
{
}

void LearnUI::on_pushButtonDecline_clicked()
{
    pushButtonAccept->setEnabled(false);
    pushButtonDecline->setEnabled(false);
    currentLearning->next();
    current = currentLearning->getCurrentWord();
    pushButtonWord->setText(current.first);
    pushButtonWord->setEnabled(true);
}

void LearnUI::on_pushButtonAccept_clicked()
{
    pushButtonAccept->setEnabled(false);
    pushButtonDecline->setEnabled(false);
    currentLearning->accept();
    current = currentLearning->getCurrentWord();
    pushButtonWord->setText(current.first);
    pushButtonWord->setEnabled(true);
}

void LearnUI::on_pushButtonWord_clicked()
{
    pushButtonWord->setEnabled(false);
    pushButtonWord->setText(current.second);
    pushButtonAccept->setEnabled(true);
    pushButtonDecline->setEnabled(true);
}
