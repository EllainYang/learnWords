#ifndef WORDVARIANTSTRAIN_HPP
#define WORDVARIANTSTRAIN_HPP

#include "TrainState.hpp"
#include "Utility.hpp"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPalette>

#include <vector>

class WordVariantsTrain : public TrainState
{
    Q_OBJECT

    public:
            WordVariantsTrain(const std::vector<LearnWord>& lWords, State::Context context, QWidget* parent = 0);
    
    private slots:
        void        askNextWord();

    private:
        void        setupCoreWidgets();
        void        setupConnections();
        void        setupVariantButtons();

        void        updateWord();
        void        updateVariants();

    private:
        RandomEngine                    mRandEngine;

        QHBoxLayout*                    mMainHBoxLayout;
        QVBoxLayout*                    mVariantsVBoxLayout;
        QLabel*                         mWord;
        std::vector<QPushButton*>       mMeaningVariantButtons;
        size_t                          mRightMeaningIndx;
        const int                       mVariantsSize;
};

#endif