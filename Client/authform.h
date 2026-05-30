#ifndef AUTHFORM_H
#define AUTHFORM_H
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include "socketmanager.h"

class AuthForm : public QWidget {
    Q_OBJECT
public:
    explicit AuthForm(QWidget *p = nullptr);
    void handleResponse(const QString& d);
signals:
    void loginSuccess();
private slots:
    void onLoginClicked();
    void onRegClicked();
    void onForgotClicked();
    void onVerifyClicked();
    void onResetPassClicked();
private:
    QStackedWidget *m_pages;
    // Вход
    QLineEdit *m_loginInput, *m_passInput;
    // Регистрация
    QLineEdit *m_regLogin, *m_regPass, *m_regEmail;
    // Код
    QLineEdit *m_codeInput;
    QLabel *m_codeLabel, *m_codeInfo;
    // Забыл пароль
    QLineEdit *m_forgotLogin;
    // Новый пароль (после кода)
    QLineEdit *m_newPassInput;
    
    QString m_pendingEmail, m_pendingLogin, m_mode;
    void sendRequest(const QString& cmd);
    void showLoginPage();
};
#endif
