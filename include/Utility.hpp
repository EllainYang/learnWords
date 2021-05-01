#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QKeySequence>

#include <string>

extern const std::string GetTrainTypeStatusName[6];
extern const Qt::Key charToKey[26];
extern const Qt::Key digitToKey[9];
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

enum TrainingType
{
    None = 0,
    WordIntro,
    WordVariants,
    MeaningVariants,
    LetterToWord,
    WordRain
};

struct LearnWord
{
    LearnWord();
    LearnWord(std::string&& word, std::string&& meaning, int attempts = 0);

    std::string     word;
    std::string     meaning;
    int             attempts;
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