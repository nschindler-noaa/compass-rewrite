#include "HelpDialog.h"
#include "ui_HelpDialog.h"
#include "help.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    QString ver = version_string();
    QString hlp = help_string(true);

    ui->pageView->setText(hlp);
    ui->labelTitle->setText(QString("Compass ver %1 Help").arg(ver));
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
