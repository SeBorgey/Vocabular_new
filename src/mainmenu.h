#ifndef MAINMENU_H
#define MAINMENU_H

#include <QGroupBox>
#include "videoplayer.h"
#include "learnui.h"
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
    VideoPlayer *mainwindow;
    LearnUI *learnui;
    Ui::MainMenu *ui;
};

#endif // MAINMENU_H
