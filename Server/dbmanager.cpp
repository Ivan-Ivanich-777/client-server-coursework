#include "dbmanager.h"

DBManager* DBManager::m_instance = nullptr;
DBManager* DBManager::getInstance() { if (!m_instance) m_instance = new DBManager(); return m_instance; }
DBManager::DBManager(QObject *parent) : QObject(parent) {}

bool DBManager::connectDB(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) { qDebug() << "[DB] Error:" << m_db.lastError().text(); return false; }
    qDebug() << "[DB] Opened:" << path;
    return createTables();
}

bool DBManager::createTables() {
    QSqlQuery q;
    
    // ТАБЛИЦА 1: users
    q.exec("CREATE TABLE IF NOT EXISTS users ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "login TEXT UNIQUE NOT NULL, "
           "password TEXT NOT NULL, "
           "email TEXT NOT NULL)");
    
    // ТАБЛИЦА 2: stats
    q.exec("CREATE TABLE IF NOT EXISTS stats ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "user_id INTEGER NOT NULL, "
           "task_num INTEGER NOT NULL, "
           "correct INTEGER DEFAULT 0, "
           "total INTEGER DEFAULT 0, "
           "FOREIGN KEY(user_id) REFERENCES users(id), "
           "UNIQUE(user_id, task_num))");
    
    // ТАБЛИЦА 3: login_history
    q.exec("CREATE TABLE IF NOT EXISTS login_history ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
           "user_id INTEGER NOT NULL, "
           "login_time DATETIME DEFAULT CURRENT_TIMESTAMP, "
           "ip_address TEXT, "
           "FOREIGN KEY(user_id) REFERENCES users(id))");
    
    qDebug() << "[DB] Tables created: users, stats, login_history";
    return true;
}

// ==================== USERS ====================
bool DBManager::registerUser(const QString &l, const QString &p, const QString &e) {
    QSqlQuery q;
    q.prepare("INSERT INTO users (login, password, email) VALUES (:l, :p, :e)");
    q.bindValue(":l", l); q.bindValue(":p", p); q.bindValue(":e", e);
    if (!q.exec()) { qDebug() << "[DB] Register error:" << q.lastError().text(); return false; }
    qDebug() << "[DB] User registered:" << l;
    return true;
}

bool DBManager::authorizeUser(const QString &l, const QString &p) {
    QSqlQuery q;
    q.prepare("SELECT id FROM users WHERE login=:l AND password=:p");
    q.bindValue(":l", l); q.bindValue(":p", p);
    return q.exec() && q.next();
}

int DBManager::getUserId(const QString &l) {
    QSqlQuery q;
    q.prepare("SELECT id FROM users WHERE login=:l");
    q.bindValue(":l", l);
    if (q.exec() && q.next()) return q.value(0).toInt();
    return -1;
}

QString DBManager::getUserEmail(const QString &l) {
    QSqlQuery q;
    q.prepare("SELECT email FROM users WHERE login=:l");
    q.bindValue(":l", l);
    if (q.exec() && q.next()) return q.value(0).toString();
    return "";
}

bool DBManager::updatePassword(const QString &l, const QString &newP) {
    QSqlQuery q;
    q.prepare("UPDATE users SET password=:p WHERE login=:l");
    q.bindValue(":p", newP); q.bindValue(":l", l);
    return q.exec();
}

// ==================== STATS ====================
QString DBManager::getUserStats(const QString &l) {
    int uid = getUserId(l);
    if (uid == -1) return "0&0&0";
    QStringList stats;
    for (int t = 1; t <= 3; t++) {
        QSqlQuery q;
        q.prepare("SELECT correct FROM stats WHERE user_id=:u AND task_num=:t");
        q.bindValue(":u", uid); q.bindValue(":t", t);
        if (q.exec() && q.next()) stats << q.value(0).toString(); else stats << "0";
    }
    return stats.join("&");
}

bool DBManager::updateUserStats(const QString &l, int t, bool c) {
    int uid = getUserId(l);
    if (uid == -1) return false;
    QSqlQuery q;
    q.prepare("INSERT INTO stats (user_id, task_num, correct, total) VALUES (:u, :t, :c, 1) "
              "ON CONFLICT(user_id, task_num) DO UPDATE SET total=total+1, correct=correct+:c2");
    q.bindValue(":u", uid); q.bindValue(":t", t);
    int corr = c ? 1 : 0;
    q.bindValue(":c", corr); q.bindValue(":c2", corr);
    return q.exec();
}

// ==================== LOGIN HISTORY ====================
bool DBManager::addLoginRecord(const QString &l, const QString &ip) {
    int uid = getUserId(l);
    if (uid == -1) return false;
    QSqlQuery q;
    q.prepare("INSERT INTO login_history (user_id, ip_address) VALUES (:u, :ip)");
    q.bindValue(":u", uid); q.bindValue(":ip", ip);
    return q.exec();
}

QString DBManager::getLastLogin(const QString &l) {
    int uid = getUserId(l);
    if (uid == -1) return "Never";
    QSqlQuery q;
    q.prepare("SELECT login_time FROM login_history WHERE user_id=:u ORDER BY id DESC LIMIT 1");
    q.bindValue(":u", uid);
    if (q.exec() && q.next()) return q.value(0).toString();
    return "Never";
}

int DBManager::getLoginCount(const QString &l) {
    int uid = getUserId(l);
    if (uid == -1) return 0;
    QSqlQuery q;
    q.prepare("SELECT COUNT(*) FROM login_history WHERE user_id=:u");
    q.bindValue(":u", uid);
    if (q.exec() && q.next()) return q.value(0).toInt();
    return 0;
}
