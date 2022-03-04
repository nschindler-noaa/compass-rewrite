#include "commondialog.h"
#include "ui_CommonDialog.h"


CommonDialog::CommonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommonDialog)
{
    ui->setupUi(this);

    setTitle(QString("CommonDialog"));
    setClean();

    connect (ui->pushButton_Apply, SIGNAL(released()), SIGNAL(Apply()));
    connect (ui->pushButton_ApplyAll, SIGNAL(released()), SIGNAL(ApplyAll()));
    connect (ui->pushButton_Reset, SIGNAL(released()), SIGNAL(Reset()));
    connect (ui->pushButton_ResetAll, SIGNAL(released()), SIGNAL(ResetAll()));
    connect (ui->pushButton_Help, SIGNAL(released()), SIGNAL(Help()));
    connect (ui->pushButton_OK, SIGNAL(released()), SIGNAL(OK()));
    connect (ui->pushButton_Cancel, SIGNAL(released()), SIGNAL(Cancel()));
    mainWidget = new QWidget();
    ui->horizontalLayout_main->addWidget(mainWidget);
}

CommonDialog::~CommonDialog()
{
    delete ui;
}

void CommonDialog::setMainWidget(QWidget *main)
{
    mainWidget = main;
    repaint();
    setClean();
//    mainWidget->setParent(this);
}

void CommonDialog::setWindowTitle(QString wtitle)
{
    QDialog::setWindowTitle(wtitle);
}

void CommonDialog::setTitle(QString title)
{
    ui->label_Title->setText(title);
}

void CommonDialog::setDirty(bool flag)
{
    ui->label_DirtyUpperRight->setVisible(flag);
    ui->label_DirtyUpperLeft->setVisible(flag);
    ui->label_DirtyLowerRight->setVisible(flag);
    ui->label_DirtyLowerLeft->setVisible(flag);
}

void CommonDialog::showReset(bool flag)
{
    ui->pushButton_Reset->setVisible(flag);
}
void CommonDialog::showResetAll(bool flag)
{
    ui->pushButton_ResetAll->setVisible(flag);
}
void CommonDialog::showApply(bool flag)
{
    ui->pushButton_Apply->setVisible(flag);
}
void CommonDialog::showApplyAll(bool flag)
{
    ui->pushButton_ApplyAll->setVisible(flag);
}
void CommonDialog::showOk(bool flag)
{
    ui->pushButton_OK->setVisible(flag);
}
void CommonDialog::showCancel(bool flag)
{
    ui->pushButton_Cancel->setVisible(flag);
}
void CommonDialog::showHelp(bool flag)
{
    ui->pushButton_Help->setVisible(flag);
}

bool CommonDialog::getChangeAll() const
{
    return changeAll;
}

void CommonDialog::setChangeAll(bool newChangeAll)
{
    changeAll = newChangeAll;
    showResetAll(newChangeAll);
    showApplyAll(newChangeAll);
    setClean();
}

bool CommonDialog::getChange() const
{
    return change;
}

void CommonDialog::setChange(bool newChange)
{
    change = newChange;
    setChangeAll(newChange);
    showReset(newChange);
    showApply(newChange);
    setClean();
}
