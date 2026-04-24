#include "mainwindow.h"
#include <QStackedWidget>
#include <QLineEdit>
MainWindow::MainWindow(QWidget *p) : QMainWindow(p) {
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);
    m_login = new LoginForm(); m_reg = new RegForm(); m_dash = new DashboardForm();
    m_stack->addWidget(m_login); m_stack->addWidget(m_reg); m_stack->addWidget(m_dash);
    m_stack->setCurrentIndex(0);
    connect(m_login,&LoginForm::toRegClicked,this,[=](){ m_stack->setCurrentIndex(1); });
    connect(m_reg,&RegForm::backClicked,this,[=](){ m_stack->setCurrentIndex(0); });
    connect(m_reg,&RegForm::regSuccess,this,[=](){ m_stack->setCurrentIndex(0); });
    connect(m_login,&LoginForm::loginSuccess,this,[=](){
        m_dash->setLogin(m_login->findChild<QLineEdit*>("lineEditLogin")->text());
        m_stack->setCurrentIndex(2);
    });
    connect(m_dash,&DashboardForm::logoutRequested,this,[=](){ m_stack->setCurrentIndex(0); });
    connect(SocketManager::instance(),&SocketManager::dataReceived,this,&MainWindow::onServerResponse);
}
MainWindow::~MainWindow() {}
void MainWindow::onServerResponse(const QString &d) {
    int i = m_stack->currentIndex();
    if (i==0) m_login->handleResponse(d);
    else if (i==1) m_reg->handleResponse(d);
    else if (i==2) m_dash->handleResponse(d);
}
