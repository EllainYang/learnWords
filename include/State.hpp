#ifndef STATE_HPP
#define STATE_HPP

#include <QWidget>

class MainWindow;
class DatabaseManager;


class State : public QWidget
{
    Q_OBJECT
    
    public:
        struct Context
        {
                Context(MainWindow* window, DatabaseManager* manager);

            MainWindow*         window;
            DatabaseManager*    DBManager;
        };
    
    public:
                State(Context context, QWidget* parent = 0);
    
    protected:
        Context*                getContext();
    
    private:
        Context                     mContext;
};

#endif