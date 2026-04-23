#include "dbmanager.h"

DBManager* DBManager::m_instance = nullptr;

DBManager* DBManager::getInstance() {
    if (!m_instance) m_instance = new DBManager();
    return m_instance;
}

DBManager::DBManager(QObject *parent) : QObject(parent) {}

bool DBManager::connectDB(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) { qDebug() << "DB error"; return false; }
    return createTables();
}

bool DBManager::createTables() {
    QSqlQuery q;
    q.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT UNIQUE, password TEXT, email TEXT)");
    q.exec("CREATE TABLE IF NOT EXISTS stats (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, task_num INTEGER, correct INTEGER DEFAULT 0, total INTEGER DEFAULT 0)");
    return true;
}

bool DBManager::registerUser(const QString &login, const QString &password, const QString &email) {
    QSqlQuery q;
    q.prepare("INSERT INTO users (login, password, email) VALUES (:l, :p, :e)");
    q.bindValue(":l", login); q.bindValue(":p", password); q.bindValue(":e", email);
    return q.exec();
}

bool DBManager::authorizeUser(const QString &login, const QString &password) {
    QSqlQuery q;
    q.prepare("SELECT id FROM users WHERE login=:l AND password=:p");
    q.bindValue(":l", login); q.bindValue(":p", password);
    q.exec();
    return q.next();
}

int DBManager::getUserId(const QString &login) { return 1; }
QString DBManager::getUserStats(const QString &login) { return "0&0&0"; }
bool DBManager::updateUserStats(const QString &login, int taskNum, bool isCorrect) { return true; }
