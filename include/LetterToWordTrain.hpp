#ifndef TESTSTATE_HPP
#define TESTSTATE_HPP

#include "TrainState.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPalette>

#include <vector>
#include <unordered_map>
#include <string>

class LetterToWordTrain : public TrainState
{
    Q_OBJECT

    public:
                LetterToWordTrain(const std::vector<LearnWord>& lWords, TrainState::Context context, QWidget* parent = 0);

    private slots:
        void        wordGuess();

    private:
        void        setupCoreWidgets();
        void        setupWordLetters();
        void        setupPalettes();
        void        setupWordMeaningLabel();
        void        setupConnections();

        void        updateButtons();

        void        clearFailPalettes();
        void        clearWordLetters();
    
    private:
        QVBoxLayout*                                                mMainVBoxLayout;
        QLabel*                                                     mWordMeaning;
        QHBoxLayout*                                                mLetterSlotsHBoxLayout;
        QHBoxLayout*                                                mLettersHBoxLayout;
        std::vector<QPushButton*>                                   mWordLetterSlots;
        std::unordered_map<char, std::pair<QPushButton*, int>>      mWordLetters;

        QPalette                    mSuccessPalette;
        QPalette                    mFailurePalette;

        size_t                      mWordGuessIndx;
};

#endif