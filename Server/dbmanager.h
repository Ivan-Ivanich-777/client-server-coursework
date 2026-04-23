#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DBManager : public QObject
{
    Q_OBJECT
public:
    static DBManager* getInstance();
    bool connectDB(const QString& path = "mydb.sqlite");
    bool registerUser(const QString& login, const QString& password, const QString& email);
    bool authorizeUser(const QString& login, const QString& password);
    QString getUserStats(const QString& login);
    bool updateUserStats(const QString& login, int taskNum, bool isCorrect);
    
private:
    explicit DBManager(QObject *parent = nullptr);
    static DBManager* m_instance;
    QSqlDatabase m_db;
    bool createTables();
    int getUserId(const QString& login);
};

#endif
