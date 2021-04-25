#ifndef DICTIONARYSTATE_HPP
#define DICTIONARYSTATE_HPP

#include "DatabaseManager.hpp"

#include <QWidget>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QTableView>
#include <QVBoxLayout>

class DictionaryState : public QWidget
{
    // Q_OBJECT

    public:
                            DictionaryState(DatabaseManager* DBManager);

    
    private:
        DatabaseManager*    mDBManager;

        QSqlTableModel*     mDictTableModel;
        QTableView*         mDictTableView;
};

#endif