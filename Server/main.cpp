#include <QCoreApplication>
#include "myserver.h"
#include "dbmanager.h"
#include "servercore.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    MyTcpServer server;
    return a.exec();
}
