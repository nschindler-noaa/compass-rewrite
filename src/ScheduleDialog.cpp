#include "ScheduleDialog.h"
#include "ui_ScheduleDialog.h"

ScheduleDialog::ScheduleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScheduleDialog)
{
    ui->setupUi(this);
}

ScheduleDialog::~ScheduleDialog()
{
    delete ui;
}
