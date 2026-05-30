#include "dbmanager.h"

DBManager* DBManager::m_instance = nullptr;

DBManager* DBManager::getInstance() {
    if (!m_instance)
        m_instance = new DBManager();
    return m_instance;
}

DBManager::DBManager(QObject *parent) : QObject(parent) {}

bool DBManager::connectDB(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) { qDebug() << "DB Error:" << m_db.lastError().text(); return false; }
    return createTables();
}

bool DBManager::createTables() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT UNIQUE, password TEXT, email TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS stats (user_id INTEGER, task_num INTEGER, correct INTEGER DEFAULT 0, total INTEGER DEFAULT 0, FOREIGN KEY(user_id) REFERENCES users(id))");
    return true;
}

bool DBManager::registerUser(const QString &l, const QString &p, const QString &e) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (login, password, email) VALUES (:l, :p, :e)");
    query.bindValue(":l", l); query.bindValue(":p", p); query.bindValue(":e", e);
    return query.exec();
}

bool DBManager::authorizeUser(const QString &l, const QString &p) {
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE login = :l AND password = :p");
    query.bindValue(":l", l); query.bindValue(":p", p);
    query.exec();
    return query.next();
}

int DBManager::getUserId(const QString &l) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE login = :l");
    query.bindValue(":l", l);
    query.exec();
    if (query.next()) return query.value(0).toInt();
    return -1;
}

QString DBManager::getUserEmail(const QString &l) {
    QSqlQuery query;
    query.prepare("SELECT email FROM users WHERE login = :l");
    query.bindValue(":l", l);
    query.exec();
    if (query.next()) return query.value(0).toString();
    return "";
}

bool DBManager::updatePassword(const QString &l, const QString &np) {
    QSqlQuery query;
    query.prepare("UPDATE users SET password = :np WHERE login = :l");
    query.bindValue(":np", np); query.bindValue(":l", l);
    return query.exec();
}

QString DBManager::getUserStats(const QString &) { return "0&0&0"; }
bool DBManager::updateUserStats(const QString &, int, bool) { return true; }
