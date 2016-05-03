#ifndef C_SCHEDULEDIALOG_H
#define C_SCHEDULEDIALOG_H

#include <QDialog>

namespace Ui {
    class ScheduleDialog;
}

class ScheduleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleDialog(QWidget *parent = 0);
    ~ScheduleDialog();

private:
    Ui::ScheduleDialog *ui;
    bool dirty;
};

#endif // C_SCHEDULEDIALOG_H
