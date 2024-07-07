#include "wordeditor.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

WordEditor::WordEditor(MainVocabulary *vocab, QWidget *parent)
    : QWidget(parent), vocabulary(vocab)
{
    setupUi();
    populateTable();
}

void WordEditor::setupUi()
{
    setWindowTitle("Word Editor");
    resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);
    tableView = new QTableView(this);
    layout->addWidget(tableView);

    model = new QStandardItemModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    tableView->setModel(proxyModel);

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->setWordWrap(true);

    connect(model, &QStandardItemModel::dataChanged, this, &WordEditor::onCellChanged);
    connect(tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &WordEditor::onHeaderClicked);

    QPushButton *addButton = new QPushButton("Add Word", this);
    QPushButton *deleteButton = new QPushButton("Delete Word", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);

    layout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &WordEditor::onAddWord);
    connect(deleteButton, &QPushButton::clicked, this, &WordEditor::onDeleteWord);
}

void WordEditor::populateTable()
{
    model->clear();
    model->setHorizontalHeaderLabels({"English", "Russian", "Times", "Last Date"});

    for (const auto& word : vocabulary->words) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(word->english));
        row.append(new QStandardItem(word->russian));
        row.append(new QStandardItem(QString::number(word->learningTimes)));
        row.append(new QStandardItem(word->lastLearning.toString("yyyy.MM.dd HH:mm:ss")));
        model->appendRow(row);
    }
}

void WordEditor::onCellChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    int row = proxyModel->mapToSource(topLeft).row();
    int column = topLeft.column();
    QString newValue = model->data(proxyModel->mapToSource(topLeft)).toString();

    Word* word = vocabulary->words[row];

    QString english = word->english;
    QString russian = word->russian;
    int times = word->learningTimes;
    QDateTime lastLearning = word->lastLearning;

    switch (column) {
    case 0: english = newValue; break;
    case 1: russian = newValue; break;
    case 2: times = newValue.toInt(); break;
    case 3: lastLearning = QDateTime::fromString(newValue, "yyyy.MM.dd HH:mm:ss"); break;
    }

    vocabulary->updateWord(row, english, russian, times, lastLearning);
    populateTable();
}

void WordEditor::onHeaderClicked(int logicalIndex)
{
    static bool ascending = true;
    proxyModel->sort(logicalIndex, ascending ? Qt::AscendingOrder : Qt::DescendingOrder);
    ascending = !ascending;
}

void WordEditor::onAddWord()
{
    vocabulary->addWord("", "");
    populateTable();
}

void WordEditor::onDeleteWord()
{
    QModelIndex currentIndex = tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Please select a word to delete.");
        return;
    }

    int row = proxyModel->mapToSource(currentIndex).row();
    vocabulary->deleteWord(row);
    populateTable();
}
