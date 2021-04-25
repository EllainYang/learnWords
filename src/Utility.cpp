#include "Utility.hpp"

#include <ctime>

LearnWord::LearnWord()
: learnStatus(false)
{
}

LearnWord::LearnWord(std::string&& word, std::string&& meaning, bool status)
: word(word)
, meaning(meaning)
, learnStatus(status)
{
}

RandomEngine::RandomEngine()
{
    state = std::time(nullptr);
}

uint32_t RandomEngine::getRandom()
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;

    return state;
}

int RandomEngine::getRandom(int bound)
{
    return getRandom() % bound;
}

const std::string GetTrainTypeStatusName[1] = {
    "BrainTrain",
};

const Qt::Key charToKey[26] = {
    Qt::Key_A,
    Qt::Key_B,
    Qt::Key_C,
    Qt::Key_D,
    Qt::Key_E,
    Qt::Key_F,
    Qt::Key_G,
    Qt::Key_H,
    Qt::Key_I,
    Qt::Key_J,
    Qt::Key_K,
    Qt::Key_L,
    Qt::Key_M,
    Qt::Key_N,
    Qt::Key_O,
    Qt::Key_P,
    Qt::Key_Q,
    Qt::Key_R,
    Qt::Key_S,
    Qt::Key_T,
    Qt::Key_U,
    Qt::Key_V,
    Qt::Key_W,
    Qt::Key_X,
    Qt::Key_Y,
    Qt::Key_Z
};

const Qt::Key digitToKey[9] = {
    Qt::Key_1,
    Qt::Key_2,
    Qt::Key_3,
    Qt::Key_4,
    Qt::Key_5,
    Qt::Key_6,
    Qt::Key_7,
    Qt::Key_8,
    Qt::Key_9,
};
