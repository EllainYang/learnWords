#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "DatabaseManager.hpp"
#include "TrainState.hpp"
#include "DictionaryState.hpp"
#include "WordsRainTrain.hpp"
#include "WordVariantsTrain.hpp"
#include "MeaningVariantsTrain.hpp"
#include "LetterToWordTrain.hpp"
#include "Utility.hpp"

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

        WordVariantsTrain*          wordVariantsTrain;
        MeaningVariantsTrain*       meaningVariantsTrain;
        DictionaryState*            dictState;
        WordsRainTrain*             wordsRainTrain;
        LetterToWordTrain*          letterToWordTrain;

        std::vector<std::pair<TrainState*, bool>>       mTrainStates;
        size_t                                          mTrainStateIndx;

        TrainingType                mTrainType;
};

#endif