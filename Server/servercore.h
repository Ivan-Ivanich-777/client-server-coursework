#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include <cmath>
#include "dbmanager.h"

class ServerCore {
public:
    ServerCore();
    QString parseMessage(const QString& msg, const QString& login = "");
private:
    DBManager* m_db;
    QString processAuth(const QStringList& parts);
    QString processReg(const QStringList& parts);
    QString processStat(const QStringList& parts);
    QString processCheck(const QStringList& parts, const QString& login);
    bool checkTask1(double x, double y);
    bool checkTask2(double x, double y);
    bool checkTask3(double x, double y);
};

#endif
