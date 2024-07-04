#include "customlabel.h"

CustomLabel::CustomLabel(QWidget *parent)
    : QLabel(parent), hovered(false)
{
    setWordWrap(true);
    setAttribute(Qt::WA_Hover, true);
    setAlignment(Qt::AlignCenter);
}

void CustomLabel::enterEvent(QEnterEvent *event)
{
    hovered = true;
    emit hoverEntered();
    QLabel::enterEvent(event);
}

void CustomLabel::leaveEvent(QEvent *event)
{
    hovered = false;
    emit hoverLeft();
    QLabel::leaveEvent(event);
}

void CustomLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}
