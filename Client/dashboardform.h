#ifndef DASHBOARDFORM_H
#define DASHBOARDFORM_H
#include <QWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include <QColorDialog>
#include <QToolButton>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QMenu>
#include <QFrame>
#include <QPropertyAnimation>
#include <QTimer>
#include "socketmanager.h"
#include "qcustomplot.h"

extern bool g_dark;
extern QString g_lang;
QString darkStyle();
QString lightStyle();
QString t(const QString& k);

namespace Ui { class DashboardForm; }

class DashboardForm : public QWidget {
    Q_OBJECT
public:
    explicit DashboardForm(QWidget *p = nullptr);
    ~DashboardForm();
    void handleResponse(const QString& d);
    void setLogin(const QString& l);
    void retranslate();
signals:
    void logoutRequested();
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void on_btnCheck_clicked();
    void on_sliderX_valueChanged(int v);
    void on_sliderA_valueChanged(int v);
    void on_spinX_valueChanged(double v);
    void on_spinA_valueChanged(double v);
    void onMouseMove(QMouseEvent* e);
    void delayedReplot();
private:
    Ui::DashboardForm *ui;
    QString m_login;
    QCPItemTracer *tracer;
    QCPItemText *tracerLabel, *coordLabel;
    double m_A;
    QColor m_c1, m_c2, m_c3, m_bg, m_grid, m_axis;
    QFrame *m_sidePanel;
    QFrame *m_overlay;
    QFrame *m_topBar;
    QToolButton *m_menuBtn;
    bool m_panelOpen;
    bool m_updating;
    QTimer *m_replotTimer;

    void rebuildGraph();
    double f(double x, double A);
    void updatePos(double x);
    void setupSidePanel();
    void togglePanel();
    void changeGraphColor(int idx);
    void changeSliderColor();
    void changeBg();
    void changeGridColor();
    void changeAxisColor();
    void resetAll();
    void setLanguage(const QString& lang);
    void toggleTheme();
    void applyStyle();
    void updateSidePanelStyle();
};
#endif
