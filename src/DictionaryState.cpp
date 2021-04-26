#include "DictionaryState.hpp"

#include <QHeaderView>
#include <QKeySequence>

#include "DatabaseManager.hpp"
#include "MainWindow.hpp"

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
    mReturnButton = new QPushButton("Return");

    setupDictTable();

    mMainVBoxLayout->addWidget(mSearchLineEdit);
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
    mDictTableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Status"));

    // mDictTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    mDictTableView = new QTableView();
    mDictSortFitler = new QSortFilterProxyModel;

    mDictSortFitler->setSourceModel(mDictTableModel);
    mDictTableView->setModel(mDictSortFitler);
    mDictTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mDictTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QHeaderView* header = mDictTableView->horizontalHeader();
    header->setStretchLastSection(true);
}

void DictionaryState::setupConnections()
{
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &DictionaryState::searchWord);
    connect(mReturnButton, &QPushButton::clicked, getContext()->window, &MainWindow::noActiveStates);
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