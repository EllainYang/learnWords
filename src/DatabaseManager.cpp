#include "DatabaseManager.hpp"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>

#include <iostream>

#define DATABASE_NAME   "dictionary_db"
#define TABLE_NAME      "dict"

DatabaseManager::DatabaseManager()
: mDBName(DATABASE_NAME)
, mTableName(TABLE_NAME)
{
    mDBConnection = QSqlDatabase::addDatabase("QMYSQL", "dictionary");

    mDBConnection.setHostName("127.0.0.1");
    mDBConnection.setDatabaseName(DATABASE_NAME);
    mDBConnection.setUserName("root");
    mDBConnection.setPassword("");
    mDBConnection.setPort(3306);

    if (!mDBConnection.open())
    {
        qDebug() << mDBConnection.lastError();
        throw std::runtime_error(std::string("Cannot open database: ") + mDBConnection.databaseName().toStdString());
    }
    
   QString table_definition = "USE dictionary_db;\n"\
                "CREATE TABLE IF NOT EXISTS dict (\n"\
                "ID INT AUTO_INCREMENT,\n"\
                "WORD VARCHAR(100) NOT NULL UNIQUE,\n"\
                "MEANING VARCHAR(1000) NOT NULL,\n"\
                "WordIntro BOOL NOT NULL DEFAULT 0,\n"\
                "WordVariants BOOL NOT NULL DEFAULT 0,\n"\
                "MeaningVariants BOOL NOT NULL DEFAULT 0,\n"\
                "LetterToWord BOOL NOT NULL DEFAULT 0,\n"\
                "WordRain BOOL NOT NULL DEFAULT 0,\n"\
                "PRIMARY KEY (ID)\n"
                ") ENGINE=INNODB;";

    QSqlQuery table_creator(table_definition, mDBConnection);
}

int DatabaseManager::countWordsForTraining(TrainingType tType) const
{
    QSqlQuery statement((std::string("select count(1) from ") + mTableName + " where " + GetTrainTypeStatusName[tType] + " = 0;").c_str(), mDBConnection);
    statement.next();
    int cnt = statement.value(statement.record().indexOf("count(1)")).toString().toInt();

    return cnt;
}

int DatabaseManager::countFullyLearnedWords() const
{
    QSqlQuery statement(QString("select count(1) from ") + QString::fromStdString(mTableName) + QString(" where ") + QString::fromStdString(GetTrainTypeStatusName[TrainingType::WordIntro]) + QString(" = 1;"), mDBConnection);
    statement.next();
    int cnt = statement.value(statement.record().indexOf("count(1)")).toString().toInt();

    return cnt;
}

int DatabaseManager::countWords() const
{
    QSqlQuery statement(QString("select count(1) from ") + QString::fromStdString(mTableName) + QString(";"), mDBConnection);
    statement.next();
    int cnt = statement.value(statement.record().indexOf("count(1)")).toString().toInt();

    return cnt;
}

std::vector<LearnWord> DatabaseManager::generateWordsForTraining(TrainingType tType) const
{
    std::vector<LearnWord> mWords;

    int wAmount = 10; // 10 words for any training except those listed below
    if (tType == TrainingType::WordIntro)
    {
        wAmount = 6;
    }
    else if (tType == TrainingType::WordRain)
    {
        wAmount = 30;
    }

    QSqlQuery statement(QString("select * from " TABLE_NAME " where %1=0 limit %2;").arg(QString::fromStdString(GetTrainTypeStatusName[tType]),QString::number(wAmount)), mDBConnection);
    QSqlRecord record = statement.record();

    if (statement.size() < 6)
    {
        qDebug() << "Not enough words for brain storm, 6 is minimum!";
    }

    while (statement.next())
    {
        std::string word = statement.value(record.indexOf("WORD")).toString().toStdString();
        std::string meaning = statement.value(record.indexOf("MEANING")).toString().toStdString();
        bool status = statement.value(record.indexOf("WordIntro")).toBool();

        mWords.push_back(LearnWord(std::move(word), std::move(meaning), status));
    }
    return mWords;
}

void DatabaseManager::setLearnWordsStatus(const std::vector<LearnWord>& lWords, TrainingType trainType)
{
    QSqlQuery update_statement(mDBConnection);
    update_statement.prepare(QString("update " TABLE_NAME " set %1=:newStatus WHERE WORD=:word").arg(QString::fromStdString(GetTrainTypeStatusName[trainType])));
    for (auto& lWord : lWords)
    {
        update_statement.bindValue(":newStatus", lWord.attempts == 0);
        update_statement.bindValue(":word", QString::fromStdString(lWord.word));
        update_statement.exec();
    }
}

const std::string& DatabaseManager::getDBName() const
{
    return mDBName;
}

const std::string& DatabaseManager::getTableName() const
{
    return mTableName;
}

QSqlDatabase& DatabaseManager::getDBConnection()
{
    return mDBConnection;
}