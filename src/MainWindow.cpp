#include "MainWindow.hpp"
#include "WelcomeTrain.hpp"
#include "DictionaryState.hpp"
#include "WordsRainTrain.hpp"
#include "WordVariantsTrain.hpp"
#include "MeaningVariantsTrain.hpp"
#include "LetterToWordTrain.hpp"

#include <QShortcut>
#include <QKeySequence>
#include <QFile>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, mTrainStateIndx(0)
, mCurrentTrainType(TrainingType::None)
{
    qDebug() << QDir::currentPath();
    QFile file("../style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);
    setWindowTitle("Exercise");
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    setupCoreWidgets();
    setupCoreWidgetsConnections();

}

void MainWindow::setupCoreWidgets()
{
    mMainWidgetStack = new QStackedWidget;

    mMainWidget = new QWidget;
    mMainVBoxLayout = new QVBoxLayout;

    int wordsForTrain = mDBManager.countWordsForTraining(TrainingType::WordIntro);
    mWordIntroTrainButton = new QPushButton(QString("WordIntro (") + QString::number(wordsForTrain) + QString(")"));
    mWordIntroTrainButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);

    wordsForTrain = mDBManager.countWordsForTraining(TrainingType::WordVariants);
    mWordVariantsTrainButton = new QPushButton(QString("WordVariants (") + QString::number(wordsForTrain) + QString(")"));
    mWordVariantsTrainButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);

    wordsForTrain = mDBManager.countWordsForTraining(TrainingType::MeaningVariants);
    mMeaningVariantsTrainButton = new QPushButton(QString("MeaningVariants (") + QString::number(wordsForTrain) + QString(")"));
    mMeaningVariantsTrainButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);

    wordsForTrain = mDBManager.countWordsForTraining(TrainingType::LetterToWord);
    mLetterToWordTrainButton = new QPushButton(QString("LetterToWordTrain (") + QString::number(wordsForTrain) + QString(")"));
    mLetterToWordTrainButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);

    wordsForTrain = mDBManager.countWordsForTraining(TrainingType::WordRain);
    mWordsRainTrainButton = new QPushButton(QString("WordsRain (") + QString::number(wordsForTrain) + QString(")"));
    mWordsRainTrainButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);

    int fullyLearnedWords = mDBManager.countFullyLearnedWords();
    int totalWords = mDBManager.countWords();
    int unlearnedWords = totalWords - fullyLearnedWords;
    mDictionaryStateButton = new QPushButton(QString("Dictionary (") + QString::number(unlearnedWords) + QString("|") + QString::number(totalWords) + QString(")"));
    mDictionaryStateButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);

    mMainVBoxLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mWordIntroTrainButton);
    mMainVBoxLayout->addWidget(mWordVariantsTrainButton);
    mMainVBoxLayout->addWidget(mMeaningVariantsTrainButton);
    mMainVBoxLayout->addWidget(mLetterToWordTrainButton);
    mMainVBoxLayout->addWidget(mWordsRainTrainButton);
    mMainVBoxLayout->addWidget(mDictionaryStateButton);
    mMainWidget->setLayout(mMainVBoxLayout);

    setCentralWidget(mMainWidget);
}

void MainWindow::setupCoreWidgetsConnections()
{
    connect(mWordIntroTrainButton, &QPushButton::clicked, this, &MainWindow::initWordIntroTraining);
    connect(mWordVariantsTrainButton, &QPushButton::clicked, this, &MainWindow::initWordVariantsTraining);
    connect(mMeaningVariantsTrainButton, &QPushButton::clicked, this, &MainWindow::initMeaningVariantsTraining);
    connect(mLetterToWordTrainButton, &QPushButton::clicked, this, &MainWindow::initLetterToWordTraining);
    connect(mWordsRainTrainButton, &QPushButton::clicked, this, &MainWindow::initWordsRainTraining);
    connect(mDictionaryStateButton, &QPushButton::clicked, this, &MainWindow::initDictionaryState);
}

void MainWindow::trainStateDone(bool status)
{
    mTrainStates[mTrainStateIndx].second = status;
    bool isAllDone = nextTrainState();
    if (isAllDone)
    {
        // allTrainsDone();
        updateDatabase(status);
        setupCoreWidgets();
        setupCoreWidgetsConnections();
    }
}

#include <iostream>
void MainWindow::updateDatabase(bool status)
{
    if (status == false) return;

    std::cerr << "update database:\n";
    for (auto& lWord : mLWords)
    {
        std::cerr << lWord.word << " " << lWord.attempts << std::endl;
    }
    mDBManager.setLearnWordsStatus(mLWords, mCurrentTrainType);
}

void MainWindow::allTrainsDone()
{
    // update database;
    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void MainWindow::noActiveStates()
{
    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void MainWindow::initWordIntroTraining()
{
    mCurrentTrainType = TrainingType::WordIntro;

    mLWords = mDBManager.generateWordsForTraining(mCurrentTrainType);

    const int size = 3;
    mTrainStates.clear();
    mTrainStates.resize(size);
    for (int i = 0; i < size; ++i)
    {
        mTrainStates[i].second = false;
    }

    mTrainStates[0].first = new WelcomeTrain(mLWords, State::Context(this, &mDBManager));
    mTrainStates[1].first = new MeaningVariantsTrain(mLWords, State::Context(this, &mDBManager));
    mTrainStates[2].first = new LetterToWordTrain(mLWords, State::Context(this, &mDBManager));
    
    // setCentralWidget(mTrainStates[0].first);
    for (int i = 0; i < size; ++i)
    {
        mMainWidgetStack->addWidget(mTrainStates[i].first);
    }

    setCentralWidget(mMainWidgetStack);
    // connect(mBrainTrain, SIGNAL(endStateSignal(bool)), this, SLOT(learnStateDone(bool)));
}

void MainWindow::initWordVariantsTraining()
{
    mCurrentTrainType = TrainingType::WordVariants;

    mLWords = mDBManager.generateWordsForTraining(mCurrentTrainType);

    mTrainStates.clear();
    mTrainStates.resize(1);
    mTrainStates[0].first = new WordVariantsTrain(mLWords, State::Context(this, &mDBManager));
    mTrainStates[0].second = false;

    mMainWidgetStack->addWidget(mTrainStates[0].first);
    setCentralWidget(mMainWidgetStack);
}

void MainWindow::initMeaningVariantsTraining()
{
    mCurrentTrainType = TrainingType::MeaningVariants;

    mLWords = mDBManager.generateWordsForTraining(mCurrentTrainType);

    mTrainStates.clear();
    mTrainStates.resize(1);
    mTrainStates[0].first = new MeaningVariantsTrain(mLWords, State::Context(this, &mDBManager));
    mTrainStates[0].second = false;

    mMainWidgetStack->addWidget(mTrainStates[0].first);
    setCentralWidget(mMainWidgetStack);
}

void MainWindow::initLetterToWordTraining()
{
    mCurrentTrainType = TrainingType::LetterToWord;

    mLWords = mDBManager.generateWordsForTraining(mCurrentTrainType);

    mTrainStates.clear();
    mTrainStates.resize(1);
    mTrainStates[0].first = new LetterToWordTrain(mLWords, State::Context(this, &mDBManager));
    mTrainStates[0].second = false;

    mMainWidgetStack->addWidget(mTrainStates[0].first);
    setCentralWidget(mMainWidgetStack);
}

void MainWindow::initWordsRainTraining()
{
    mCurrentTrainType = TrainingType::WordRain;

    mLWords = mDBManager.generateWordsForTraining(mCurrentTrainType);

    mTrainStates.clear();
    mTrainStates.resize(1);
    mTrainStates[0].first = new WordsRainTrain(mLWords, State::Context(this, &mDBManager));
    mTrainStates[0].second = false;


    mMainWidgetStack->addWidget(mTrainStates[0].first);


    setCentralWidget(mMainWidgetStack);
}

#include <QDebug>
void MainWindow::initDictionaryState()
{
    qDebug() << "init dictionary!";
    DictionaryState* dictState = new DictionaryState(State::Context(this, &mDBManager));
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
    mMainWidgetStack->setCurrentIndex(mTrainStateIndx);
    // setCentralWidget(mTrainStates[mTrainStateIndx].first);

    return isAllDone;
}