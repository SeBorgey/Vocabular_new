#ifndef LEARNUI_H
#define LEARNUI_H

#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "mainvocabulary.h"
#include "currentlearning.h"

class LearnUI : public QGroupBox
{
    Q_OBJECT

public:
    explicit LearnUI(QWidget *parent = nullptr);
    void set_vocab(MainVocabulary* vocab);
    ~LearnUI();

    void run_ui();

private slots:
    void on_pushButtonDecline_clicked();
    void on_pushButtonAccept_clicked();
    void on_pushButtonWord_clicked();

private:
    MainVocabulary *mainVocab;
    CurrentLearning *currentLearning;
    std::pair<QString,QString> current;

    QPushButton *pushButtonWord;
    QPushButton *pushButtonDecline;
    QPushButton *pushButtonAccept;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

    void setupUi();
};

#endif // LEARNUI_H
