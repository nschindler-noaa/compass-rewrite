#ifndef CMPMAINWINDOW_H
#define CMPMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class cmpMainWindow;
}

class cmpMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit cmpMainWindow(QWidget *parent = nullptr);
    ~cmpMainWindow();

private:
    Ui::cmpMainWindow *ui;
};

#endif // CMPMAINWINDOW_H
