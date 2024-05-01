#ifndef MULTISELECTLABEL_H
#define MULTISELECTLABEL_H

#include <QLabel>
#include <QWidget>

class QMouseEvent;
class QGraphicsDropShadowEffect;

class MultiSelectLabel : public QLabel {
public:
    MultiSelectLabel(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    int cursorPositionAt(const QPoint& pos);
    void updateSelection();

    int offsetX;
    int selection_start;
    int selection_end;
};

#endif // MULTISELECTLABEL_H
