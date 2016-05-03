#include "MapWindow.h"
#include "ui_MapWindow.h"

MapWindow::MapWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MapWindow)
{
    ui->setupUi(this);
}

MapWindow::~MapWindow()
{
    delete ui;
}
