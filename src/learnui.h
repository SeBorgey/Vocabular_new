#ifndef LEARNUI_H
#define LEARNUI_H

#include <QGroupBox>

namespace Ui {
class LearnUI;
}

class LearnUI : public QGroupBox
{
    Q_OBJECT

public:
    explicit LearnUI(QWidget *parent = nullptr);
    ~LearnUI();

private slots:
    void on_pushButtonDecline_clicked();

    void on_pushButtonAccept_clicked();

    void on_pushButtonWord_clicked();

private:
    Ui::LearnUI *ui;
};

#endif // LEARNUI_H
