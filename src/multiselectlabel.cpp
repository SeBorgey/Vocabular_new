#include "multiselectlabel.h"
#include <QMouseEvent>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>

MultiSelectLabel::MultiSelectLabel(QWidget* parent) : QLabel(parent) {
    setMouseTracking(true);
    selection_start = 0;
    selection_end = 0;
    setTextFormat(Qt::RichText);
    auto shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::black);
    setGraphicsEffect(shadow);
    setStyleSheet("font-size: 20pt; color: white;");
}

void MultiSelectLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        selection_start = cursorPositionAt(event->pos());
        selection_end = selection_start;
    }
    QLabel::mousePressEvent(event);
}

void MultiSelectLabel::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        selection_end = cursorPositionAt(event->pos());
        updateSelection();
    }
    QLabel::mouseMoveEvent(event);
}

int MultiSelectLabel::cursorPositionAt(const QPoint& pos) {
    QFontMetrics fm(font());
    QString text = this->text();
    int labelWidth = width();
    int textWidth = fm.horizontalAdvance(text);
    offsetX = fm.leading();
    int cursorPos = 0;
    int textPos = offsetX;

    for (int i = 0; i < text.length(); ++i) {
        int charWidth = fm.horizontalAdvance(text[i]);
        if (pos.x() >= textPos && pos.x() < textPos + charWidth) {
            cursorPos = i;
            break;
        }
        textPos += charWidth;
    }
    return cursorPos;
}

void MultiSelectLabel::updateSelection() {
    update();
    setSelection(std::min(selection_start, selection_end), std::abs(selection_end - selection_start));
}
