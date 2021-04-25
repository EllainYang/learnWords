#include "WelcomeTrain.hpp"
#include "MainWindow.hpp"

#include <QKeySequence>

WelcomeTrain::WelcomeTrain(const std::vector<LearnWord>& lWords, State::Context context, QWidget* parent)
: TrainState(lWords, context, 999, parent)
{
    setupCoreWidgets();
    setupConnections();
}

void WelcomeTrain::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mWord = new QLabel(QString::fromStdString(getCurWord()));
    mWordMeaning = new QLabel(QString::fromStdString(getCurWordMeaning()));
    mNextWordButton = new QPushButton("Next");
    mNextWordButton->setShortcut(QKeySequence(Qt::Key_Return));

    mMainVBoxLayout->addWidget(mWord);
    mMainVBoxLayout->addWidget(mWordMeaning);
    mMainVBoxLayout->addWidget(mNextWordButton);

    setLayout(mMainVBoxLayout);
}

void WelcomeTrain::setupConnections()
{
    connect(this, &TrainState::circlePassedSignal, getContext()->window, &MainWindow::trainStateDone);
    connect(mNextWordButton, &QPushButton::clicked, this, &WelcomeTrain::askNextWord);
}

void WelcomeTrain::askNextWord()
{
    bool circlePassed;
    bool status = selectNextUnlearnedWord(trainSuccess(), circlePassed);
    if (circlePassed)
    {
        emit circlePassedSignal(status);
    }
    
    updateWord();
}

void WelcomeTrain::updateWord()
{
    mWord->setText(QString::fromStdString(getCurWord()));
    mWordMeaning->setText(QString::fromStdString(getCurWordMeaning()));
}