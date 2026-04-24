/**
 * @file socketmanager.h
 * @brief Singleton для управления TCP-подключением к серверу
 * @author Гусев Иван
 * @version 1.0
 */

#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

/**
 * @class SocketManager
 * @brief Singleton (улучшенная реализация по Майерсу)
 *
 * Управляет единственным TCP-подключением к серверу.
 * Отправляет запросы и получает ответы.
 */
class SocketManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Получить единственный экземпляр класса
     * @return Указатель на SocketManager
     */
    static SocketManager* instance();

    /**
     * @brief Подключиться к серверу
     * @param host IP-адрес (по умолчанию 127.0.0.1)
     * @param port Порт (по умолчанию 33333)
     */
    void connectToServer(const QString& host = "127.0.0.1", quint16 port = 33333);

    /**
     * @brief Отправить сообщение на сервер
     * @param msg Строка сообщения
     */
    void sendMessage(const QString& msg);

    /**
     * @brief Проверить подключение
     * @return true если подключены
     */
    bool isConnected() const;

signals:
    /**
     * @brief Сигнал: получены данные от сервера
     * @param data Ответ сервера
     */
    void dataReceived(const QString& data);

    /**
     * @brief Сигнал: подключение установлено
     */
    void connected();

    /**
     * @brief Сигнал: подключение разорвано
     */
    void disconnected();

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
    explicit SocketManager(QObject *parent = nullptr);
    ~SocketManager() = default;

    // Запрет копирования
    SocketManager(const SocketManager&) = delete;
    SocketManager& operator=(const SocketManager&) = delete;

    QTcpSocket* m_socket;
};

#endif // SOCKETMANAGER_H
