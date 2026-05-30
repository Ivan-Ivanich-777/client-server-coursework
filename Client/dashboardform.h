#ifndef DASHBOARDFORM_H
#define DASHBOARDFORM_H
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include "socketmanager.h"
namespace Ui { class DashboardForm; }
class PlotWidget : public QWidget {
    Q_OBJECT
public:
    explicit PlotWidget(QWidget *parent = nullptr);
    void setParams(double a, double b, double c);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    double m_A=1,m_B=3,m_C=1;
    double f(double x);
};
class DashboardForm : public QWidget {
    Q_OBJECT
public:
    explicit DashboardForm(QWidget *p = nullptr);
    ~DashboardForm();
    void handleResponse(const QString& d);
    void setLogin(const QString& l);
signals:
    void backClicked();
private:
    Ui::DashboardForm *ui;
    QString m_login;
    double m_A=1,m_B=3,m_C=1;
    QTableWidget *m_table;
    PlotWidget *m_plotWidget;
    QSlider *sldA,*sldB,*sldC;
    QDoubleSpinBox *spnA,*spnB,*spnC;
    double localF(double x);
    void rebuildGraph();
};
#endif
