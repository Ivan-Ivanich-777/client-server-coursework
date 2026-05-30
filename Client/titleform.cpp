#include "titleform.h"
#include "ui_titleform.h"
TitleForm::TitleForm(QWidget *p) : QWidget(p), ui(new Ui::TitleForm) { ui->setupUi(this); }
void TitleForm::on_btnNext_clicked() { emit nextClicked(); }
