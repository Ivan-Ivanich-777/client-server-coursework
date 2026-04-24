#include "regform.h"
#include "ui_regform.h"

RegForm::RegForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegForm)
{
    ui->setupUi(this);
}

RegForm::~RegForm()
{
    delete ui;
}

void RegForm::on_btnRegister_clicked()
{
    QString login = ui->lineEditLogin->text().trimmed();
    QString pass1 = ui->lineEditPass->text();
    QString pass2 = ui->lineEditPass2->text();
    QString email = ui->lineEditEmail->text().trimmed();

    if (login.isEmpty() || pass1.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Error", "Fill all fields!");
        return;
    }
    if (pass1 != pass2) {
        QMessageBox::warning(this, "Error", "Passwords do not match!");
        return;
    }

    SocketManager::instance()->sendMessage("reg&" + login + "&" + pass1 + "&" + email);
    ui->labelStatus->setText("Sending...");
}

void RegForm::on_btnBack_clicked()
{
    emit backClicked();
}

void RegForm::handleResponse(const QString &data)
{
    if (data.startsWith("reg+")) {
        ui->labelStatus->setText("Registration successful!");
        QMessageBox::information(this, "Success", "Now log in with your credentials.");
        emit regSuccess();
    } else {
        ui->labelStatus->setText("Registration error");
        QMessageBox::warning(this, "Error", "User already exists!");
    }
}
