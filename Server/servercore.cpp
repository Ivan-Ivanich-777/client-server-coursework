#include "servercore.h"
ServerCore::ServerCore() { m_db = DBManager::getInstance(); m_db->connectDB("C:/MyProject/mydb.sqlite"); }
QString ServerCore::parseMessage(const QString &msg, const QString &) {
    QStringList p = msg.split('&');
    if (p.isEmpty()) return "error";
    if (p[0] == "auth") return processAuth(p);
    if (p[0] == "reg") return processReg(p);
    if (p[0] == "verify") return processVerify(p);
    if (p[0] == "forgot") return processForgot(p);
    if (p[0] == "reset") return processReset(p);
    return "error";
}
QString ServerCore::processAuth(const QStringList &p) {
    if (p.size() != 3) return "auth-";
    if (m_db->authorizeUser(p[1], p[2])) return "auth+&" + p[1];
    return "auth-";
}
QString ServerCore::processReg(const QStringList &p) {
    if (p.size() != 4) return "reg-";
    QString code = QString::number(rand()%900000+100000);
    m_codes[p[3]] = code;
    m_pending[p[3]] = qMakePair(p[1], p[2]);
    sendEmail(p[3], "MathSolver - Code", "Your code: " + code);
    return "code_sent&" + p[3];
}
QString ServerCore::processVerify(const QStringList &p) {
    if (p.size() < 3) return "verify-";
    if (m_codes.value(p[1]) == p[2]) {
        m_codes.remove(p[1]);
        if (m_pending.contains(p[1])) {
            auto d = m_pending[p[1]];
            m_db->registerUser(d.first, d.second, p[1]);
            m_pending.remove(p[1]);
            return "reg+&" + d.first;
        }
        return "verify+";
    }
    return "verify-";
}
QString ServerCore::processForgot(const QStringList &p) {
    if (p.size() != 2) return "forgot-";
    QString email = m_db->getUserEmail(p[1]);
    if (email.isEmpty()) return "forgot-";
    QString code = QString::number(rand()%900000+100000);
    m_codes[p[1]] = code;
    sendEmail(email, "MathSolver - Reset", "Code: " + code);
    return "code_sent&" + p[1];
}
QString ServerCore::processReset(const QStringList &p) {
    if (p.size() != 4) return "reset-";
    if (m_codes.value(p[1]) == p[2]) { m_db->updatePassword(p[1], p[3]); m_codes.remove(p[1]); return "reset+"; }
    return "reset-";
}
void ServerCore::sendEmail(const QString &to, const QString &subject, const QString &body) {
    QSslSocket s;
    s.connectToHostEncrypted("smtp.gmail.com", 465);
    if (!s.waitForConnected(5000)) return;
    auto r = [&](){ while(s.waitForReadyRead(1000)) s.readAll(); };
    auto w = [&](const QString& c){ s.write((c+"\r\n").toUtf8()); s.waitForBytesWritten(1000); };
    r(); w("EHLO localhost"); r(); w("AUTH LOGIN"); r();
    w(QByteArray("MathSolver@gmail.com").toBase64()); r();
    w(QByteArray("vbmklilaegrkipom").toBase64()); r();
    w("MAIL FROM:<MathSolver@gmail.com>"); r();
    w("RCPT TO:<"+to.toUtf8()+">"); r(); w("DATA"); r();
    w("From: MathSolver <MathSolver@gmail.com>\r\nTo: "+to+"\r\nSubject: "+subject+"\r\n\r\n"+body+"\r\n."); r();
    w("QUIT"); r(); s.close();
}
