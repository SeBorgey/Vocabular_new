#ifndef MAINMENU_H
#define MAINMENU_H

#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include "learnui.h"
#include "mainvocabulary.h"


#ifndef Q_OS_ANDROID
#include "mainwindow.h"
#include "wordeditor.h"
#endif

class MainMenu : public QGroupBox
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    void on_pushButtonLearn_clicked();
    void on_pushButtonImport_clicked();
    void on_pushButtonExport_clicked();

private:
#ifndef Q_OS_ANDROID
    MainWindow *mainWindow;
    WordEditor *wordEditor;
#endif
    LearnUI *learnui;

    MainVocabulary *main_vocab;

    QPushButton *pushButtonLearn;
    QPushButton *pushButtonImport;
    QPushButton *pushButtonExport;
    QVBoxLayout *mainLayout;

    void setupUi();

#ifndef Q_OS_ANDROID
    void centerWindow();
    QPushButton *pushButtonWatch;
    QPushButton *pushButtonEdit;
private slots:
    void on_pushButtonWatch_clicked();
    void on_pushButtonEdit_clicked();
#endif
};

#endif // MAINMENU_H
