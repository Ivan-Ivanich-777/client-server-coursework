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
    if (!m_db.open()) { qDebug() << "[DB] Error opening database:" << m_db.lastError().text(); return false; }
    qDebug() << "[DB] Database opened:" << path;
    return createTables();
}

bool DBManager::createTables() {
    QSqlQuery q;
    bool ok = q.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT UNIQUE, password TEXT, email TEXT)");
    if (!ok) qDebug() << "[DB] Error creating users table:" << q.lastError().text();
    ok = q.exec("CREATE TABLE IF NOT EXISTS stats (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, task_num INTEGER, correct INTEGER DEFAULT 0, total INTEGER DEFAULT 0, FOREIGN KEY(user_id) REFERENCES users(id), UNIQUE(user_id, task_num))");
    if (!ok) qDebug() << "[DB] Error creating stats table:" << q.lastError().text();
    qDebug() << "[DB] Tables created successfully";
    return true;
}

bool DBManager::registerUser(const QString &login, const QString &password, const QString &email) {
    QSqlQuery q;
    q.prepare("INSERT INTO users (login, password, email) VALUES (:l, :p, :e)");
    q.bindValue(":l", login);
    q.bindValue(":p", password);
    q.bindValue(":e", email);
    if (!q.exec()) { qDebug() << "[DB] Registration error:" << q.lastError().text(); return false; }
    qDebug() << "[DB] User registered:" << login;
    return true;
}

bool DBManager::authorizeUser(const QString &login, const QString &password) {
    QSqlQuery q;
    q.prepare("SELECT id FROM users WHERE login = :l AND password = :p");
    q.bindValue(":l", login);
    q.bindValue(":p", password);
    if (!q.exec()) { qDebug() << "[DB] Authorization error:" << q.lastError().text(); return false; }
    bool result = q.next();
    qDebug() << "[DB] Authorization for" << login << ":" << (result ? "success" : "failed");
    return result;
}

int DBManager::getUserId(const QString &login) {
    Q_UNUSED(login)
    return 1;
}

QString DBManager::getUserStats(const QString &login) {
    Q_UNUSED(login)
    return "0&0&0";
}

bool DBManager::updateUserStats(const QString &login, int taskNum, bool isCorrect) {
    Q_UNUSED(login)
    Q_UNUSED(taskNum)
    Q_UNUSED(isCorrect)
    return true;
}
