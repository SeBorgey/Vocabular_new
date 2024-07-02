#include "hovereventfilter.h"
#include <QEvent>
#include <QPushButton>

HoverEventFilter::HoverEventFilter(QObject *parent)
    : QObject(parent)
{
}

bool HoverEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == subtitleButton) {
        if (event->type() == QEvent::HoverEnter) {
            emit hoverEnteredSubtitleButton();
        } else if (event->type() == QEvent::HoverLeave) {
            emit hoverLeftSubtitleButton();
        }
    }
    return QObject::eventFilter(watched, event);
}
