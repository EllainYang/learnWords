#include "WordVariantsTrain.hpp"
#include "MainWindow.hpp"

#include <QColor>

WordVariantsTrain::WordVariantsTrain(std::vector<LearnWord>& lWords, State::Context context, QWidget* parent)
: TrainState(lWords, context, 1, parent)
, mRandEngine()
, mVariantsSize(5)
{
    mRightMeaningIndx = mRandEngine.getRandom(mVariantsSize);

    setStyleSheet(QString::fromStdString(
        "QPushButton:pressed"\
        "{"\
            "border-style:solid;"\
            "border-width:2px;"\
            "color: rgb(0,175,0)"\
        "}"));

    setupCoreWidgets();
    setupConnections();
}

void WordVariantsTrain::setupCoreWidgets()
{
    mWord = new QLabel(QString::fromStdString(getCurWordMeaning()));
    // mWord->setAlignment();

    mVariantsVBoxLayout = new QVBoxLayout;
    mVariantsVBoxLayout->setAlignment(Qt::AlignCenter);
    setupVariantButtons();

    mMainHBoxLayout = new QHBoxLayout;
    mMainHBoxLayout->addWidget(mWord, 0, Qt::AlignCenter);
    mMainHBoxLayout->addLayout(mVariantsVBoxLayout);
    setLayout(mMainHBoxLayout);
}

void WordVariantsTrain::setupVariantButtons()
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
        mMeaningVariantButtons[i]->setFixedSize(0.4 * WINDOW_WIDTH, 50);
        connect(mMeaningVariantButtons[i], &QPushButton::clicked, this, &WordVariantsTrain::askNextWord);

        mVariantsVBoxLayout->addWidget(mMeaningVariantButtons[i]);
    }
    mMeaningVariantButtons[mRightMeaningIndx]->setText(QString::number(mRightMeaningIndx+1) + QString(")") + QString::fromStdString(getCurWord()));
    mMeaningVariantButtons[mRightMeaningIndx]->setShortcut(QKeySequence(digitToKey[mRightMeaningIndx]));
}

#include <QDebug>
void WordVariantsTrain::askNextWord()
{
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    size_t bIndx = senderButton->text().toStdString()[0] - '0' - 1;
    if (bIndx != mRightMeaningIndx)
    {
        recordMistake();
        senderButton->setStyleSheet(QString::fromStdString(
        "QPushButton"\
        "{"\
            "border-style:solid;"\
            "border-width:2px;"\
            "color: rgb(175,0,0)"\
        "}"));
        return;
    }

    bool circlePassed;
    bool status = selectNextUnlearnedWord(trainSuccess(), circlePassed);
    qDebug() << "Status: " << status;
    if (circlePassed)
    {
        emit circlePassedSignal(status);
    }

    resetMistakes();
    updateWord();
    updateVariants();
}

void WordVariantsTrain::updateWord()
{
    mWord->setText(QString::fromStdString(getCurWordMeaning()));
}

void WordVariantsTrain::updateVariants()
{
    for (auto& button : mMeaningVariantButtons)
    {
        button->setStyleSheet(QString::fromStdString(
        "QPushButton"\
        "{"\
            "border-style:solid;"\
            "border-width:2px;"\
            "color: #b1b1b1"\
        "}"));
    }

    mRightMeaningIndx = mRandEngine.getRandom(mVariantsSize);
    mMeaningVariantButtons[mRightMeaningIndx]->setText(QString::number(mRightMeaningIndx+1) + QString(")") + QString::fromStdString(getCurWord()));
    mMeaningVariantButtons[mRightMeaningIndx]->setShortcut(QKeySequence(digitToKey[mRightMeaningIndx]));
}

void WordVariantsTrain::setupConnections()
{
    connect(this, &WordVariantsTrain::circlePassedSignal, getContext()->window, &MainWindow::trainStateDone);
}