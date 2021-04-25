#include "TrainState.hpp"
#include "MainWindow.hpp"
#include "DatabaseManager.hpp"

TrainState::TrainState(const std::vector<LearnWord>& lWords, State::Context context, int maxMistakes, QWidget* parent)
: State(context, parent)
, mLWords(lWords)
, mLWordIndx(0)
, mCurLWord(&mLWords[mLWordIndx])
, mMistakes(0)
, mMaxMistakes(maxMistakes)
{
    
}

void TrainState::endState(bool status)
{
    emit endStateSignal(status);
}

bool TrainState::selectNextUnlearnedWord(bool success, bool& circlePassed)
{
    mLWords[mLWordIndx].learnStatus = success;

    size_t indx = mLWordIndx;
    do
    {
        indx = (indx + 1) % mLWords.size();
    } while (mLWords[indx].learnStatus && indx != mLWordIndx);


    mCurLWord = &mLWords[indx];
    circlePassed = (indx <= mLWordIndx);
    bool isAllTrue = indx == mLWordIndx && mLWords[indx].learnStatus == true;
    mLWordIndx = indx;

    return isAllTrue;
}

const std::string& TrainState::getCurWord() const
{
    return mCurLWord->word;
}

const std::string& TrainState::getCurWordMeaning() const
{
    return mCurLWord->meaning;
};

void TrainState::recordMistake()
{
    mMistakes++;
}

void TrainState::resetMistakes()
{
    mMistakes = 0;
}

bool TrainState::trainSuccess() const
{
    return mMistakes < mMaxMistakes;
}