#ifndef TASKFORM_H
#define TASKFORM_H
#include <QWidget>
namespace Ui { class TaskForm; }
class TaskForm : public QWidget {
    Q_OBJECT
public:
    explicit TaskForm(QWidget *p = nullptr);
    ~TaskForm();
signals:
    void nextClicked();
    void backClicked();
private:
    Ui::TaskForm *ui;
};
#endif
