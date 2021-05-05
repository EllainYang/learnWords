#include "WordsRainTrain.hpp"

#include "MainWindow.hpp"

WordsRainTrain::WordsRainTrain(std::vector<LearnWord>& lWords, State::Context context, QWidget* parent)
: TrainState(lWords, context, 1, parent)
, mRandEngine()
, mVariantsSize(5)
{
    mRightMeaningIndx = mRandEngine.getRandom(mVariantsSize);
    setupCoreWidgets();
    setupConnections();
}

void WordsRainTrain::setupCoreWidgets()
{
    mWord = new QLabel("horizon");
    mWord->setAttribute(Qt::WA_TranslucentBackground);
    mAnimation = new QPropertyAnimation(mWord, "pos");
    mAnimation->setDuration(10000);
    mAnimation->setStartValue(QPoint(WINDOW_WIDTH/2, 10));
    mAnimation->setEndValue(QPoint(WINDOW_WIDTH/2, 0.8 * WINDOW_HEIGHT));
    mAnimation->start();

    QHBoxLayout* variantsHBoxLayout = new QHBoxLayout;
    setupVariantButtons(variantsHBoxLayout);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(variantsHBoxLayout);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(mWord);


    setLayout(layout);
}

void WordsRainTrain::setupConnections()
{
    connect(mAnimation, &QPropertyAnimation::finished, this, &WordsRainTrain::timeout);
    connect(this, &TrainState::circlePassedSignal, getContext()->window, &MainWindow::trainStateDone);
}

void WordsRainTrain::setupVariantButtons(QHBoxLayout* MainLayout)
{
    if (mVariantsSize >= 9)
    {
        throw std::runtime_error("size cannot be larger than 8, change codebase");
    }
    
    mMeaningVariantButtons.resize(mVariantsSize);
    for (int i = 0; i < mVariantsSize; ++i)
    {
        mMeaningVariantButtons[i] = new QPushButton(QString::number(i+1) + QString(") ") + QString("test"));
        mMeaningVariantButtons[i]->setMinimumSize(150, 35);
        mMeaningVariantButtons[i]->setShortcut(QKeySequence(digitToKey[i]));
        connect(mMeaningVariantButtons[i], &QPushButton::clicked, this, &WordsRainTrain::askNextWord);

        MainLayout->addWidget(mMeaningVariantButtons[i]);
        // MainLayout->stretch()
    }
    mMeaningVariantButtons[mRightMeaningIndx]->setText(QString::number(mRightMeaningIndx+1) + QString(")") + QString::fromStdString(getCurWordMeaning()));
    mMeaningVariantButtons[mRightMeaningIndx]->setShortcut(QKeySequence(digitToKey[mRightMeaningIndx]));
}

void WordsRainTrain::timeout()
{
    recordMistake();
    bool circlePassed;
    bool status = selectNextUnlearnedWord(trainSuccess(), circlePassed);
    if (circlePassed)
    {
        emit circlePassedSignal(status);
    }

    resetMistakes();
    updateWord();
    updateVariants();
}

void WordsRainTrain::askNextWord()
{
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    size_t bIndx = senderButton->text().toStdString()[0] - '0' - 1;

    if (bIndx != mRightMeaningIndx)
    {
        recordMistake();
    }

    bool circlePassed;
    bool status = selectNextUnlearnedWord(trainSuccess(), circlePassed);
    if (circlePassed)
    {
        emit circlePassedSignal(status);
    }

    resetMistakes();
    updateWord();
    updateVariants();
}

void WordsRainTrain::updateWord()
{
    mWord->setText(QString::fromStdString(getCurWord()));
    mAnimation->setCurrentTime(0);
    mAnimation->start();
}

void WordsRainTrain::updateVariants()
{
    mRightMeaningIndx = mRandEngine.getRandom(mVariantsSize);
    mMeaningVariantButtons[mRightMeaningIndx]->setText(QString::number(mRightMeaningIndx+1) + QString(")") + QString::fromStdString(getCurWordMeaning()));
    mMeaningVariantButtons[mRightMeaningIndx]->setShortcut(QKeySequence(digitToKey[mRightMeaningIndx]));
}