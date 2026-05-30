#include "loginform.h"
#include "ui_loginform.h"
LoginForm::LoginForm(QWidget *p) : QWidget(p), ui(new Ui::LoginForm) { ui->setupUi(this); }
void LoginForm::on_btnLogin_clicked() {
    SocketManager::instance()->sendMessage("auth&"+ui->lineEditLogin->text()+"&"+ui->lineEditPass->text());
}
void LoginForm::on_btnToReg_clicked() { emit toRegClicked(); }
void LoginForm::handleResponse(const QString &d) {
    if (d.startsWith("auth+")) emit loginSuccess();
}
