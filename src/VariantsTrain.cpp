#include "VariantsTrain.hpp"
#include "MainWindow.hpp"

#include <QColor>

VariantTrain::VariantTrain(const std::vector<LearnWord>& lWords, State::Context context, QWidget* parent)
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

    setupPalettes();
    setupCoreWidgets();
    setupConnections();
}

void VariantTrain::setupPalettes()
{
    QPushButton tempButton;
    mFailurePalette.setColor(QPalette::Button, QColor(Qt::red));
    mSuccessPalette.setColor(QPalette::Button, QColor(Qt::green));
    mDefaultPalette = tempButton.palette();
}

void VariantTrain::setupCoreWidgets()
{
    mWord = new QLabel(QString::fromStdString(getCurWord()));

    mVariantsVBoxLayout = new QVBoxLayout;
    setupVariantButtons();

    mMainHBoxLayout = new QHBoxLayout;
    mMainHBoxLayout->addWidget(mWord);
    mMainHBoxLayout->addLayout(mVariantsVBoxLayout);
    setLayout(mMainHBoxLayout);
}

void VariantTrain::setupVariantButtons()
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
        connect(mMeaningVariantButtons[i], &QPushButton::clicked, this, &VariantTrain::askNextWord);

        mVariantsVBoxLayout->addWidget(mMeaningVariantButtons[i]);
    }
    mMeaningVariantButtons[mRightMeaningIndx]->setText(QString::number(mRightMeaningIndx+1) + QString(")") + QString::fromStdString(getCurWordMeaning()));
    mMeaningVariantButtons[mRightMeaningIndx]->setShortcut(QKeySequence(digitToKey[mRightMeaningIndx]));


}
#include <QDebug>
void VariantTrain::askNextWord()
{
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    size_t bIndx = senderButton->text().toStdString()[0] - '0' - 1;
    if (bIndx != mRightMeaningIndx)
    {
        recordMistake();
        senderButton->setPalette(mFailurePalette);
        return;
    }

    bool circlePassed;
    bool status = selectNextUnlearnedWord(trainSuccess(), circlePassed);
    qDebug() << "Status: " << status;
    if (circlePassed)
    {
        emit circlePassedSignal(status);
    }

    updateWord();
    updateVariants();
}

void VariantTrain::updateWord()
{
    mWord->setText(QString::fromStdString(getCurWord()));
}

void VariantTrain::updateVariants()
{
    for (auto& button : mMeaningVariantButtons)
    {
        button->setPalette(mDefaultPalette);
    }

    mRightMeaningIndx = mRandEngine.getRandom(mVariantsSize);
    mMeaningVariantButtons[mRightMeaningIndx]->setText(QString::number(mRightMeaningIndx+1) + QString(")") + QString::fromStdString(getCurWordMeaning()));
    mMeaningVariantButtons[mRightMeaningIndx]->setShortcut(QKeySequence(digitToKey[mRightMeaningIndx]));
}

void VariantTrain::setupConnections()
{
    connect(this, &VariantTrain::circlePassedSignal, getContext()->window, &MainWindow::trainStateDone);
}