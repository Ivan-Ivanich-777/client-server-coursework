#ifndef REGFORM_H
#define REGFORM_H
#include <QWidget>
#include "socketmanager.h"
namespace Ui { class RegForm; }
class RegForm : public QWidget {
    Q_OBJECT
public:
    explicit RegForm(QWidget *p = nullptr);
    void handleResponse(const QString& d);
signals:
    void backClicked();
    void regSuccess();
private slots:
    void on_btnRegister_clicked();
    void on_btnBack_clicked();
private:
    Ui::RegForm *ui;
};
#endif
