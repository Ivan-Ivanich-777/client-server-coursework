#ifndef SERVERCORE_H
#define SERVERCORE_H
#include <QString>
#include <QStringList>
#include <QMap>
#include <QPair>
#include <QSslSocket>
#include <QDebug>
#include <cmath>
#include "dbmanager.h"

class ServerCore {
public:
    ServerCore();
    QString parseMessage(const QString &msg, const QString &login);
private:
    DBManager *m_db;
    QMap<QString, QString> m_codes;
    QMap<QString, QPair<QString, QString>> m_pending;
    
    QString processAuth(const QStringList &p);
    QString processReg(const QStringList &p);
    QString processVerify(const QStringList &p);
    QString processForgot(const QStringList &p);
    QString processReset(const QStringList &p);
    QString processCalcTable(const QStringList &p);
    QString processCheck(const QStringList &p, const QString &login);
    void sendEmail(const QString &to, const QString &subject, const QString &body);
};
#endif
