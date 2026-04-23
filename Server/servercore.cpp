#include "servercore.h"

ServerCore::ServerCore() { m_db = DBManager::getInstance(); m_db->connectDB(); }

QString ServerCore::parseMessage(const QString &msg, const QString &login) {
    QStringList parts = msg.split('&');
    if (parts.isEmpty()) return "error";
    QString cmd = parts[0];
    if (cmd == "auth") return processAuth(parts);
    if (cmd == "reg") return processReg(parts);
    if (cmd == "stat") return processStat(parts);
    if (cmd == "check") return processCheck(parts, login);
    return "error";
}

QString ServerCore::processAuth(const QStringList &parts) {
    if (parts.size() != 3) return "auth-";
    return m_db->authorizeUser(parts[1], parts[2]) ? "auth+&" + parts[1] : "auth-";
}

QString ServerCore::processReg(const QStringList &parts) {
    if (parts.size() != 4) return "reg-";
    return m_db->registerUser(parts[1], parts[2], parts[3]) ? "reg+&" + parts[1] : "reg-";
}

QString ServerCore::processStat(const QStringList &parts) {
    if (parts.size() != 2) return "stat&0&0&0";
    return "stat&" + m_db->getUserStats(parts[1]);
}

QString ServerCore::processCheck(const QStringList &parts, const QString &login) {
    if (parts.size() != 4) return "check-";
    int task = parts[1].toInt();
    double x = parts[2].toDouble();
    double y = parts[3].toDouble();
    bool result = false;
    if (task == 1) result = checkTask1(x, y);
    else if (task == 2) result = checkTask2(x, y);
    else if (task == 3) result = checkTask3(x, y);
    if (!login.isEmpty()) m_db->updateUserStats(login, task, result);
    return result ? "check+" : "check-";
}

bool ServerCore::checkTask1(double x, double y) {
    return (fabs(x) < 1e-6 && fabs(y) < 1e-6);
}

bool ServerCore::checkTask2(double x, double y) {
    double s3 = sqrt(3.0);
    // Максимум: (-sqrt(3)/3, -2*sqrt(3)/9) — ОБА отрицательные
    if (fabs(x + s3/3.0) < 1e-6 && fabs(y + 2.0*s3/9.0) < 1e-6) return true;
    // Минимум: (sqrt(3)/3, 2*sqrt(3)/9) — ОБА положительные
    if (fabs(x - s3/3.0) < 1e-6 && fabs(y - 2.0*s3/9.0) < 1e-6) return true;
    return false;
}

bool ServerCore::checkTask3(double x, double y) {
    return (fabs(x - 1.0) < 1e-6 && fabs(y - 1.0) < 1e-6);
}
