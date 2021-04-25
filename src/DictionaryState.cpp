#include "DictionaryState.hpp"

#include <QHeaderView>

DictionaryState::DictionaryState(DatabaseManager* DBManager)
: mDBManager(DBManager)
{
    mDictTableModel = new QSqlTableModel(0, DBManager->getDBConnection());
    mDictTableModel->setTable(QString::fromStdString(mDBManager->getTableName()));
    mDictTableModel->select();
    mDictTableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    mDictTableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Word"));
    mDictTableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Meaning"));
    mDictTableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Status"));

    // mDictTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    mDictTableView = new QTableView();
    mDictTableView->setModel(mDictTableModel);
    mDictTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mDictTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QHeaderView* header = mDictTableView->horizontalHeader();
    header->setStretchLastSection(true);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(mDictTableView);

    setLayout(layout);
}