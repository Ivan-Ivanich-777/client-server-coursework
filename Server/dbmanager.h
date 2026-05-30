#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DBManager : public QObject {
    Q_OBJECT
public:
    static DBManager* getInstance();
    bool connectDB(const QString& path = "mydb.sqlite");
    bool registerUser(const QString& l, const QString& p, const QString& e);
    bool authorizeUser(const QString& l, const QString& p);
    int getUserId(const QString& l);
    QString getUserEmail(const QString& l);
    bool updatePassword(const QString& l, const QString& np);
    QString getUserStats(const QString& l);
    bool updateUserStats(const QString& l, int t, bool c);
private:
    explicit DBManager(QObject *p = nullptr);
    static DBManager *m_instance;
    QSqlDatabase m_db;
    bool createTables();
};
#endif
