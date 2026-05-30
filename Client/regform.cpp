#include "regform.h"
#include "ui_regform.h"
#include <QDebug>

RegForm::RegForm(QWidget *p) : QWidget(p), ui(new Ui::RegForm) { 
    ui->setupUi(this); 
    qDebug() << "[REGFORM] Created";
}
void RegForm::on_btnRegister_clicked() {
    QString login = ui->lineEditLogin->text();
    QString email = ui->lineEditEmail->text();
    qDebug() << "[REGFORM] Register:" << login << email;
    SocketManager::instance()->sendMessage("reg&"+login+"&"+ui->lineEditPass->text()+"&"+email);
}
void RegForm::on_btnBack_clicked() { 
    qDebug() << "[REGFORM] Back to login";
    emit backClicked(); 
}
void RegForm::handleResponse(const QString &d) {
    qDebug() << "[REGFORM] Response:" << d;
    if (d.startsWith("reg+")) {
        qDebug() << "[REGFORM] Registration SUCCESS";
        emit regSuccess();
    }
}
