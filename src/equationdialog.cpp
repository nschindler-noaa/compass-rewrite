#include "equationdialog.h"
#include "ui_equationdialog.h"

EquationDialog::EquationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EquationDialog)
{
    ui->setupUi(this);
}

EquationDialog::~EquationDialog()
{
    delete ui;
}
