#include "IOWindow.h"
#include "ui_IOWindow.h"

IOWindow::IOWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IOWindow)
{
    ui->setupUi(this);
}

IOWindow::~IOWindow()
{
    delete ui;
}
