#include "socketmanager.h"
#include <QDebug>

SocketManager* SocketManager::instance() {
    static SocketManager mgr;
    return &mgr;
}

SocketManager::SocketManager(QObject *p) : QObject(p) {
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &SocketManager::onReadyRead);
}

void SocketManager::connectToServer(const QString &host, quint16 port) {
    qDebug() << "[SOCKET] Connecting to" << host << ":" << port;
    m_socket->connectToHost(host, port);
}

void SocketManager::sendMessage(const QString &msg) {
    qDebug() << "[SOCKET] Sending:" << msg;
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(msg.toUtf8());
        m_socket->flush();
    }
}

void SocketManager::onReadyRead() {
    QByteArray data = m_socket->readAll();
    emit dataReceived(QString::fromUtf8(data).trimmed());
}
