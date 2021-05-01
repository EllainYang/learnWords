#include "TrainState.hpp"
#include "MainWindow.hpp"
#include "DatabaseManager.hpp"

TrainState::TrainState(std::vector<LearnWord>& lWords, State::Context context, int maxMistakes, QWidget* parent)
: State(context, parent)
, mLWords(lWords)
, mLWordsStatus(lWords.size(), false)
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
    mLWordsStatus[mLWordIndx] = success;

    size_t indx = mLWordIndx;
    do
    {
        indx = (indx + 1) % mLWords.size();
    } while (mLWordsStatus[indx] && indx != mLWordIndx);


    mCurLWord = &mLWords[indx];
    circlePassed = (indx <= mLWordIndx);
    bool isAllTrue = indx == mLWordIndx && mLWordsStatus[indx] == true;
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
    mCurLWord->attempts++;
}

void TrainState::resetMistakes()
{
    mMistakes = 0;
}

bool TrainState::trainSuccess() const
{
    return mMistakes < mMaxMistakes;
}