#ifndef DICTIONARYSTATE_HPP
#define DICTIONARYSTATE_HPP

#include "DatabaseManager.hpp"
#include "State.hpp"

#include <QWidget>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QShortcut>
#include <QPushButton>

class DictionaryState : public State
{
    // Q_OBJECT

    public:
                            DictionaryState(State::Context context);

    private slots:
        void                searchWord();
        void                addWord();
        void                unlearnWord();
        void                deleteWord();

    private:
        void                setupCoreWidgets();
        void                setupDictTable();
        void                setupConnections();
        void                setupButtons();
    
    private:
        QVBoxLayout*                mMainVBoxLayout;
        QLineEdit*                  mSearchLineEdit;
        QSqlTableModel*             mDictTableModel;
        QTableView*                 mDictTableView;
        QSortFilterProxyModel*      mDictSortFitler; 

        QPushButton*                mWordAddbutton;  
        QPushButton*                mWordResetButton;
        QPushButton*                mWordDeleteButton;
        QPushButton*                mReturnButton;
};

#endif