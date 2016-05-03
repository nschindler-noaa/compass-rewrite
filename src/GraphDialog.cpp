#include "GraphDialog.h"
#include "ui_GraphDialog.h"

GraphDialog::GraphDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphDialog)
{
    ui->setupUi(this);
}

GraphDialog::~GraphDialog()
{
    delete ui;
}

bool GraphDialog::addSeries (GraphSeries gSeries)
{
    return true;
}

bool GraphDialog::removeSeries (int index)
{
    return true;
}

bool GraphDialog::hideSeries (int index)
{
    return true;
}

bool GraphDialog::showSeries (int index)
{
    return true;
}
