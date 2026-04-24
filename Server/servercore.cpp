#include "servercore.h"

ServerCore::ServerCore() { m_db = DBManager::getInstance(); m_db->connectDB(); }

QString ServerCore::generateVerificationCode() {
    return QString::number(QRandomGenerator::global()->bounded(100000, 999999));
}

bool ServerCore::sendEmail(const QString& to, const QString& subject, const QString& body) {
    QSslSocket socket;
    socket.connectToHostEncrypted("smtp.gmail.com", 465);
    if (!socket.waitForConnected(5000)) {
        qDebug() << "[MAIL] Connection failed:" << socket.errorString();
        return false;
    }
    
    auto read = [&]() { 
        if (socket.waitForReadyRead(3000)) 
            qDebug() << "[MAIL]" << socket.readAll().trimmed(); 
    };
    auto send = [&](const QString& cmd) { 
        qDebug() << "[MAIL] SEND:" << (cmd.length() > 50 ? cmd.left(50) + "..." : cmd);
        socket.write((cmd + "\r\n").toUtf8()); 
        socket.waitForBytesWritten(1000); 
    };
    
    read();
    send("EHLO localhost"); read();
    send("AUTH LOGIN"); read();
    send(QByteArray("gusevivan777@gmail.com").toBase64()); read();
    send(QByteArray("orot fiyj rbsf ufqm").toBase64()); read();
    send("MAIL FROM:<gusevivan777@gmail.com>"); read();
    send("RCPT TO:<" + to.toUtf8() + ">"); read();
    send("DATA"); read();
    send("From: MathSolver <gusevivan777@gmail.com>\r\nTo: " + to.toUtf8() + "\r\nSubject: " + subject.toUtf8() + "\r\nContent-Type: text/plain; charset=UTF-8\r\n\r\n" + body.toUtf8() + "\r\n.");
    read();
    send("QUIT"); read();
    socket.close();
    
    qDebug() << "[MAIL] Sent to" << to;
    return true;
}
QString ServerCore::parseMessage(const QString &msg, const QString &login) {
    QStringList parts = msg.split('&');
    if (parts.isEmpty()) return "error";
    QString cmd = parts[0];
    if (cmd == "auth") return processAuth(parts);
    if (cmd == "reg") return processReg(parts);
    if (cmd == "verify") return processVerify(parts);
    if (cmd == "forgot") return processForgot(parts);
    if (cmd == "reset") return processReset(parts);
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
    QString login = parts[1], password = parts[2], email = parts[3];
    if (m_db->getUserId(login) != -1) return "reg-&login_exists";
    
    QString code = generateVerificationCode();
    m_verificationCodes[email] = code;
    m_pendingRegistrations[email] = qMakePair(login, password);
    
    qDebug() << "[SERVER] Sending code" << code << "to" << email;
    bool sent = sendEmail(email, "MathSolver - Verification Code",
        "Your verification code: " + code);
    
    if (!sent) {
        qDebug() << "[SERVER] EMAIL FAILED! Showing code in console.";
    }
    
    return "code_sent&" + email + "&" + code;
}

QString ServerCore::processVerify(const QStringList &parts) {
    if (parts.size() != 3) return "verify-";
    QString email = parts[1], code = parts[2];
    if (m_verificationCodes.contains(email) && m_verificationCodes[email] == code) {
        auto data = m_pendingRegistrations[email];
        if (m_db->registerUser(data.first, data.second, email)) {
            m_verificationCodes.remove(email); m_pendingRegistrations.remove(email);
            return "reg+&" + data.first;
        }
        return "reg-";
    }
    return "verify-&wrong_code";
}

QString ServerCore::processForgot(const QStringList &parts) {
    if (parts.size() != 2) return "forgot-";
    QString login = parts[1];
    QString email = m_db->getUserEmail(login);
    if (email.isEmpty()) return "forgot-&no_user";
    
    QString code = generateVerificationCode();
    m_resetCodes[login] = code;
    sendEmail(email, "MathSolver - Password Reset", "Your reset code: " + code);
    return "forgot+&" + login;
}

QString ServerCore::processReset(const QStringList &parts) {
    if (parts.size() != 4) return "reset-";
    QString login = parts[1], code = parts[2], newPass = parts[3];
    if (m_resetCodes.contains(login) && m_resetCodes[login] == code) {
        m_db->updatePassword(login, newPass);
        m_resetCodes.remove(login);
        return "reset+";
    }
    return "reset-&wrong_code";
}

QString ServerCore::processStat(const QStringList &parts) {
    if (parts.size() != 2) return "stat&0&0&0";
    return "stat&" + m_db->getUserStats(parts[1]);
}

QString ServerCore::processCheck(const QStringList &parts, const QString &login) {
    if (parts.size() != 4) return "check-";
    int task = parts[1].toInt();
    double x = parts[2].toDouble(), y = parts[3].toDouble();
    bool result = false;
    if (task == 1) result = checkTask1(x, y);
    else if (task == 2) result = checkTask2(x, y);
    else if (task == 3) result = checkTask3(x, y);
    if (!login.isEmpty()) m_db->updateUserStats(login, task, result);
    return result ? "check+" : "check-";
}

bool ServerCore::checkTask1(double x, double y) { return (fabs(x) < 1e-6 && fabs(y) < 1e-6); }
bool ServerCore::checkTask2(double x, double y) {
    double s3 = sqrt(3.0);
    if (fabs(x + s3/3.0) < 1e-6 && fabs(y + 2.0*s3/9.0) < 1e-6) return true;
    if (fabs(x - s3/3.0) < 1e-6 && fabs(y - 2.0*s3/9.0) < 1e-6) return true;
    return false;
}
bool ServerCore::checkTask3(double x, double y) { return (fabs(x - 1.0) < 1e-6 && fabs(y - 1.0) < 1e-6); }