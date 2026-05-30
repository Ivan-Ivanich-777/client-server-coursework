#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include "authform.h"
#include "titleform.h"
#include "taskform.h"
#include "dashboardform.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *p = nullptr);
private:
    QStackedWidget *m_stack;
    TitleForm *m_title;
    TaskForm *m_task;
    DashboardForm *m_dash;
    AuthForm *m_auth;
};
#endif
