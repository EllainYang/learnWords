#include "State.hpp"

State::Context::Context(MainWindow* window, DatabaseManager* manager)
: window(window)
, DBManager(manager)
{
}

State::State(Context context, QWidget* parent)
: QWidget(parent)
, mContext(context)
{

}

State::Context* State::getContext()
{
    return &mContext;
}