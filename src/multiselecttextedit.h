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
    int selection_start;
    int selection_end;

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

};

#endif // MULTISELECTTEXTEDIT_H
