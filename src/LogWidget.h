#ifndef C_LOGWIDGET_H
#define C_LOGWIDGET_H

#include <QWidget>
#include <QMenuBar>

#include "Log.h"

namespace Ui {
    class LogWidget;
}

class LogWidget : public QWidget, public Log
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = 0);
    ~LogWidget();

//    QMenuBar *menuBar() {return menu;}
    void setMessage (bool flg);
    void setWarning (bool flg);
    void setRawDump (bool flg);
    void setDebug (bool flg);
    void clear ();

public slots:
    void display (QString text);

signals:
    void detach();
    void attach();

private:
    Ui::LogWidget *ui;
    QMenuBar *menu;

private slots:
    void on_action_Print_triggered();
    void on_actionSave_As_triggered();
    void on_action_Reset_triggered();
    void on_actionDebug_toggled(bool show);
    void on_actionRaw_Data_toggled(bool show);
    void on_actionWarning_toggled(bool show);
    void on_actionMessage_toggled(bool show);
};

#endif // C_LOGWIDGET_H
