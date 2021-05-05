#include "DictionaryState.hpp"
#include "DatabaseManager.hpp"
#include "MainWindow.hpp"
#include "Utility.hpp"
#include "TableButton.hpp"

#include <QHeaderView>
#include <QKeySequence>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QDir>
#include <QDebug>
#include <QSignalMapper>

DictionaryState::DictionaryState(State::Context context)
: State(context)
{
    setupCoreWidgets();
    setupButtons();
    setupConnections();
}

void DictionaryState::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mSearchLineEdit = new QLineEdit;
    mWordAddbutton = new QPushButton("ADD");
    mWordAddbutton->setShortcut(QKeySequence(Qt::Key_Return));
    mWordAddbutton->setFixedSize(80, 30);
    mReturnButton = new QPushButton("Return");
    mReturnButton->setShortcut(QKeySequence(Qt::Key_Escape));

    setupDictTable();

    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(mSearchLineEdit);
    horizontalLayout->addWidget(mWordAddbutton);

    mMainVBoxLayout->addLayout(horizontalLayout);
    mMainVBoxLayout->addWidget(mDictTableView);
    mMainVBoxLayout->addWidget(mReturnButton);


    setLayout(mMainVBoxLayout);
}

void DictionaryState::setupDictTable()
{
    DatabaseManager* DBManager = getContext()->DBManager;
    mDictTableModel = new QSqlTableModel(0, DBManager->getDBConnection());
    mDictTableModel->setTable(QString::fromStdString(DBManager->getTableName()));
    mDictTableModel->select();
    mDictTableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    mDictTableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Word"));
    mDictTableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Meaning"));
    mDictTableModel->setHeaderData(3, Qt::Horizontal, QObject::tr(GetTrainTypeStatusName[WordIntro].c_str()));
    mDictTableModel->setHeaderData(4, Qt::Horizontal, QObject::tr(GetTrainTypeStatusName[WordVariants].c_str()));
    mDictTableModel->setHeaderData(5, Qt::Horizontal, QObject::tr(GetTrainTypeStatusName[MeaningVariants].c_str()));
    mDictTableModel->setHeaderData(6, Qt::Horizontal, QObject::tr(GetTrainTypeStatusName[LetterToWord].c_str()));
    mDictTableModel->setHeaderData(7, Qt::Horizontal, QObject::tr(GetTrainTypeStatusName[WordRain].c_str()));
    // mDictTableModel->

    mDictTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    mDictTableView = new QTableView();
    mDictSortFitler = new QSortFilterProxyModel;

    mDictSortFitler->setSourceModel(mDictTableModel);
    mDictTableView->setModel(mDictSortFitler);
    mDictTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mDictTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // mDictTableModel->insertCOlumn

    // mDictSortFitler->setDynamicSortFilter(true);

    QHeaderView* header = mDictTableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

void DictionaryState::setupButtons()
{
    mDictTableModel->insertColumn(8);
    mDictTableModel->insertColumn(9);
    mDictTableModel->setHeaderData(8, Qt::Horizontal, QObject::tr(""));
    mDictTableModel->setHeaderData(9, Qt::Horizontal, QObject::tr(""));

    DatabaseManager* dbManager = getContext()->DBManager;
    QSqlQuery statement(QString("select * from ") + QString::fromStdString(dbManager->getTableName()), dbManager->getDBConnection());
    QSqlRecord record = statement.record();

    int i = 0;
    while (statement.next())
    {
        TableButton* resetButton = new TableButton(i, "O");
        TableButton* deleteButton = new TableButton(i ,"X");
        mDictTableView->setIndexWidget(mDictSortFitler->index(i, 8), resetButton);
        mDictTableView->setIndexWidget(mDictSortFitler->index(i, 9), deleteButton);
        connect(resetButton, &QPushButton::clicked, this, &DictionaryState::unlearnWord);
        connect(deleteButton, &QPushButton::clicked, this, &DictionaryState::deleteWord);
        
        ++i;
    }
}

void DictionaryState::setupConnections()
{
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &DictionaryState::searchWord);
    connect(mReturnButton, &QPushButton::clicked, getContext()->window, &MainWindow::noActiveStates);
    connect(mWordAddbutton, &QPushButton::clicked, this, &DictionaryState::addWord);
}

// #include <QDebug>
void DictionaryState::unlearnWord()
{
    TableButton* senderButton = qobject_cast<TableButton*>(sender());
    size_t row = senderButton->getRow();
    
    mDictTableModel->setData(mDictTableModel->index(row, 3), 0);
    mDictTableModel->setData(mDictTableModel->index(row, 4), 0);
    mDictTableModel->setData(mDictTableModel->index(row, 5), 0);
    mDictTableModel->setData(mDictTableModel->index(row, 6), 0);
    mDictTableModel->setData(mDictTableModel->index(row, 7), 0);

    mDictTableModel->submitAll();
    setupButtons();
}


void DictionaryState::deleteWord()
{
    TableButton* senderButton = qobject_cast<TableButton*>(sender());
    size_t row = senderButton->getRow();

    mDictSortFitler->removeRow(row);
    mDictTableModel->submitAll();
    setupButtons();
}

void DictionaryState::addWord()
{
    QString word = mSearchLineEdit->text();
    if (word.size() == 0) return;

    bool ok;
    QString meaning = QInputDialog::getText(this, tr("Add meaning"),
            tr("Word meaning: "), QLineEdit::Normal, "", &ok);

    if (ok && !meaning.isEmpty())
    {
        int row = mDictTableModel->rowCount();
        mDictTableModel->insertRow(row);
        mDictTableModel->setData(mDictTableModel->index(row, 0), 0);
        mDictTableModel->setData(mDictTableModel->index(row, 1), word);
        mDictTableModel->setData(mDictTableModel->index(row, 2), meaning);
        mDictTableModel->setData(mDictTableModel->index(row, 3), 0);
        mDictTableModel->setData(mDictTableModel->index(row, 4), 0);
        mDictTableModel->setData(mDictTableModel->index(row, 5), 0);
        mDictTableModel->setData(mDictTableModel->index(row, 6), 0);
        mDictTableModel->setData(mDictTableModel->index(row, 7), 0);
        mDictTableModel->submitAll();

        mSearchLineEdit->setText("");
        setupButtons();
    }
}

void DictionaryState::searchWord()
{
    QString word = mSearchLineEdit->text();
    // if (word.size() == 0) return;

    QString filter = "^" + word + ".*$";
    // mDictTableModel->setFilter(filter);
    mDictSortFitler->setFilterKeyColumn(1);
    mDictSortFitler->setFilterRegExp(filter);
}