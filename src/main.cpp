#include "mainmenu.h"
#include <QApplication>
// todo:
//        добавить управление с клавиатуры
//        поддержку формата mkv
//        разоворот на весь экран
//        выделение целых слов
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu w;
    w.show();
    return a.exec();
}
