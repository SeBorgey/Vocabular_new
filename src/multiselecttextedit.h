#ifndef MULTISELECTTEXTEDIT_H
#define MULTISELECTTEXTEDIT_H
#include <QPainter>
#include <QTextEdit>
#include <QTextBlock>
class MultiSelectTextEdit : public QTextEdit {
    Q_OBJECT

public:
    explicit MultiSelectTextEdit(QWidget* parent = nullptr);
    // friend class MainWindow;
    void clearSelection();
    QString getSelectedText() const;
    void setCustomSelection(int start, int end);
    int selectionStart;
    int selectionEnd;

    void updateSelection();
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
public:

    void publicMousePressEvent(QMouseEvent* event){
        mousePressEvent(event);
    }
    void publicMouseMoveEvent(QMouseEvent* event){
        mouseMoveEvent(event);
    }
private:
    int findWordBoundary(int position, bool start);
private:
    void simulateDoubleClick(const QPoint& pos);
};

#endif // MULTISELECTTEXTEDIT_H
