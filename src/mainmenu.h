#ifndef MAINMENU_H
#define MAINMENU_H

#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "learnui.h"
#include "mainvocabulary.h"

class MainMenu : public QGroupBox
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void on_pushButtonWatch_clicked();
    void on_pushButtonLearn_clicked();

private:
    MainWindow *mainWindow;
    LearnUI *learnui;
    MainVocabulary *mainVocab;

    QPushButton *pushButtonWatch;
    QPushButton *pushButtonLearn;
    QVBoxLayout *mainLayout;

    void setupUi();
    void centerWindow();
};

#endif // MAINMENU_H
