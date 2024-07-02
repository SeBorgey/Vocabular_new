#ifndef HOVEREVENTFILTER_H
#define HOVEREVENTFILTER_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
class HoverEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit HoverEventFilter(QObject *parent = nullptr);
    QPushButton *subtitleButton;
signals:
    void hoverEnteredSubtitleButton();
    void hoverLeftSubtitleButton();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:

};

#endif // HOVEREVENTFILTER_H
