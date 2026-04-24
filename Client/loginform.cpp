#include "loginform.h"
#include "ui_loginform.h"

// Эти строки были пропущены, добавляем их
extern QString g_lang;
extern QString t(const QString& k);
extern void initTranslations();

LoginForm::LoginForm(QWidget *p) : QWidget(p), ui(new Ui::LoginForm) {
    ui->setupUi(this);
    ui->lineEditPass->setEchoMode(QLineEdit::Password);
    retranslate();
}
LoginForm::~LoginForm() { delete ui; }
void LoginForm::keyPressEvent(QKeyEvent *e) { if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) on_btnLogin_clicked(); else QWidget::keyPressEvent(e); }
void LoginForm::on_btnLogin_clicked() {
    QString l = ui->lineEditLogin->text().trimmed(), p = ui->lineEditPass->text();
    if (l.isEmpty() || p.isEmpty()) { QMessageBox::warning(this,"Error",t("fillAll")); return; }
    SocketManager::instance()->sendMessage("auth&"+l+"&"+p);
    ui->labelStatus->setText("Connecting...");
}
void LoginForm::on_btnToReg_clicked() { emit toRegClicked(); }
void LoginForm::on_btnLang_clicked() {
    g_lang = (g_lang == "en") ? "ru" : "en";
    retranslate();
}
void LoginForm::retranslate() {
    ui->labelTitle->setText(t("signIn"));
    ui->label->setText(t("login"));
    ui->label_2->setText(t("password"));
    ui->btnLogin->setText(t("signIn"));
    ui->btnToReg->setText(t("createAccount"));
    ui->btnLang->setText(g_lang == "ru" ? "🌐 Русский" : "🌐 English");
    ui->lineEditLogin->setPlaceholderText(t("login"));
    ui->lineEditPass->setPlaceholderText(t("password"));
}
void LoginForm::handleResponse(const QString &d) {
    if (d.startsWith("auth+")) { ui->labelStatus->setText("Success!"); emit loginSuccess(); }
    else { ui->labelStatus->setText(t("wrongPass")); QMessageBox::warning(this,"Error",t("wrongPass")); }
}
