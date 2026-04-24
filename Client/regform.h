#ifndef REGFORM_H
#define REGFORM_H

#include <QWidget>
#include <QMessageBox>
#include "socketmanager.h"

namespace Ui {
class RegForm;
}

class RegForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegForm(QWidget *parent = nullptr);
    ~RegForm();

    void handleResponse(const QString& data);

signals:
    void backClicked();
    void regSuccess();

private slots:
    void on_btnRegister_clicked();
    void on_btnBack_clicked();

private:
    Ui::RegForm *ui;
};

#endif // REGFORM_H
