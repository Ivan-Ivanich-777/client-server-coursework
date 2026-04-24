#include "regform.h"
#include "ui_regform.h"

RegForm::RegForm(QWidget *p) : QWidget(p), ui(new Ui::RegForm) {
    ui->setupUi(this);
    showCodeField(false);
}

RegForm::~RegForm() { delete ui; }

void RegForm::showCodeField(bool show) {
    ui->labelCode->setVisible(show);
    ui->lineEditCode->setVisible(show);
    ui->btnVerify->setVisible(show);
    ui->btnRegister->setVisible(!show);
}

void RegForm::on_btnRegister_clicked() {
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

    m_pendingEmail = email;
    SocketManager::instance()->sendMessage("reg&" + login + "&" + pass1 + "&" + email);
}

void RegForm::on_btnVerify_clicked() {
    QString code = ui->lineEditCode->text().trimmed();
    if (code.isEmpty()) {
        QMessageBox::warning(this, "Error", "Enter verification code!");
        return;
    }
    SocketManager::instance()->sendMessage("verify&" + m_pendingEmail + "&" + code);
}

void RegForm::on_btnBack_clicked() {
    showCodeField(false);
    emit backClicked();
}

void RegForm::handleResponse(const QString &d) {
    if (d.startsWith("reg_code&")) {
        // reg_code&email&CODE
        QStringList parts = d.split('&');
        if (parts.size() >= 3) {
            QString code = parts[2];
            
            // ПОКАЗЫВАЕМ КОД В КРАСИВОМ ОКНЕ
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Verification Code");
            msgBox.setText("Your verification code:");
            msgBox.setInformativeText("<h1 style='color:#58a6ff; font-size:36px; letter-spacing:8px;'>" + code + "</h1>");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            
            // Показываем поле для ввода кода
            showCodeField(true);
            ui->labelStatus->setText("Enter the 6-digit code from the popup");
        }
    }
    else if (d.startsWith("reg+")) {
        showCodeField(false);
        ui->labelStatus->setText("Success!");
        QMessageBox::information(this, "Success", "Registration complete! Now you can log in.");
        emit regSuccess();
    }
    else if (d.startsWith("verify-")) {
        ui->labelStatus->setText("Wrong code!");
        QMessageBox::warning(this, "Error", "Wrong verification code! Try again.");
    }
    else if (d.startsWith("reg-&login_exists")) {
        ui->labelStatus->setText("Login taken!");
        QMessageBox::warning(this, "Error", "This login is already taken!");
    }
    else if (d == "reg-") {
        ui->labelStatus->setText("Error!");
        QMessageBox::warning(this, "Error", "Registration failed!");
    }
}
