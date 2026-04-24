#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include "loginform.h"
#include "regform.h"
#include "dashboardform.h"
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *p=nullptr);
    ~MainWindow();
private slots:
    void onServerResponse(const QString& d);
private:
    QStackedWidget *m_stack;
    LoginForm *m_login;
    RegForm *m_reg;
    DashboardForm *m_dash;
};
#endif
