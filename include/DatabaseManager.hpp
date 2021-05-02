#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include "NonCopyable.hpp"
#include "Utility.hpp"

#include <QSqlDatabase>

#include <vector>

class DatabaseManager : private NonCopyable
{
    public:
        DatabaseManager();
    
    public:
        std::vector<LearnWord>      generateWordsForTraining(TrainingType tType) const;

        int                         countWordsForTraining(TrainingType tType) const;
        int                         countFullyLearnedWords() const;
        int                         countWords() const;

        void                        setLearnWordsStatus(const std::vector<LearnWord>& lWords, TrainingType trainType);

        const std::string&          getDBName() const;
        const std::string&          getTableName() const;
        QSqlDatabase&               getDBConnection();

    
    private:
        QSqlDatabase                mDBConnection;
        
        const std::string           mDBName;
        const std::string           mTableName;
};

#endif