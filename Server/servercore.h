#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QMap>
#include <QDateTime>
#include <cmath>
#include <QRandomGenerator>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QTcpSocket>
#include "dbmanager.h"

class ServerCore {
public:
    ServerCore();
    QString parseMessage(const QString& msg, const QString& login = "");
private:
    DBManager* m_db;
    QMap<QString, QString> m_verificationCodes;
    QMap<QString, QPair<QString, QString>> m_pendingRegistrations;
    QMap<QString, QString> m_resetCodes;
    
    QString generateVerificationCode();
    bool sendEmail(const QString& to, const QString& subject, const QString& body);
    QString processAuth(const QStringList& parts);
    QString processReg(const QStringList& parts);
    QString processVerify(const QStringList& parts);
    QString processForgot(const QStringList& parts);
    QString processReset(const QStringList& parts);
    QString processStat(const QStringList& parts);
    QString processCheck(const QStringList& parts, const QString& login);
    bool checkTask1(double x, double y);
    bool checkTask2(double x, double y);
    bool checkTask3(double x, double y);
};

#endif
