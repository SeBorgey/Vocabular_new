#ifndef MAINMENU_H
#define MAINMENU_H

#include <QGroupBox>
#include "mainwindow.h"
#include "learnui.h"
#include "main_vocabulary.h"
namespace Ui {
class MainMenu;
}

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
    Ui::MainMenu *ui;
    Main_vocabulary *main_vocab;
};

#endif // MAINMENU_H
