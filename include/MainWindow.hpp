#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "DatabaseManager.hpp"
#include "TrainState.hpp"
#include "DictionaryState.hpp"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>

#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    public:
        MainWindow(QWidget* parent = 0);

    public slots:
        void        trainStateDone(bool status);
        void        noActiveStates();

    private:
        void        setupCoreWidgets();
        void        setupCoreWidgetsConnections();

        void        initBrainstormTraining();
        void        initDictionaryState();

        bool        nextTrainState();
        void        allTrainsDone();

    private:
        DatabaseManager             mDBManager;

        QWidget*                    mMainWidget;
        QVBoxLayout*                mMainVBoxLayout;
        QPushButton*                mBranstormTrainButton;
        QPushButton*                mDictionaryStateButton;
        DictionaryState* dictState;

        std::vector<std::pair<TrainState*, bool>>       mTrainStates;
        size_t                                          mTrainStateIndx;
};

#endif