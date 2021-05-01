#include "WelcomeTrain.hpp"
#include "MainWindow.hpp"

#include <QKeySequence>

WelcomeTrain::WelcomeTrain(std::vector<LearnWord>& lWords, State::Context context, QWidget* parent)
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

    mWord->setAlignment(Qt::AlignCenter);
    mWordMeaning->setAlignment(Qt::AlignCenter);


    mNextWordButton = new QPushButton("Next", this);
    mNextWordButton->setShortcut(QKeySequence(Qt::Key_Return));
    // mNextWordButton->setFixedSize(0.8 * WINDOW_WIDTH, 40);
    mNextWordButton->resize(0.8 * WINDOW_WIDTH, 40);
    mNextWordButton->move(WINDOW_WIDTH / 2 - 0.4*WINDOW_WIDTH, WINDOW_HEIGHT / 1.5);
    mNextWordButton->show();

    mMainVBoxLayout->setAlignment(Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mWord);
    mMainVBoxLayout->addWidget(mWordMeaning);
    // mMainVBoxLayout->addWidget(mNextWordButton);

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