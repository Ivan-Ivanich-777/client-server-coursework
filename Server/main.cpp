#include <QCoreApplication>
#include "myserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    qDebug() << "========================================";
    qDebug() << "         TCP SERVER STARTED";
    qDebug() << "========================================";
    
    MyTcpServer server;
    
    return a.exec();
}
