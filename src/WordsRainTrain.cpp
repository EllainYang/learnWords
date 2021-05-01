#include "WordsRainTrain.hpp"

WordsRainTrain::WordsRainTrain(const std::vector<LearnWord>& lWords, State::Context context, QWidget* parent)
: TrainState(lWords, context, 1, parent)
, mRandEngine()
, mVariantsSize(5)
{
    mRightMeaningIndx = mRandEngine.getRandom(mVariantsSize);
    setupCoreWidgets();
}

void WordsRainTrain::setupCoreWidgets()
{
    mWord = new QLabel("horizon");
    mAnimation = new QPropertyAnimation(mWord, "pos");
    mAnimation->setDuration(10000);
    mAnimation->setStartValue(QPoint(50, 0));
    mAnimation->setEndValue(QPoint(50, 400));
    mAnimation->start();

    QHBoxLayout* variantsHBoxLayout = new QHBoxLayout;
    setupVariantButtons(variantsHBoxLayout);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(variantsHBoxLayout);
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(mWord);


    setLayout(layout);
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
        mMeaningVariantButtons[i]->setShortcut(QKeySequence(digitToKey[i]));
        connect(mMeaningVariantButtons[i], &QPushButton::clicked, this, &WordsRainTrain::askNextWord);

        MainLayout->addWidget(mMeaningVariantButtons[i]);
    }
    mMeaningVariantButtons[mRightMeaningIndx]->setText(QString::number(mRightMeaningIndx+1) + QString(")") + QString::fromStdString(getCurWordMeaning()));
    mMeaningVariantButtons[mRightMeaningIndx]->setShortcut(QKeySequence(digitToKey[mRightMeaningIndx]));
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

    updateWord();
    updateVariants();
}

void WordsRainTrain::updateWord()
{
    mWord->setText(QString::fromStdString(getCurWord()));
}

void WordsRainTrain::updateVariants()
{
    mRightMeaningIndx = mRandEngine.getRandom(mVariantsSize);
    mMeaningVariantButtons[mRightMeaningIndx]->setText(QString::number(mRightMeaningIndx+1) + QString(")") + QString::fromStdString(getCurWordMeaning()));
    mMeaningVariantButtons[mRightMeaningIndx]->setShortcut(QKeySequence(digitToKey[mRightMeaningIndx]));
}