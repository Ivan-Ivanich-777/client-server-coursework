#ifndef REGFORM_H
#define REGFORM_H
#include <QWidget>
#include <QMessageBox>
#include <QLineEdit>
#include "socketmanager.h"

namespace Ui { class RegForm; }

class RegForm : public QWidget {
    Q_OBJECT
public:
    explicit RegForm(QWidget *p = nullptr);
    ~RegForm();
    void handleResponse(const QString& d);
signals:
    void backClicked();
    void regSuccess();
private slots:
    void on_btnRegister_clicked();
    void on_btnBack_clicked();
    void on_btnVerify_clicked();
private:
    Ui::RegForm *ui;
    QString m_pendingEmail;
    void showCodeField(bool show);
};
#endif
