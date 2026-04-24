#include "dashboardform.h"
#include "ui_dashboardform.h"
#include <cmath>

DashboardForm::DashboardForm(QWidget *p) : QWidget(p), ui(new Ui::DashboardForm),
    m_A(1.0), m_c1(0,140,220), m_c2(220,120,0), m_c3(140,80,220), 
    m_bg(13,17,23), m_grid(48,54,61), m_axis(200,200,210),
    m_panelOpen(false), m_updating(false)
{
    ui->setupUi(this);
    SocketManager::instance()->connectToServer();
    
    m_replotTimer = new QTimer(this);
    m_replotTimer->setSingleShot(true);
    m_replotTimer->setInterval(30);
    connect(m_replotTimer, &QTimer::timeout, this, &DashboardForm::delayedReplot);
    
    ui->btnCheck->hide();
    
    ui->sliderA->setMinimum(-500); ui->sliderA->setMaximum(500);
    ui->spinA->setMinimum(-5.0); ui->spinA->setMaximum(5.0);
    connect(ui->spinX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DashboardForm::on_spinX_valueChanged);
    connect(ui->spinA, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DashboardForm::on_spinA_valueChanged);
    setupSidePanel();
    rebuildGraph();
    ui->sliderA->setValue(100);
}

DashboardForm::~DashboardForm() { delete ui; }
void DashboardForm::setLogin(const QString &l) { m_login = l; }

void DashboardForm::retranslate() {
    ui->labelX->setText(t("sliderX"));
    ui->labelA->setText(t("sliderA"));
    updateSidePanelStyle();
    rebuildGraph();
    updatePos(ui->spinX->value());
}

void DashboardForm::applyStyle() {
    qApp->setStyleSheet(g_dark ? darkStyle() : lightStyle());
    if (m_topBar)
        m_topBar->setStyleSheet(g_dark ? 
            "QFrame#topBar { background-color: #161b22; border-bottom: 1px solid #30363d; }" : 
            "QFrame#topBar { background-color: #f6f8fa; border-bottom: 1px solid #d0d7de; }");
    updateSidePanelStyle();
}

void DashboardForm::updateSidePanelStyle() {
    if (!m_sidePanel) return;
    QLayout *oldLay = m_sidePanel->layout();
    if (oldLay) {
        QLayoutItem *item;
        while ((item = oldLay->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLay;
    }
    
    QVBoxLayout *lay = new QVBoxLayout(m_sidePanel);
    lay->setContentsMargins(10, 40, 10, 20);
    lay->setSpacing(8);

    QLabel *title = new QLabel(t("appName"));
    title->setStyleSheet("color: #58a6ff; font-size: 20px; font-weight: bold; padding-bottom: 8px;");
    lay->addWidget(title);

    // ========== ВКЛАДКА "ФУНКЦИЯ" ==========
    auto makeInfoBtn = [&](const QString& txt, const QString& info) {
        QPushButton *b = new QPushButton(txt);
        b->setStyleSheet("QPushButton { text-align: left; padding: 10px; border-radius: 6px; } QPushButton:hover { background-color: #21262d; }");
        b->setCursor(Qt::PointingHandCursor);
        connect(b, &QPushButton::clicked, [this, txt, info]() {
            QMessageBox msg(this);
            msg.setWindowTitle(txt);
            msg.setText(info);
            msg.setIcon(QMessageBox::Information);
            msg.exec();
        });
        lay->addWidget(b);
        return b;
    };

    QString funcText;
    if (g_lang == "ru") {
        funcText = "f(x) = {\n"
                   "    x²,              x < 0\n"
                   "    x³ - 3x,         0 ≤ x < 2\n"
                   "    x⁴ - 4x³ + 4x²,  x ≥ 2\n"
                   "}\n\n"
                   "Особые точки:\n"
                   "• Минимум x²: (0, 0)\n"
                   "• Максимум x³-3x: (-√3/3, -2√3/9)\n"
                   "• Минимум x³-3x: (√3/3, 2√3/9)\n"
                   "• Перегиб x⁴-4x³+4x²: (1, 1)";
    } else {
        funcText = "f(x) = {\n"
                   "    x²,              x < 0\n"
                   "    x³ - 3x,         0 ≤ x < 2\n"
                   "    x⁴ - 4x³ + 4x²,  x ≥ 2\n"
                   "}\n\n"
                   "Special points:\n"
                   "• Min of x²: (0, 0)\n"
                   "• Max of x³-3x: (-√3/3, -2√3/9)\n"
                   "• Min of x³-3x: (√3/3, 2√3/9)\n"
                   "• Inflection of x⁴-4x³+4x²: (1, 1)";
    }
    
    makeInfoBtn("📐 " + t("function"), funcText);

    lay->addSpacing(5);

    // ========== ЦВЕТА ==========
    auto makeMenuBtn = [&](const QString& txt) {
        QPushButton *b = new QPushButton(txt + "  ▾");
        b->setStyleSheet("QPushButton { text-align: left; padding: 10px; border-radius: 6px; } QPushButton:hover { background-color: #21262d; }");
        b->setCursor(Qt::PointingHandCursor);
        QMenu *m = new QMenu(this);
        b->setMenu(m);
        lay->addWidget(b);
        return m;
    };

    QMenu *colMenu = makeMenuBtn(t("colors"));
    colMenu->addAction(t("graph1"), [this](){ changeGraphColor(1); });
    colMenu->addAction(t("graph2"), [this](){ changeGraphColor(2); });
    colMenu->addAction(t("graph3"), [this](){ changeGraphColor(3); });
    colMenu->addSeparator();
    colMenu->addAction(t("graphBg"), this, &DashboardForm::changeBg);
    colMenu->addAction(t("sliderColor"), this, &DashboardForm::changeSliderColor);
    colMenu->addAction(t("gridColor"), this, &DashboardForm::changeGridColor);
    colMenu->addAction(t("axisColor"), this, &DashboardForm::changeAxisColor);
    colMenu->addSeparator();
    colMenu->addAction(t("resetAll"), this, &DashboardForm::resetAll);

    QMenu *langMenu = makeMenuBtn(t("language"));
    langMenu->addAction(t("english"), [this](){ setLanguage("en"); });
    langMenu->addAction(t("russian"), [this](){ setLanguage("ru"); });

    QMenu *thMenu = makeMenuBtn(t("theme"));
    thMenu->addAction(t("darkTheme"), [this](){ g_dark = true; applyStyle(); rebuildGraph(); updatePos(ui->spinX->value()); });
    thMenu->addAction(t("lightTheme"), [this](){ g_dark = false; applyStyle(); rebuildGraph(); updatePos(ui->spinX->value()); });

    lay->addStretch();

    QPushButton *logoutBtn = new QPushButton(t("logout"));
    logoutBtn->setObjectName("btnLogout");
    logoutBtn->setStyleSheet("QPushButton { background-color: #da3633; border: 1px solid #f85149; border-radius: 8px; padding: 12px; color: white; font-weight: bold; } QPushButton:hover { background-color: #f85149; }");
    logoutBtn->setCursor(Qt::PointingHandCursor);
    connect(logoutBtn, &QPushButton::clicked, this, &DashboardForm::logoutRequested);
    lay->addWidget(logoutBtn);
}

void DashboardForm::setupSidePanel() {
    m_overlay = new QFrame(this);
    m_overlay->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    m_overlay->setStyleSheet("background-color: rgba(0, 0, 0, 180);");
    m_overlay->setGeometry(0, 0, width(), height());
    m_overlay->hide();
    m_overlay->installEventFilter(this);

    m_sidePanel = new QFrame(this);
    m_sidePanel->setObjectName("sidePanel");
    m_sidePanel->setFixedWidth(280);
    m_sidePanel->setGeometry(-280, 0, 280, height());
    
    updateSidePanelStyle();

    m_topBar = new QFrame(this);
    m_topBar->setObjectName("topBar");
    m_topBar->setGeometry(0, 0, width(), 40);

    QHBoxLayout *topLay = new QHBoxLayout(m_topBar);
    topLay->setContentsMargins(8, 2, 4, 2);

    m_menuBtn = new QToolButton(this);
    m_menuBtn->setText("☰");
    m_menuBtn->setCursor(Qt::PointingHandCursor);
    topLay->addWidget(m_menuBtn);
    topLay->addStretch();
    
    connect(m_menuBtn, &QToolButton::clicked, this, &DashboardForm::togglePanel);
}

bool DashboardForm::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_overlay && event->type() == QEvent::MouseButtonPress) {
        if (m_panelOpen) togglePanel();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void DashboardForm::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (m_overlay) m_overlay->setGeometry(0, 0, width(), height());
    if (m_sidePanel) m_sidePanel->setGeometry(m_panelOpen ? 0 : -280, 0, 280, height());
    if (m_topBar) m_topBar->setGeometry(0, 0, width(), 40);
}

void DashboardForm::togglePanel() {
    if (m_updating) return;
    m_panelOpen = !m_panelOpen;
    m_sidePanel->setGeometry(m_panelOpen ? 0 : -280, 0, 280, height());
    m_overlay->setGeometry(0, 0, width(), height());

    QPropertyAnimation *anim = new QPropertyAnimation(m_sidePanel, "pos");
    anim->setDuration(180);
    if (m_panelOpen) {
        anim->setStartValue(QPoint(-280, 0)); anim->setEndValue(QPoint(0, 0));
        m_overlay->show(); m_overlay->raise(); m_sidePanel->raise();
    } else {
        anim->setStartValue(QPoint(0, 0)); anim->setEndValue(QPoint(-280, 0));
        connect(anim, &QPropertyAnimation::finished, this, [this](){ m_overlay->hide(); });
    }
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void DashboardForm::setLanguage(const QString &lang) {
    g_lang = lang;
    retranslate();
    applyStyle();
}

void DashboardForm::toggleTheme() { g_dark = !g_dark; applyStyle(); rebuildGraph(); updatePos(ui->spinX->value()); }
void DashboardForm::changeGraphColor(int idx) {
    QColor c = (idx==1)?m_c1:(idx==2)?m_c2:m_c3;
    QColor nc = QColorDialog::getColor(c, this);
    if (nc.isValid()) { if(idx==1)m_c1=nc; else if(idx==2)m_c2=nc; else m_c3=nc; rebuildGraph(); updatePos(ui->spinX->value()); }
}
void DashboardForm::changeSliderColor() {
    QColor c = QColorDialog::getColor(QColor("#58a6ff"), this, t("sliderColor"));
    if (c.isValid()) {
        QString s = qApp->styleSheet();
        s.replace("#58a6ff", c.name()).replace("#0969da", c.name());
        qApp->setStyleSheet(s);
    }
}
void DashboardForm::changeBg() { QColor c=QColorDialog::getColor(m_bg,this); if(c.isValid()){m_bg=c; rebuildGraph(); updatePos(ui->spinX->value());} }
void DashboardForm::changeGridColor() { QColor c=QColorDialog::getColor(m_grid,this,t("gridColor")); if(c.isValid()){m_grid=c; rebuildGraph(); updatePos(ui->spinX->value());} }
void DashboardForm::changeAxisColor() { QColor c=QColorDialog::getColor(m_axis,this,t("axisColor")); if(c.isValid()){m_axis=c; rebuildGraph(); updatePos(ui->spinX->value());} }

void DashboardForm::resetAll() { 
    m_c1=QColor(0,140,220); m_c2=QColor(220,120,0); m_c3=QColor(140,80,220); 
    m_bg=QColor(13,17,23); m_grid=QColor(48,54,61); m_axis=QColor(200,200,210); 
    applyStyle(); rebuildGraph(); updatePos(ui->spinX->value()); 
}

double DashboardForm::f(double x, double A) {
    if (x<0) return A*x*x;
    if (x<2) return A*(x*x*x-3*x);
    double x2=x*x; return A*(x2*x2-4*x2*x+4*x2);
}
void DashboardForm::updatePos(double x) {
    if (m_updating) return;
    double y=f(x,m_A);
    tracer->position->setCoords(x,y);
    tracerLabel->position->setCoords(x+0.3,y+0.3);
    tracerLabel->setText(QString("(%1,%2)").arg(x,0,'f',3).arg(y,0,'f',3));
    m_replotTimer->start();
}
void DashboardForm::delayedReplot() { ui->plot->replot(); }

void DashboardForm::on_sliderX_valueChanged(int v) {
    if (m_updating) return;
    double x=v/100.0;
    ui->spinX->blockSignals(true); ui->spinX->setValue(x); ui->spinX->blockSignals(false);
    updatePos(x);
}
void DashboardForm::on_sliderA_valueChanged(int v) {
    if (m_updating) return;
    m_A=v/100.0;
    ui->spinA->blockSignals(true); ui->spinA->setValue(m_A); ui->spinA->blockSignals(false);
    rebuildGraph(); updatePos(ui->spinX->value());
}
void DashboardForm::on_spinX_valueChanged(double v) {
    if (m_updating) return;
    ui->sliderX->blockSignals(true); ui->sliderX->setValue((int)(v*100)); ui->sliderX->blockSignals(false);
    updatePos(v);
}
void DashboardForm::on_spinA_valueChanged(double v) {
    if (m_updating) return;
    m_A=v;
    ui->sliderA->blockSignals(true); ui->sliderA->setValue((int)(v*100)); ui->sliderA->blockSignals(false);
    rebuildGraph(); updatePos(ui->spinX->value());
}
void DashboardForm::onMouseMove(QMouseEvent* e) {
    double x=ui->plot->xAxis->pixelToCoord(e->pos().x());
    double y=ui->plot->yAxis->pixelToCoord(e->pos().y());
    coordLabel->setText(QString("X:%1 Y:%2").arg(x,0,'f',3).arg(y,0,'f',3));
    m_replotTimer->start();
}
void DashboardForm::rebuildGraph() {
    QCustomPlot* p=ui->plot;
    p->clearGraphs(); p->clearItems();
    p->setBackground(m_bg);
    p->axisRect()->setBackground(m_bg.lighter(110));
    p->axisRect()->setMargins(QMargins(30,15,15,15));
    
    p->xAxis->grid()->setPen(QPen(m_grid,0.5));
    p->yAxis->grid()->setPen(QPen(m_grid,0.5));
    p->xAxis->grid()->setSubGridVisible(true); 
    p->yAxis->grid()->setSubGridVisible(true);
    p->xAxis->grid()->setSubGridPen(QPen(m_grid.lighter(120),0.3));
    p->yAxis->grid()->setSubGridPen(QPen(m_grid.lighter(120),0.3));
    
    p->xAxis->setBasePen(QPen(m_axis,1.5));
    p->yAxis->setBasePen(QPen(m_axis,1.5));
    p->xAxis->setTickPen(QPen(m_axis,1));
    p->yAxis->setTickPen(QPen(m_axis,1));
    p->xAxis->setTickLabelColor(m_axis); 
    p->yAxis->setTickLabelColor(m_axis);
    
    p->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);

    QCPItemLine *arrowX = new QCPItemLine(p);
    arrowX->setPen(QPen(m_axis, 1.5));
    arrowX->start->setCoords(3.85, 0);
    arrowX->end->setCoords(4.0, 0);
    arrowX->setHead(QCPLineEnding(QCPLineEnding::esSpikeArrow, 4, 6));

    QCPItemLine *arrowY = new QCPItemLine(p);
    arrowY->setPen(QPen(m_axis, 1.5));
    arrowY->start->setCoords(0, 8.85);
    arrowY->end->setCoords(0, 9.0);
    arrowY->setHead(QCPLineEnding(QCPLineEnding::esSpikeArrow, 4, 6));

    QCPItemText *labelX = new QCPItemText(p);
    labelX->position->setCoords(3.7, -0.15);
    labelX->setText(t("xAxis"));
    labelX->setColor(m_axis);
    labelX->setFont(QFont("Segoe UI", 8));

    QCPItemText *labelY = new QCPItemText(p);
    labelY->position->setCoords(-0.15, 8.7);
    labelY->setText(t("yAxis"));
    labelY->setColor(m_axis);
    labelY->setFont(QFont("Segoe UI", 8));

    coordLabel=new QCPItemText(p);
    coordLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    coordLabel->position->setCoords(1,0);
    coordLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignRight);
    coordLabel->setText("X:0 Y:0");
    coordLabel->setColor(m_axis);
    coordLabel->setFont(QFont("Segoe UI",8));
    coordLabel->setPadding(QMargins(6,2,6,2));

    QVector<double> x1,y1,x2,y2,x3,y3;
    for (double i=-3;i<=0;i+=0.01){x1<<i;y1<<f(i,m_A);}
    for (double i=0;i<=2;i+=0.01){x2<<i;y2<<f(i,m_A);}
    for (double i=2;i<=4;i+=0.01){x3<<i;y3<<f(i,m_A);}
    p->addGraph(); p->graph()->setData(x1,y1); p->graph()->setPen(QPen(m_c1,2));
    p->addGraph(); p->graph()->setData(x2,y2); p->graph()->setPen(QPen(m_c2,2));
    p->addGraph(); p->graph()->setData(x3,y3); p->graph()->setPen(QPen(m_c3,2));

    tracer=new QCPItemTracer(p);
    tracer->setPen(QPen(QColor(255,80,80),1.5));
    tracer->setBrush(QColor(255,100,100,150));
    tracer->setStyle(QCPItemTracer::tsCircle); tracer->setSize(8);

    tracerLabel=new QCPItemText(p);
    tracerLabel->position->setCoords(0.3,f(0,m_A)+0.3);
    tracerLabel->setText("(0,0)");
    tracerLabel->setColor(QColor(255,200,200));
    tracerLabel->setFont(QFont("Segoe UI",8,QFont::Bold));
    tracerLabel->setBrush(QBrush(QColor(22,27,34,220)));
    tracerLabel->setPadding(QMargins(5,2,5,2));

    p->xAxis->setRange(-3,4);
    p->yAxis->setRange(-3*std::max(1.0,std::abs(m_A)),9*std::max(1.0,std::abs(m_A)));
    p->replot();
    connect(p,&QCustomPlot::mouseMove,this,&DashboardForm::onMouseMove);
}
void DashboardForm::handleResponse(const QString &d) {}
void DashboardForm::on_btnCheck_clicked() {}
