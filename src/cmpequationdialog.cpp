#include "cmpequationdialog.h"
#include "ui_cmpequationdialog.h"

cmpEquationDialog::cmpEquationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cmpEquationDialog)
{
    ui->setupUi(this);
}

cmpEquationDialog::~cmpEquationDialog()
{
    delete ui;
}

cmpEquation *cmpEquationDialog::getEquation() const
{
    return equation;
}

void cmpEquationDialog::setEquation(cmpEquation *newEquation)
{
    equation = newEquation;
}
