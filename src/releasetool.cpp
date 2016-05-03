#include "releasetool.h"
#include "ui_releasetool.h"

ReleaseTool::ReleaseTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReleaseTool)
{
    ui->setupUi(this);
}

ReleaseTool::~ReleaseTool()
{
    delete ui;
}
