#include "mainwindow.h"
#include "authform.h"
#include "titleform.h"
#include "taskform.h"
#include "dashboardform.h"
#include "socketmanager.h"

MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
    setWindowTitle("Math Solver");
    resize(1200, 800);
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);
    m_title = new TitleForm();
    m_task = new TaskForm();
    m_dash = new DashboardForm();
    m_auth = new AuthForm();
    m_stack->addWidget(m_title);
    m_stack->addWidget(m_task);
    m_stack->addWidget(m_dash);
    m_stack->setCurrentIndex(0);
    m_auth->setParent(this);
    m_auth->setGeometry((width()-360)/2, (height()-420)/2, 360, 420);
    m_auth->show();
    connect(m_auth, &AuthForm::loginSuccess, [this](){ m_auth->hide(); });
    connect(m_title, &TitleForm::nextClicked, [this](){ m_stack->setCurrentIndex(1); });
    connect(m_task, &TaskForm::nextClicked, [this](){ m_stack->setCurrentIndex(2); });
    connect(m_task, &TaskForm::backClicked, [this](){ m_stack->setCurrentIndex(0); });
    connect(m_dash, &DashboardForm::backClicked, [this](){ m_stack->setCurrentIndex(1); });
    connect(SocketManager::instance(), &SocketManager::dataReceived, [this](const QString &d){ m_auth->handleResponse(d); });
    SocketManager::instance()->connectToServer("127.0.0.1", 33333);
}
