#include "transporttool.h"
#include "ui_transporttool.h"

TransportTool::TransportTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransportTool)
{
    ui->setupUi(this);
}

TransportTool::~TransportTool()
{
    delete ui;
}
