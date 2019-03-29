#ifndef C_IOWINDOW_H
#define C_IOWINDOW_H

#include <QMainWindow>
#include "ui_IOWindow.h"

namespace Ui {
    class IOWindow;
}

class IOWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IOWindow(QWidget *parent = NULL);
    ~IOWindow();

private:
    Ui::IOWindow *ui;
};

#endif // C_IOWINDOW_H
