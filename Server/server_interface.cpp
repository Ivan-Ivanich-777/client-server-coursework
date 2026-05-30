#include "server_interface.h"
#include "protocol.h"
#include <QHostAddress>
#include <iostream>

/**
 * @file server_interface.cpp
 * @brief Реализация TCP-сервера ServerInterface на базе QTcpServer (кроссплатформенная).
 */

using namespace std;

/**
 * @brief Создаёт QTcpServer и начинает прослушивание на указанном порту.
 * @throws std::runtime_error при ошибке listen().
 */
ServerInterface::ServerInterface(int port, ServerLogic& logic)
    : server(new QTcpServer()), logic(logic)
{
    if (!server->listen(QHostAddress::Any, port)) {
        string err = server->errorString().toStdString();
        delete server;
        server = nullptr;
        throw runtime_error("listen() failed: " + err);
    }

    cout << "Server running on " << port << endl;
}

/**
 * @brief Закрывает сервер и освобождает QTcpServer.
 */
ServerInterface::~ServerInterface() {
    if (server) {
        server->close();
        delete server;
    }
}

/**
 * @brief Блокирующий цикл приёма входящих TCP-соединений.
 *
 * Вызывается из main(). Принимает клиентов по одному (синхронная модель).
 */
void ServerInterface::run() {
    while (true) {
        if (!server->waitForNewConnection(-1)) {
            cerr << "waitForNewConnection() failed" << endl;
            continue;
        }

        QTcpSocket* client = server->nextPendingConnection();
        if (!client) continue;

        cout << "Client connected: "
             << client->peerAddress().toString().toStdString() << endl;

        handleClient(client);

        cout << "Client disconnected" << endl;
        delete client;
    }
}

/**
 * @brief Обрабатывает все запросы одного клиента до разрыва соединения.
 * @param client Подключённый сокет клиента.
 */
void ServerInterface::handleClient(QTcpSocket* client) {
    while (client->state() == QAbstractSocket::ConnectedState) {
        if (!client->waitForReadyRead(30000))
            break;

        QByteArray data = client->readAll();
        string raw = data.toStdString();
        cout << "Received: " << raw;

        Request req = parseRequest(raw);
        string response = logic.handleRequest(req);

        cout << "Sending: " << response;
        client->write(QByteArray::fromStdString(response));
        client->waitForBytesWritten(5000);
    }

    client->disconnectFromHost();
    client->waitForDisconnected(5000);
}
