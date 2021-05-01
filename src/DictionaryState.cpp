#include "DictionaryState.hpp"
#include "DatabaseManager.hpp"
#include "MainWindow.hpp"
#include "Utility.hpp"

#include <QHeaderView>
#include <QKeySequence>

#include <QHBoxLayout>
#include <QInputDialog>
#include <QDir>
#include <QDebug>

DictionaryState::DictionaryState(State::Context context)
: State(context)
{
    setupCoreWidgets();
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


    mDictTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    mDictTableView = new QTableView();
    mDictSortFitler = new QSortFilterProxyModel;

    mDictSortFitler->setSourceModel(mDictTableModel);
    mDictTableView->setModel(mDictSortFitler);
    mDictTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mDictTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QHeaderView* header = mDictTableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

void DictionaryState::setupConnections()
{
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &DictionaryState::searchWord);
    connect(mReturnButton, &QPushButton::clicked, getContext()->window, &MainWindow::noActiveStates);
    connect(mWordAddbutton, &QPushButton::clicked, this, &DictionaryState::addWord);
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
        qDebug() << mDictTableModel->submitAll();
        mDictTableModel->select();

        mSearchLineEdit->setText("");
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