#include "dashboardform.h"
#include "ui_dashboardform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QSlider>
#include <QDoubleSpinBox>
DashboardForm::DashboardForm(QWidget *p) : QWidget(p), ui(new Ui::DashboardForm) {
    ui->setupUi(this);
    QWidget *pw = new QWidget(this); QHBoxLayout *pl = new QHBoxLayout(pw);
    pl->addWidget(new QLabel("a=")); sldA = new QSlider(Qt::Horizontal); spnA = new QDoubleSpinBox();
    sldA->setRange(-1000,1000); sldA->setValue(100); spnA->setRange(-10,10); spnA->setValue(1.0);
    pl->addWidget(sldA); pl->addWidget(spnA);
    pl->addWidget(new QLabel("b=")); sldB = new QSlider(Qt::Horizontal); spnB = new QDoubleSpinBox();
    sldB->setRange(-1000,1000); sldB->setValue(300); spnB->setRange(-10,10); spnB->setValue(3.0);
    pl->addWidget(sldB); pl->addWidget(spnB);
    pl->addWidget(new QLabel("c=")); sldC = new QSlider(Qt::Horizontal); spnC = new QDoubleSpinBox();
    sldC->setRange(-1000,1000); sldC->setValue(100); spnC->setRange(-10,10); spnC->setValue(1.0);
    pl->addWidget(sldC); pl->addWidget(spnC);
    QVBoxLayout *ml = qobject_cast<QVBoxLayout*>(layout());
    if(ml) ml->insertWidget(1, pw);
    m_plotWidget = new PlotWidget(ui->frameGraph);
    if(ui->frameGraph->layout()) delete ui->frameGraph->layout();
    QVBoxLayout *gl = new QVBoxLayout(ui->frameGraph); gl->setContentsMargins(0,0,0,0); gl->addWidget(m_plotWidget);
    m_table = ui->tableWidget; m_table->setRowCount(11); m_table->setHorizontalHeaderLabels({"x","f(x)"});
    connect(sldA, &QSlider::valueChanged, [this](int v){ m_A=v/100.0; spnA->setValue(m_A); rebuildGraph(); });
    connect(sldB, &QSlider::valueChanged, [this](int v){ m_B=v/100.0; spnB->setValue(m_B); rebuildGraph(); });
    connect(sldC, &QSlider::valueChanged, [this](int v){ m_C=v/100.0; spnC->setValue(m_C); rebuildGraph(); });
    connect(spnA, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double v){ m_A=v; sldA->setValue(v*100); rebuildGraph(); });
    connect(spnB, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double v){ m_B=v; sldB->setValue(v*100); rebuildGraph(); });
    connect(spnC, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double v){ m_C=v; sldC->setValue(v*100); rebuildGraph(); });
    connect(ui->btnBackNav, &QPushButton::clicked, this, &DashboardForm::backClicked);
    rebuildGraph();
}
DashboardForm::~DashboardForm() { delete ui; }
void DashboardForm::setLogin(const QString &l) { m_login = l; }
void DashboardForm::handleResponse(const QString &) {}
double DashboardForm::localF(double x) {
    if(x<0) return m_A*x*x;
    else if(x<=2) return x*x*x-m_B*x;
    double x2=x*x; return m_C*(x2*x2-4*x2*x+4*x2);
}
void DashboardForm::rebuildGraph() {
    m_plotWidget->setParams(m_A,m_B,m_C);
    for(int i=0;i<=10;i++){
        double x=-10.0+i*2.0;
        m_table->setItem(i,0,new QTableWidgetItem(QString::number(x,'f',2)));
        m_table->setItem(i,1,new QTableWidgetItem(QString::number(localF(x),'f',4)));
    }
}
PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent) { setMinimumSize(400,300); }
void PlotWidget::setParams(double a,double b,double c) { m_A=a;m_B=b;m_C=c; update(); }
double PlotWidget::f(double x) {
    if(x<0) return m_A*x*x;
    else if(x<=2) return x*x*x-m_B*x;
    double x2=x*x; return m_C*(x2*x2-4*x2*x+4*x2);
}
void PlotWidget::paintEvent(QPaintEvent *) {
    QPainter p(this); p.fillRect(rect(), Qt::white);
    int w=width(), h=height();
    double xMin=-10,xMax=10,yMin=-10,yMax=50;
    auto mx=[&](double x){ return (x-xMin)/(xMax-xMin)*w; };
    auto my=[&](double y){ return h-(y-yMin)/(yMax-yMin)*h; };
    p.setPen(QPen(QColor(230,230,230),1));
    for(int i=-10;i<=10;i++) p.drawLine(mx(i),0,mx(i),h);
    for(int i=-10;i<=50;i+=10) p.drawLine(0,my(i),w,my(i));
    p.setPen(QPen(Qt::black,2));
    p.drawLine(mx(-10),my(0),mx(10),my(0));
    p.drawLine(mx(0),my(-10),mx(0),my(50));
    QPainterPath p1,p2,p3; bool s1=0,s2=0,s3=0;
    for(int i=0;i<=300;i++){
        double x=xMin+i*20.0/300.0;
        double y=f(x);
        QPointF pt(mx(x),my(y));
        if(x<0){ if(!s1){p1.moveTo(pt);s1=1;} else p1.lineTo(pt); }
        else if(x<=2){ if(!s2){p2.moveTo(pt);s2=1;} else p2.lineTo(pt); }
        else{ if(!s3){p3.moveTo(pt);s3=1;} else p3.lineTo(pt); }
    }
    p.setPen(QPen(QColor(0,140,255),2.5)); if(s1) p.drawPath(p1);
    p.setPen(QPen(QColor(255,130,0),2.5)); if(s2) p.drawPath(p2);
    p.setPen(QPen(QColor(150,80,220),2.5)); if(s3) p.drawPath(p3);
}
