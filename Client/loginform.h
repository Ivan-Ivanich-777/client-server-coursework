#ifndef LOGINFORM_H
#define LOGINFORM_H
#include <QWidget>
#include "socketmanager.h"
namespace Ui { class LoginForm; }
class LoginForm : public QWidget {
    Q_OBJECT
public:
    explicit LoginForm(QWidget *p = nullptr);
    void handleResponse(const QString& d);
signals:
    void toRegClicked();
    void loginSuccess();
private slots:
    void on_btnLogin_clicked();
    void on_btnToReg_clicked();
private:
    Ui::LoginForm *ui;
};
#endif
