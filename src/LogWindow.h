#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QMainWindow>

#include "Log.h"

namespace Ui {
    class LogWindow;
}

class LogWindow : public QMainWindow, public Log
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();

public slots:
    void display (QString text);

private:
    Ui::LogWindow *ui;
};

#endif // LOGWINDOW_H
