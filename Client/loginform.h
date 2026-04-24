#ifndef LOGINFORM_H
#define LOGINFORM_H
#include <QWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include "socketmanager.h"
namespace Ui { class LoginForm; }
class LoginForm : public QWidget {
    Q_OBJECT
public:
    explicit LoginForm(QWidget *p = nullptr);
    ~LoginForm();
    void handleResponse(const QString& d);
    void retranslate();
signals:
    void toRegClicked();
    void loginSuccess();
protected:
    void keyPressEvent(QKeyEvent *e) override;
private slots:
    void on_btnLogin_clicked();
    void on_btnToReg_clicked();
    void on_btnLang_clicked();
private:
    Ui::LoginForm *ui;
};
#endif
