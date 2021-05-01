#include "LetterToWordTrain.hpp"
#include "Utility.hpp"
#include "MainWindow.hpp"

#include <QString>

#include <iostream>

LetterToWordTrain::LetterToWordTrain(const std::vector<LearnWord>& lWords, TrainState::Context context, QWidget* parent)
: TrainState(lWords, context, 2, parent)
, mWordGuessIndx(0)
{
    setupPalettes();
    setupCoreWidgets();
    setupConnections();
}

void LetterToWordTrain::wordGuess()
{
    std::cerr << "wordGuess\n";
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    const std::string& curWord = getCurWord();

    std::string letter = senderButton->text().toStdString();
    std::cerr << curWord[mWordGuessIndx] << " " << letter[0] << std::endl;
    if (curWord[mWordGuessIndx] == letter[0])
    {
        clearFailPalettes();
        mWordLetterSlots[mWordGuessIndx]->setText(QString::fromStdString(letter));

        if (--mWordLetters[letter[0]].second == 0)
        {
            mWordLetters[letter[0]].first->setText("");
        }

        mWordLetterSlots[mWordGuessIndx]->setPalette(mSuccessPalette);

        mWordGuessIndx++;
    }
    else
    {
        recordMistake();
        senderButton->setPalette(mFailurePalette);
    }

    if (mWordGuessIndx >= curWord.size())
    {
        // endState(mistakes <= 2);

        // mLWords[mLWordIndx].learnStatus = (mistakes <= 2);
        bool circlePassed;
        bool status = selectNextUnlearnedWord(trainSuccess(), circlePassed);
        std::cerr << getCurWord() << " " << status << " " << circlePassed << std::endl;
        if (circlePassed)
        {
            std::cerr << "circlep passed! status: " << status << std::endl;;
            emit circlePassedSignal(status);
        }
       
        mWordGuessIndx = 0;
        resetMistakes();
        updateButtons();
        
    }
}

void LetterToWordTrain::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mWordMeaning = new QLabel(QString::fromStdString(getCurWordMeaning()));
    mMainVBoxLayout->addWidget(mWordMeaning, 0, Qt::AlignCenter);

    mLetterSlotsHBoxLayout = new QHBoxLayout;
    mLettersHBoxLayout = new QHBoxLayout;
    setupWordLetters();
    mLetterSlotsHBoxLayout->setAlignment(Qt::AlignCenter);
    mLettersHBoxLayout->setAlignment(Qt::AlignCenter);
    mMainVBoxLayout->addLayout(mLetterSlotsHBoxLayout);
    mMainVBoxLayout->addLayout(mLettersHBoxLayout);

    setLayout(mMainVBoxLayout);
    
}

void LetterToWordTrain::setupWordMeaningLabel()
{
    // mWordMeaning = new QLabel(QString::fromStdString(getCurWordMeaning()));
    // mMainVBoxLayout->addWidget(mWordMeaning);
}

void LetterToWordTrain::setupPalettes()
{
    mSuccessPalette.setColor(QPalette::Button, QColor(Qt::green));
    mFailurePalette.setColor(QPalette::Button, QColor(Qt::red));
}

void LetterToWordTrain::updateButtons()
{
    clearWordLetters();
    setupWordLetters();
    mWordMeaning->setText(QString::fromStdString(getCurWordMeaning()));
}

void LetterToWordTrain::clearFailPalettes()
{
    QPushButton tempButton;
    const QPalette& defaultPalette = tempButton.palette();
    for (auto& element : mWordLetters)
    {
        auto& button = element.second.first;
        button->setPalette(defaultPalette);
    }
}

void LetterToWordTrain::clearWordLetters()
{
    for (size_t i = 0; i < mWordLetterSlots.size(); ++i)
    {
        delete mWordLetterSlots[i];
    }
    mWordLetterSlots.clear();
    qDeleteAll(mLetterSlotsHBoxLayout->children());


    for (auto& elem : mWordLetters)
    {
        delete elem.second.first;
    }
    mWordLetters.clear();
    qDeleteAll(mLettersHBoxLayout->children());
}

void LetterToWordTrain::setupWordLetters()
{
    const std::string& curWord = getCurWord();

    mWordLetterSlots.resize(curWord.size());
    for (size_t i = 0; i < curWord.size(); ++i)
    {
        char letter = curWord[i];

        mWordLetterSlots[i] = new QPushButton();
        mWordLetterSlots[i]->setFixedSize(40, 40);
        mLetterSlotsHBoxLayout->addWidget(mWordLetterSlots[i]);

        if (mWordLetters.find(letter) == mWordLetters.end())
        {
            auto& mWordLetterButton = mWordLetters.insert(
                    std::make_pair(letter, std::make_pair(new QPushButton, 1))).first->second.first;
            mWordLetterButton = new QPushButton(QString(letter));
            mWordLetterButton->setFixedSize(40, 40);
            mWordLetterButton->setShortcut(QKeySequence(charToKey[letter-'a']));
            // mLettersHBoxLayout->addWidget(mWordLetters[i]);
            connect(mWordLetterButton, &QPushButton::clicked, this, &LetterToWordTrain::wordGuess);
        }
        else
        {
            mWordLetters[letter].second++;
        }
    }

    std::vector<char> letters;
    for (auto& mWordLetter : mWordLetters)
    {
        letters.push_back(mWordLetter.first);
    }
    
    std::random_shuffle(letters.begin(), letters.end());
    for (int letter : letters)
    {
        mLettersHBoxLayout->addWidget(mWordLetters[letter].first);
    }
}

void LetterToWordTrain::setupConnections()
{
    connect(this, &TrainState::circlePassedSignal, getContext()->window, &MainWindow::trainStateDone);
}