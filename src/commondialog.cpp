#include "commondialog.h"
#include "ui_CommonDialog.h"


CommonDialog::CommonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommonDialog)
{
    ui->setupUi(this);
    connect (ui->pushButton_Apply, SIGNAL(released()), SIGNAL(Apply()));
    connect (ui->pushButton_Reset, SIGNAL(released()), SIGNAL(Reset()));
    connect (ui->pushButton_Help, SIGNAL(released()), SIGNAL(Help()));
    connect (ui->pushButton_OK, SIGNAL(released()), SIGNAL(OK()));
    connect (ui->pushButton_Cancel, SIGNAL(released()), SIGNAL(Cancel()));
}

CommonDialog::~CommonDialog()
{
    delete ui;
}

void CommonDialog::setMainWidget(QWidget *main)
{
    ui->horizontalLayout_main->addWidget(main);
}

void CommonDialog::setTitle(QString title)
{
    ui->label_Title->setText(title);
}

void CommonDialog::setClean(bool flag)
{
    ui->label_DirtyUpperRight->setVisible(flag);
    ui->label_DirtyUpperLeft->setVisible(flag);
    ui->label_DirtyLowerRight->setVisible(flag);
    ui->label_DirtyLowerLeft->setVisible(flag);
}
