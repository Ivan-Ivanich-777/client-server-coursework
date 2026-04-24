/**
 * @file socketmanager.cpp
 * @brief Реализация Singleton для TCP-подключения
 */

#include "socketmanager.h"

SocketManager* SocketManager::instance()
{
    static SocketManager manager;
    return &manager;
}

SocketManager::SocketManager(QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::readyRead,
            this, &SocketManager::onReadyRead);
    connect(m_socket, &QTcpSocket::connected,
            this, &SocketManager::onConnected);
    connect(m_socket, &QTcpSocket::disconnected,
            this, &SocketManager::onDisconnected);
}

void SocketManager::connectToServer(const QString &host, quint16 port)
{
    qDebug() << "[Client] Connecting to" << host << ":" << port;
    m_socket->connectToHost(host, port);
}

void SocketManager::sendMessage(const QString &msg)
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "[Client] Sending:" << msg;
        m_socket->write(msg.toUtf8());
        m_socket->flush();
    } else {
        qDebug() << "[Client] Error: not connected!";
    }
}

bool SocketManager::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void SocketManager::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    QString strData = QString::fromUtf8(data).trimmed();

    if (!strData.isEmpty()) {
        qDebug() << "[Client] Received:" << strData;
        emit dataReceived(strData);
    }
}

void SocketManager::onConnected()
{
    qDebug() << "[Client] Connected to server!";
    emit connected();
}

void SocketManager::onDisconnected()
{
    qDebug() << "[Client] Disconnected from server!";
    emit disconnected();
}
