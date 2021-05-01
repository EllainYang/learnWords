#include "MainWindow.hpp"
#include "WelcomeTrain.hpp"

#include <QShortcut>
#include <QKeySequence>
#include <QFile>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, mTrainStateIndx(0)
, mTrainType(TrainingType::None)
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
    mMainWidget = new QWidget;
    mMainVBoxLayout = new QVBoxLayout;

    int brainstormWords = mDBManager.countWordsForTraining(TrainingType::WordIntro);
    mWordIntroTrainButton = new QPushButton(QString("WordIntro (") + QString::number(brainstormWords) + QString(")"));
    mWordIntroTrainButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);

    mWordVariantsTrainButton = new QPushButton(QString("WordVariants"));
    mWordVariantsTrainButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);
    mMeaningVariantsTrainButton = new QPushButton(QString("MeaningVariants"));
    mMeaningVariantsTrainButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);
    mLetterToWordTrainButton = new QPushButton(QString("LetterToWordTrain"));
    mLetterToWordTrainButton->setFixedSize(0.9 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);

    mWordsRainTrainButton = new QPushButton(QString("WordsRain"));
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
    connect(mWordIntroTrainButton, &QPushButton::clicked, this, &MainWindow::initBrainstormTraining);
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

void MainWindow::updateDatabase(bool status)
{
    if (status == false) return;

    
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

void MainWindow::initBrainstormTraining()
{
    std::vector<LearnWord> mLWords = mDBManager.generateWordsForBrainStorm();

    const int size = 3;
    mTrainStates.resize(size);
    for (int i = 0; i < size; ++i)
    {
        mTrainStates[i].second = false;
    }

    mTrainStates[0].first = new WelcomeTrain(mLWords, State::Context(this, &mDBManager));
    mTrainStates[1].first = new MeaningVariantsTrain(mLWords, State::Context(this, &mDBManager));
    mTrainStates[2].first = new LetterToWordTrain(mLWords, State::Context(this, &mDBManager));
    
    setCentralWidget(mTrainStates[0].first);
    // connect(mBrainTrain, SIGNAL(endStateSignal(bool)), this, SLOT(learnStateDone(bool)));
}

void MainWindow::initWordVariantsTraining()
{
    std::vector<LearnWord> mLWords = mDBManager.generateWordsForBrainStorm();

    wordVariantsTrain = new WordVariantsTrain(mLWords, State::Context(this, &mDBManager));
    setCentralWidget(wordVariantsTrain);
}

void MainWindow::initMeaningVariantsTraining()
{
    std::vector<LearnWord> mLWords = mDBManager.generateWordsForBrainStorm();

    meaningVariantsTrain = new MeaningVariantsTrain(mLWords, State::Context(this, &mDBManager));
    setCentralWidget(meaningVariantsTrain);
}

void MainWindow::initLetterToWordTraining()
{
    std::vector<LearnWord> mLWords = mDBManager.generateWordsForBrainStorm();

    letterToWordTrain = new LetterToWordTrain(mLWords, State::Context(this, &mDBManager));
    setCentralWidget(letterToWordTrain);
}

void MainWindow::initWordsRainTraining()
{
    std::vector<LearnWord> mLWords = mDBManager.generateWordsForBrainStorm();

    wordsRainTrain = new WordsRainTrain(mLWords, State::Context(this, &mDBManager));
    setCentralWidget(wordsRainTrain);
}

#include <QDebug>
void MainWindow::initDictionaryState()
{
    qDebug() << "init dictionary!";
    dictState = new DictionaryState(State::Context(this, &mDBManager));
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