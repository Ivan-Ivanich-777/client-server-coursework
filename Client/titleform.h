#ifndef TITLEFORM_H
#define TITLEFORM_H
#include <QWidget>
namespace Ui { class TitleForm; }
class TitleForm : public QWidget {
    Q_OBJECT
public:
    explicit TitleForm(QWidget *p = nullptr);
signals:
    void nextClicked();
private slots:
    void on_btnNext_clicked();
private:
    Ui::TitleForm *ui;
};
#endif
