#ifndef WORDSRAINTRAIN_HPP
#define WORDSRAINTRAIN_HPP

#include "TrainState.hpp"
#include "Utility.hpp"

#include <QWidget>
#include <QAnimationGroup>
#include <QPropertyAnimation>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class WordsRainTrain : public TrainState
{
    public:
                WordsRainTrain(std::vector<LearnWord>& lWords, State::Context context, QWidget* parent = 0);
    
    private slots:
        void        askNextWord();

    private:
        void        setupCoreWidgets();
        void        setupVariantButtons(QHBoxLayout* MainLayout);

        void        updateWord();
        void        updateVariants();

    private:
        RandomEngine                mRandEngine;

        QPropertyAnimation*         mAnimation;
        QAnimationGroup*            mAnimationGroup;
        QLabel*                     mWord;

        std::vector<QPushButton*>   mMeaningVariantButtons;
        size_t                      mRightMeaningIndx;
        const int                   mVariantsSize;
};

#endif