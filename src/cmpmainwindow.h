#ifndef CMPMAINWINDOW_H
#define CMPMAINWINDOW_H

#include <QMainWindow>

//#include "cmplogwidget.h"
#include "cmpfile.h"
//#include "cmpscenario.h"
#include "cmpcommandsettings.h"
//#include "cmpreleasedialog.h"
//#include "cmptransporttool.h"
//#include "cmphelpdialog.h"
#include "cmpequationdialog.h"

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
