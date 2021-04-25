#ifndef TRAINSTATE_HPP
#define TRAINSTATE_HPP

#include "Utility.hpp"
#include "State.hpp"

#include <vector>

class TrainState : public State
{
    Q_OBJECT

    public:
        TrainState(const std::vector<LearnWord>& lWords, State::Context context, int maxMistakes, QWidget* parent = 0);
    
    signals:
        void    endStateSignal(bool status);
        void    circlePassedSignal(bool status);

    protected:
        void    endState(bool status);

    protected:
        bool                    selectNextUnlearnedWord(bool wordSuccess, bool& circlePassed);
        const std::string&      getCurWord() const;
        const std::string&      getCurWordMeaning() const;

        void                    recordMistake();
        void                    resetMistakes();
        bool                    trainSuccess() const;

    private:
        std::vector<LearnWord>      mLWords;
        size_t                      mLWordIndx;
        LearnWord*                  mCurLWord;

        int                         mMistakes;
        const int                   mMaxMistakes;
};  


#endif