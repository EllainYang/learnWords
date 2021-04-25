#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QKeySequence>

#include <string>

extern const std::string GetTrainTypeStatusName[1];
extern const Qt::Key charToKey[26];
extern const Qt::Key digitToKey[9];

enum TrainingType
{
    Brainstorm = 0
};

struct LearnWord
{
    LearnWord();
    LearnWord(std::string&& word, std::string&& meaning, bool status);

    std::string     word;
    std::string     meaning;
    bool            learnStatus;
};

struct RandomEngine
{
    public:
            RandomEngine();

    uint32_t    getRandom();
    int         getRandom(int bound);

    uint32_t    state;
};


#endif