#include "authform.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
AuthForm::AuthForm(QWidget *p) : QWidget(p) {
    setStyleSheet("background-color: white;");
    QVBoxLayout *l = new QVBoxLayout(this);
    l->setAlignment(Qt::AlignCenter);
    QLabel *t = new QLabel("Math Solver"); t->setStyleSheet("font-size:24px;font-weight:bold; color:#333;"); l->addWidget(t);
    m_loginInput = new QLineEdit(); m_loginInput->setPlaceholderText("Login"); m_loginInput->setMinimumHeight(40);
    m_loginInput->setStyleSheet("padding:8px;border:1px solid #ccc;border-radius:4px;font-size:14px;"); l->addWidget(m_loginInput);
    m_passInput = new QLineEdit(); m_passInput->setPlaceholderText("Password"); m_passInput->setEchoMode(QLineEdit::Password); m_passInput->setMinimumHeight(40);
    m_passInput->setStyleSheet("padding:8px;border:1px solid #ccc;border-radius:4px;font-size:14px;"); l->addWidget(m_passInput);
    QPushButton *btnLogin = new QPushButton("Sign In"); btnLogin->setMinimumHeight(44);
    btnLogin->setStyleSheet("background-color:#4a76a8;color:white;border:none;border-radius:4px;font-size:16px;font-weight:bold;"); l->addWidget(btnLogin);
    connect(btnLogin, &QPushButton::clicked, this, &AuthForm::onLoginClicked);
    QPushButton *btnReg = new QPushButton("Create Account"); btnLogin->setMinimumHeight(36);
    btnReg->setStyleSheet("background:transparent;color:#4a76a8;border:none;font-size:14px;"); l->addWidget(btnReg);
    connect(btnReg, &QPushButton::clicked, this, &AuthForm::onRegClicked);
    setMinimumSize(380, 440);
}
void AuthForm::onLoginClicked() { SocketManager::instance()->sendMessage("auth&"+m_loginInput->text()+"&"+m_passInput->text()); }
void AuthForm::onRegClicked() { SocketManager::instance()->sendMessage("reg&"+m_loginInput->text()+"&"+m_passInput->text()+"&"+m_loginInput->text()+"@mail.com"); }
void AuthForm::onVerifyClicked() {}
void AuthForm::onForgotClicked() {}
void AuthForm::onResetPassClicked() {}
void AuthForm::handleResponse(const QString &d) { if (d.startsWith("auth+") || d.startsWith("reg+")) emit loginSuccess(); }
