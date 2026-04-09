/**
 * @file mainwindow.h
 * @brief Главное окно приложения
 * @author Гусев Иван
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
class AuthForm;
class TitleForm;
class TaskForm;
class DashboardForm;
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
