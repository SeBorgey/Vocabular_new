#include "multiselecttextedit.h"
#include <QMouseEvent>
#include <QTextCursor>
#include <QGraphicsDropShadowEffect>

MultiSelectTextEdit::MultiSelectTextEdit(QWidget* parent) : QTextEdit(parent) {
    setMouseTracking(true);
    selectionStart = 0;
    selectionEnd = 0;
    setReadOnly(true);
    auto shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::black);
    setGraphicsEffect(shadow);
    setStyleSheet("color: white; background-color: transparent;");
}
void MultiSelectTextEdit::simulateDoubleClick(const QPoint& pos) {
    QMouseEvent pressEvent(QEvent::MouseButtonPress, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QMouseEvent dblClickEvent(QEvent::MouseButtonDblClick, pos, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

    QTextEdit::mousePressEvent(&pressEvent);
    QTextEdit::mouseReleaseEvent(&releaseEvent);
    QTextEdit::mouseDoubleClickEvent(&dblClickEvent);
}
int MultiSelectTextEdit::findWordBoundary(int position, bool start) {
    QTextCursor cursor(document());
    cursor.setPosition(position);
    if (start) {
        cursor.movePosition(QTextCursor::StartOfWord);
    } else {
        cursor.movePosition(QTextCursor::EndOfWord);
    }
    return cursor.position();
}
void MultiSelectTextEdit::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        simulateDoubleClick(event->pos());
        QTextCursor cursor = textCursor();
        selectionStart = cursor.selectionStart();
        selectionEnd = cursor.selectionEnd();
    } else {
        QTextEdit::mousePressEvent(event);
    }
}

void MultiSelectTextEdit::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        QTextCursor cursor = cursorForPosition(event->pos());
        int position = cursor.position();
        if (position < selectionStart) {
            cursor.setPosition(position);
            cursor.setPosition(selectionStart, QTextCursor::KeepAnchor);
        } else {
            cursor.setPosition(selectionStart);
            cursor.setPosition(position, QTextCursor::KeepAnchor);
        }
        setTextCursor(cursor);
        selectionEnd = cursor.position();
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
