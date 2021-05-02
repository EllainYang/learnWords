#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "DatabaseManager.hpp"
#include "Utility.hpp"
#include "TrainState.hpp"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QStackedWidget>

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

        void        initWordIntroTraining();
        void        initWordVariantsTraining();
        void        initMeaningVariantsTraining();
        void        initLetterToWordTraining();
        void        initWordsRainTraining();
        void        initDictionaryState();

        void        updateDatabase(bool status);

        bool        nextTrainState();
        void        allTrainsDone();

    private:

        DatabaseManager             mDBManager;

        QWidget*                    mMainWidget;
        QVBoxLayout*                mMainVBoxLayout;
        QPushButton*                mWordIntroTrainButton;
        QPushButton*                mWordVariantsTrainButton;
        QPushButton*                mMeaningVariantsTrainButton;
        QPushButton*                mLetterToWordTrainButton;
        QPushButton*                mDictionaryStateButton;
        QPushButton*                mWordsRainTrainButton;


        TrainingType                mCurrentTrainType;
        std::vector<LearnWord>      mLWords;

        QStackedWidget*             mMainWidgetStack;

        std::vector<std::pair<TrainState*, bool>>       mTrainStates;
        size_t                                          mTrainStateIndx;
};

#endif