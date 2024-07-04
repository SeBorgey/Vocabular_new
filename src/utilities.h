#ifndef UTILITIES_H
#define UTILITIES_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QMainWindow>
#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QDebug>
#include <QApplication>
#include <QTextEdit>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include <QtMultimedia/QtMultimedia>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QtMultimediaWidgets>
#include <QMediaPlayer>
#include <QRegularExpression>
namespace utilities {
void debugWidgetInfo(QWidget* widget, const QString& widgetName) {
    qDebug() << "--- Debug information for" << widgetName << "---";
    qDebug() << "Address:" << widget;
    qDebug() << "Class name:" << widget->metaObject()->className();
    qDebug() << "Object name:" << widget->objectName();
    qDebug() << "Is visible:" << widget->isVisible();
    qDebug() << "Is enabled:" << widget->isEnabled();
    qDebug() << "Is window:" << widget->isWindow();
    qDebug() << "Window flags:" << widget->windowFlags();
    qDebug() << "Geometry:" << widget->geometry();
    qDebug() << "Global geometry:" << QRect(widget->mapToGlobal(QPoint(0,0)), widget->size());
    qDebug() << "Size:" << widget->size();
    qDebug() << "Minimum size:" << widget->minimumSize();
    qDebug() << "Maximum size:" << widget->maximumSize();
    qDebug() << "Base size:" << widget->baseSize();
    qDebug() << "Size policy:" << widget->sizePolicy().horizontalPolicy() << widget->sizePolicy().verticalPolicy();
    qDebug() << "Font:" << widget->font().toString();
    qDebug() << "Palette:" << widget->palette();
    qDebug() << "Style sheet:" << widget->styleSheet();
    qDebug() << "Focus policy:" << widget->focusPolicy();
    qDebug() << "Focus:" << widget->hasFocus();
    qDebug() << "Mouse tracking:" << widget->hasMouseTracking();
    qDebug() << "Parent widget:" << widget->parentWidget();
    qDebug() << "Layout:" << widget->layout();
    qDebug() << "Mask:" << widget->mask();
    qDebug() << "Window opacity:" << widget->windowOpacity();
    qDebug() << "Window modality:" << widget->windowModality();
    qDebug() << "Window state:" << widget->windowState();
    qDebug() << "Attribute Qt::WA_TranslucentBackground:" << widget->testAttribute(Qt::WA_TranslucentBackground);
    qDebug() << "Attribute Qt::WA_NoSystemBackground:" << widget->testAttribute(Qt::WA_NoSystemBackground);
    qDebug() << "Is ancestor of application's active window:" << widget->isAncestorOf(QApplication::activeWindow());

    if (auto textEdit = qobject_cast<QTextEdit*>(widget)) {
        qDebug() << "Text:" << textEdit->toPlainText();
        qDebug() << "HTML:" << textEdit->toHtml();
        qDebug() << "Read only:" << textEdit->isReadOnly();
        qDebug() << "Cursor position:" << textEdit->textCursor().position();
        qDebug() << "Selection start:" << textEdit->textCursor().selectionStart();
        qDebug() << "Selection end:" << textEdit->textCursor().selectionEnd();
        qDebug() << "Text interaction flags:" << textEdit->textInteractionFlags();
    }

    qDebug() << "Children:";
    for (auto child : widget->children()) {
        if (auto childWidget = qobject_cast<QWidget*>(child)) {
            qDebug() << "  " << childWidget->metaObject()->className() << childWidget->objectName();
        }
    }
    qDebug() << "--- End of debug information ---";
}
void debugWidgetHierarchy(QWidget* widget, int depth, QSet<QWidget*>& processedWidgets) {
    if (!widget || processedWidgets.contains(widget)) {
        return;
    }
    processedWidgets.insert(widget);

    QString indent(depth * 2, ' ');
    qDebug().noquote() << indent << "Widget:" << widget->metaObject()->className() << widget->objectName();
    qDebug().noquote() << indent << "  Address:" << widget;
    qDebug().noquote() << indent << "  Visible:" << widget->isVisible();
    qDebug().noquote() << indent << "  Enabled:" << widget->isEnabled();
    qDebug().noquote() << indent << "  Geometry:" << widget->geometry();
    qDebug().noquote() << indent << "  Global Geometry:" << QRect(widget->mapToGlobal(QPoint(0,0)), widget->size());
    qDebug().noquote() << indent << "  Focus:" << widget->hasFocus();
    qDebug().noquote() << indent << "  Focus Policy:" << widget->focusPolicy();
    qDebug().noquote() << indent << "  Window Flags:" << widget->windowFlags();
    qDebug().noquote() << indent << "  Style Sheet:" << widget->styleSheet();

    if (auto textEdit = qobject_cast<QTextEdit*>(widget)) {
        qDebug().noquote() << indent << "  Text Length:" << textEdit->toPlainText().length();
        qDebug().noquote() << indent << "  Read Only:" << textEdit->isReadOnly();
        qDebug().noquote() << indent << "  Cursor Position:" << textEdit->textCursor().position();
        qDebug().noquote() << indent << "  Selection Start:" << textEdit->textCursor().selectionStart();
        qDebug().noquote() << indent << "  Selection End:" << textEdit->textCursor().selectionEnd();
        qDebug().noquote() << indent << "  Text Interaction Flags:" << textEdit->textInteractionFlags();
    }

    // Layout information
    if (QLayout* layout = widget->layout()) {
        qDebug().noquote() << indent << "  Layout:" << layout->metaObject()->className();
        qDebug().noquote() << indent << "    Contents Margins:" << layout->contentsMargins();
        qDebug().noquote() << indent << "    Spacing:" << layout->spacing();
    } else {
        qDebug().noquote() << indent << "  No Layout";
    }

    // Parent information
    if (QWidget* parent = widget->parentWidget()) {
        qDebug().noquote() << indent << "  Parent:" << parent->metaObject()->className() << parent->objectName();
        debugWidgetHierarchy(parent, depth + 1, processedWidgets);
    } else {
        qDebug().noquote() << indent << "  No Parent";
    }

    // Children information
    QList<QWidget*> children = widget->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
    if (!children.isEmpty()) {
        qDebug().noquote() << indent << "  Children:";
        for (QWidget* child : children) {
            debugWidgetHierarchy(child, depth + 1, processedWidgets);
        }
    } else {
        qDebug().noquote() << indent << "  No Children";
    }

    // Siblings information (widgets at the same level)
    if (QWidget* parent = widget->parentWidget()) {
        QList<QWidget*> siblings = parent->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        if (siblings.size() > 1) {
            qDebug().noquote() << indent << "  Siblings:";
            for (QWidget* sibling : siblings) {
                if (sibling != widget) {
                    qDebug().noquote() << indent << "    " << sibling->metaObject()->className() << sibling->objectName();
                }
            }
        } else {
            qDebug().noquote() << indent << "  No Siblings";
        }
    }

    // Z-order information
    if (QWidget* parent = widget->parentWidget()) {
        QList<QWidget*> siblings = parent->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
        int index = siblings.indexOf(widget);
        if (index > 0) {
            qDebug().noquote() << indent << "  Widget below:" << siblings[index - 1]->metaObject()->className() << siblings[index - 1]->objectName();
        }
        if (index < siblings.size() - 1) {
            qDebug().noquote() << indent << "  Widget above:" << siblings[index + 1]->metaObject()->className() << siblings[index + 1]->objectName();
        }
    }
}

// Функция-обертка для запуска анализа
void analyzeWidget(QWidget* widget) {
    QSet<QWidget*> processedWidgets;
    debugWidgetHierarchy(widget, 0, processedWidgets);
}

}
#endif // UTILITIES_H
