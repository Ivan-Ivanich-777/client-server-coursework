#include "myserver.h"

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent) {
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);
    if (!m_server->listen(QHostAddress::Any, 33333))
        qDebug() << "Server failed to start:" << m_server->errorString();
    else
        qDebug() << "Server started on port 33333";
}

void MyTcpServer::slotNewConnection() {
    QTcpSocket* s = m_server->nextPendingConnection();
    int id = s->socketDescriptor();
    m_sockets.insert(id, s);
    connect(s, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(s, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
    qDebug() << "New connection:" << id;
}

void MyTcpServer::slotServerRead() {
    QTcpSocket* s = qobject_cast<QTcpSocket*>(sender());
    if (!s) return;
    QString data = QString::fromUtf8(s->readAll()).trimmed();
    QString login = m_logins.value(s->socketDescriptor(), "");
    QString resp = m_core.parseMessage(data, login);
    if (data.startsWith("auth") && resp.startsWith("auth+"))
        m_logins.insert(s->socketDescriptor(), resp.split('&')[1]);
    s->write(resp.toUtf8());
}

void MyTcpServer::slotClientDisconnected() {
    QTcpSocket* s = qobject_cast<QTcpSocket*>(sender());
    if (!s) return;
    int id = s->socketDescriptor();
    m_sockets.remove(id);
    m_logins.remove(id);
    s->deleteLater();
    qDebug() << "Client disconnected:" << id;
}
