#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QDebug>

class SocketManager : public QObject {
    Q_OBJECT
public:
    static SocketManager* instance();
    void connectToServer(const QString& host = "127.0.0.1", quint16 port = 33333);
    void sendMessage(const QString& msg);
signals:
    void dataReceived(const QString& data);
private:
    explicit SocketManager(QObject *p = nullptr);
    QTcpSocket *m_socket;
    void onReadyRead();
};
#endif
