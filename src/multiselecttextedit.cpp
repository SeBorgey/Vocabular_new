#include "multiselecttextedit.h"
#include <QMouseEvent>
#include <QTextCursor>
#include <QGraphicsDropShadowEffect>

MultiSelectTextEdit::MultiSelectTextEdit(QWidget* parent) : QTextEdit(parent) {
    setMouseTracking(true);
    selectionStart = 0;
    selectionEnd = 0;
    setReadOnly(true);  // Чтобы предотвратить редактирование
    auto shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::black);
    setGraphicsEffect(shadow);
    setStyleSheet("color: white; background-color: transparent;");
}

void MultiSelectTextEdit::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QTextCursor cursor = cursorForPosition(event->pos());
        selectionStart = cursor.position();
        selectionEnd = selectionStart;
    }
    QTextEdit::mousePressEvent(event);
}

void MultiSelectTextEdit::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        QTextCursor cursor = cursorForPosition(event->pos());
        selectionEnd = cursor.position();
        updateSelection();
    }
    QTextEdit::mouseMoveEvent(event);
}

void MultiSelectTextEdit::updateSelection() {
    QTextCursor cursor = textCursor();
    cursor.setPosition(selectionStart);
    cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}

void MultiSelectTextEdit::clearSelection() {
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
}

QString MultiSelectTextEdit::getSelectedText() const {
    return textCursor().selectedText();
}

void MultiSelectTextEdit::setCustomSelection(int start, int end) {
    QTextCursor cursor = textCursor();
    cursor.setPosition(start);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}
