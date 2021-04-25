#include "MainWindow.hpp"
#include "LetterToWordTrain.hpp"
#include "WelcomeTrain.hpp"
#include "VariantsTrain.hpp"

#include <QShortcut>
#include <QKeySequence>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, mTrainStateIndx(0)
{
    setWindowTitle("Exercise");
    // setFixedSize(640, 480);

    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void MainWindow::setupCoreWidgets()
{
    mMainWidget = new QWidget;
    mMainVBoxLayout = new QVBoxLayout;

    int brainstormWords = mDBManager.countWordsForTraining(TrainingType::Brainstorm);
    mBranstormTrainButton = new QPushButton(QString("Brainstorm (") + QString::number(brainstormWords) + QString(")"));

    int fullyLearnedWords = mDBManager.countFullyLearnedWords();
    int totalWords = mDBManager.countWords();
    int unlearnedWords = totalWords - fullyLearnedWords;
    mDictionaryStateButton = new QPushButton(QString("Dictionary (") + QString::number(unlearnedWords) + QString("|") + QString::number(totalWords) + QString(")"));

    mMainVBoxLayout->addWidget(mBranstormTrainButton);
    mMainVBoxLayout->addWidget(mDictionaryStateButton);
    mMainWidget->setLayout(mMainVBoxLayout);

    setCentralWidget(mMainWidget);
    setupCoreWidgetsConnections();
}

void MainWindow::setupCoreWidgetsConnections()
{
    connect(mBranstormTrainButton, &QPushButton::clicked, this, &MainWindow::initBrainstormTraining);
    connect(mDictionaryStateButton, &QPushButton::clicked, this, &MainWindow::initDictionaryState);
}
#include <iostream>
void MainWindow::trainStateDone(bool status)
{
    // mTestStateIndx++;
    // setCentralWidget(mTestStates[mTestStateIndx]);
    // connect(mTestStates[mTestStateIndx], &LearnState::endStateSignal, this, &MainWindow::learnStateDone);    
    // if (status == true)
    // {
    //     // std::cerr << (mMainWidget) << std::endl;
    //     // setupCoreWidgets();
    //     // std::cerr << "invisible potato\n";
        
    // }
    mTrainStates[mTrainStateIndx].second = status;
    bool isAllDone = nextTrainState();
    if (isAllDone)
    {
        allTrainsDone();
    }
}

void MainWindow::allTrainsDone()
{
    // update database;
    setupCoreWidgets();
}

void MainWindow::noActiveStates()
{
    setupCoreWidgets();
}

void MainWindow::initBrainstormTraining()
{
    std::vector<LearnWord> mLWords = mDBManager.generateWordsForBrainStorm();

    const int size = 3;
    mTrainStates.resize(size);
    for (int i = 0; i < size; ++i)
    {
        mTrainStates[i].second = false;
    }

    mTrainStates[0].first = new WelcomeTrain(mLWords, TrainState::Context(this, &mDBManager));
    mTrainStates[1].first = new VariantTrain(mLWords, TrainState::Context(this, &mDBManager));
    mTrainStates[2].first = new LetterToWordTrain(mLWords, TrainState::Context(this, &mDBManager));
    
    setCentralWidget(mTrainStates[0].first);
    // connect(mBrainTrain, SIGNAL(endStateSignal(bool)), this, SLOT(learnStateDone(bool)));
}

void MainWindow::initDictionaryState()
{
    dictState = new DictionaryState(&mDBManager);
    setCentralWidget(dictState);
}

bool MainWindow::nextTrainState()
{
    size_t indx = mTrainStateIndx;
    do
    {
        indx = (indx + 1) % mTrainStates.size();
    } while (mTrainStates[indx].second == true && indx != mTrainStateIndx);

    bool isAllDone = (indx == mTrainStateIndx && mTrainStates[indx].second == true);
    mTrainStateIndx = indx;
    setCentralWidget(mTrainStates[mTrainStateIndx].first);

    return isAllDone;
}