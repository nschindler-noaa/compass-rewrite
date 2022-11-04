#include "cmpmainwindow.h"
#include "ui_cmpmainwindow.h"

cmpMainWindow::cmpMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cmpMainWindow)
{
    ui->setupUi(this);
}

cmpMainWindow::~cmpMainWindow()
{
    delete ui;
}
