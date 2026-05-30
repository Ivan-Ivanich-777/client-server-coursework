#ifndef MYSERVER_H
#define MYSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include "servercore.h"

class MyTcpServer : public QObject {
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *p = nullptr);
private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
private:
    QTcpServer *m_server;
    QMap<int, QTcpSocket*> m_sockets;
    QMap<int, QString> m_logins;
    ServerCore m_core;
};
#endif
