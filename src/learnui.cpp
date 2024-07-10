#include "learnui.h"
#include <QFont>

LearnUI::LearnUI(QWidget *parent)
    : QGroupBox(parent)
{
    setMinimumWidth(600);
    setupUi();
}

void LearnUI::setupUi()
{
    this->setWindowTitle("Learn");

    mainLayout = new QVBoxLayout(this);

    pushButtonWord = new QPushButton(this);
    pushButtonWord->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pushButtonWord->setMinimumHeight(500);

    wordLabel = new QLabel(this);
    QFont font;
    font.setPointSize(30);
    wordLabel->setFont(font);
    wordLabel->setAlignment(Qt::AlignCenter);
    wordLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    wordLabel->setWordWrap(true);

    QVBoxLayout *innerLayout = new QVBoxLayout(pushButtonWord);
    innerLayout->addWidget(wordLabel);
    innerLayout->setContentsMargins(20, 20, 20, 20);  // Добавляем отступы

    pushButtonWord->setLayout(innerLayout);

    buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(0);

    pushButtonDecline = new QPushButton("Decline", this);
    pushButtonDecline->setMinimumHeight(200);
    QFont buttonFont;
    buttonFont.setPointSize(14);
    buttonFont.setBold(true);
    pushButtonDecline->setFont(buttonFont);
    pushButtonDecline->setStyleSheet("background-color: rgb(198, 70, 0); color: rgb(0, 0, 0);");

    pushButtonAccept = new QPushButton("Accept", this);
    pushButtonAccept->setMinimumHeight(200);
    pushButtonAccept->setFont(buttonFont);
    pushButtonAccept->setStyleSheet("background-color: rgb(38, 162, 105); color: rgb(0, 0, 0);");

    buttonLayout->addWidget(pushButtonDecline);
    buttonLayout->addWidget(pushButtonAccept);

    mainLayout->addWidget(pushButtonWord, 1);  // Устанавливаем коэффициент растяжения 1
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
    wordLabel->setText(current.first);
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
    wordLabel->setText(current.first);
    pushButtonWord->setEnabled(true);
}

void LearnUI::on_pushButtonAccept_clicked()
{
    pushButtonAccept->setEnabled(false);
    pushButtonDecline->setEnabled(false);
    currentLearning->accept();
    current = currentLearning->getCurrentWord();
    wordLabel->setText(current.first);
    pushButtonWord->setEnabled(true);
}

void LearnUI::on_pushButtonWord_clicked()
{
    pushButtonWord->setEnabled(false);
    wordLabel->setText(current.second);
    pushButtonAccept->setEnabled(true);
    pushButtonDecline->setEnabled(true);
}
