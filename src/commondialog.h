#ifndef COMMONDIALOG_H
#define COMMONDIALOG_H

#include <QDialog>

#include "releasetool.h"

namespace Ui {
class CommonDialog;
}

class CommonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommonDialog(QWidget *parent = nullptr);
    ~CommonDialog();

    void setMainWidget (QWidget *main);
    void setTitle (QString title);
    void setClean (bool flag = true);
    void setDirty () {setClean(false);}

public slots:


signals:
    void Reset();
    void Apply();
    void OK();
    void Cancel();
    void Help();

private:
    Ui::CommonDialog *ui;
};


#endif // COMMONDIALOG_H
