#ifndef WORDEDITOR_H
#define WORDEDITOR_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "mainvocabulary.h"

class WordEditor : public QWidget
{
    Q_OBJECT

public:
    explicit WordEditor(MainVocabulary *vocab, QWidget *parent = nullptr);

private:
    MainVocabulary *vocabulary;
    QTableView *tableView;
    QStandardItemModel *model;
    QSortFilterProxyModel *proxyModel;

    void setupUi();
    void populateTable();

private slots:
    void onCellChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void onHeaderClicked(int logicalIndex);
private slots:
    void onAddWord();
    void onDeleteWord();
    void updateColumnWidths();
};

#endif // WORDEDITOR_H
