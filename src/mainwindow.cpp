#include "mainwindow.h"
#include "main_vocabulary.h"
#include "current_learning.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    Main_vocabulary ob;
    Current_learning c(ob);
    c.choose_words_for_learning();
}

MainWindow::~MainWindow()
{
}

