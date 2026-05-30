#include "myserver.h"
#include <QDateTime>

MyTcpServer::MyTcpServer(QObject *p) : QObject(p) {
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &MyTcpServer::onNewConnection);
    m_server->listen(QHostAddress::Any, 33333);
    qDebug() << "========================================";
    qDebug() << "  SERVER STARTED on port 33333";
    qDebug() << "  Time:" << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    qDebug() << "========================================";
}

void MyTcpServer::onNewConnection() {
    QTcpSocket *s = m_server->nextPendingConnection();
    int id = s->socketDescriptor();
    m_sockets.insert(id, s);
    qDebug() << "";
    qDebug() << "[CONNECT] New client connected";
    qDebug() << "  Socket ID:" << id;
    qDebug() << "  IP:" << s->peerAddress().toString();
    qDebug() << "  Total clients:" << m_sockets.size();
    connect(s, &QTcpSocket::readyRead, this, &MyTcpServer::onReadyRead);
    connect(s, &QTcpSocket::disconnected, this, &MyTcpServer::onDisconnected);
}

void MyTcpServer::onReadyRead() {
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    if (!s) return;
    
    int id = s->socketDescriptor();
    QString data = QString::fromUtf8(s->readAll()).trimmed();
    QStringList parts = data.split('&');
    QString cmd = parts.isEmpty() ? "unknown" : parts[0];
    
    qDebug() << "";
    qDebug() << "[REQUEST] From client" << id;
    qDebug() << "  Command:" << cmd;
    qDebug() << "  Raw data:" << data;
    
    QString login = m_logins.value(id, "");
    QString resp = m_core.parseMessage(data, login);
    
    // Сохраняем логин при успешной авторизации/регистрации
    if (cmd == "auth" && resp.startsWith("auth+")) {
        login = resp.section('&', 1);
        m_logins.insert(id, login);
        qDebug() << "  -> User logged in:" << login;
    }
    if (cmd == "reg" && resp.startsWith("reg+")) {
        login = resp.section('&', 1);
        m_logins.insert(id, login);
        qDebug() << "  -> User registered:" << login;
    }
    
    qDebug() << "  Response:" << resp.left(100);
    s->write((resp + "\r\n").toUtf8());
}

void MyTcpServer::onDisconnected() {
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    if (!s) return;
    int id = s->socketDescriptor();
    QString login = m_logins.value(id, "unknown");
    qDebug() << "";
    qDebug() << "[DISCONNECT] Client" << id << "(" << login << ") disconnected";
    qDebug() << "  Remaining clients:" << m_sockets.size() - 1;
    m_sockets.remove(id);
    m_logins.remove(id);
    s->deleteLater();
}
