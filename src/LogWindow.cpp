#include "LogWindow.h"
#include "ui_LogWindow.h"

LogWindow::LogWindow (QWidget *parent) :
    QMainWindow (parent), Log (parent),
    ui (new Ui::LogWindow)
{
    ui->setupUi (this);
}

void LogWindow::display (QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}

LogWindow::~LogWindow()
{
    delete ui;
}
