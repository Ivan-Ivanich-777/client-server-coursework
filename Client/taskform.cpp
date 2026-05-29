#include "taskform.h"
#include "ui_taskform.h"
#include <QPixmap>

TaskForm::TaskForm(QWidget *p) : QWidget(p), ui(new Ui::TaskForm) {
    ui->setupUi(this);

    QPixmap funcPic("function.png");
    if (!funcPic.isNull()) {
        ui->labelFormula->setPixmap(funcPic);
        ui->labelFormula->setScaledContents(true);
    } else {
        ui->labelFormula->setText("f(x) = { x^2, x<0; x^3-3x, 0<=x<2; x^4-4x^3+4x^2, x>=2 }");
    }

    QPixmap featPic("features.png");
    if (!featPic.isNull()) {
        ui->labelFeatures->setPixmap(featPic);
        ui->labelFeatures->setScaledContents(true);
    } else {
        ui->labelFeatures->setText("Min: (0,0); Max: (-1/sqrt(3), ...); Inflection: (1,1)");
    }

    connect(ui->btnNext, &QPushButton::clicked, this, &TaskForm::nextClicked);
    connect(ui->btnBack, &QPushButton::clicked, this, &TaskForm::backClicked);
}

TaskForm::~TaskForm() { delete ui; }
