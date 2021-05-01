#ifndef WELCOMETRAIN_HPP
#define WELCOMETRAIN_HPP

#include "TrainState.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <vector>

class WelcomeTrain : public TrainState
{
    Q_OBJECT

    public:
                WelcomeTrain(std::vector<LearnWord>& lWords, State::Context context, QWidget* parent = 0);
    
    private slots:
        void        askNextWord();

    private:
        void        setupCoreWidgets();
        void        setupConnections();

        void        updateWord();

    private:
        QVBoxLayout*            mMainVBoxLayout;
        QLabel*                 mWord;
        QLabel*                 mWordMeaning;
        QPushButton*            mNextWordButton;
        



};

#endif