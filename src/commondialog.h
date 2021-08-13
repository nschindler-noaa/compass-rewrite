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
    ~CommonDialog() override;

    void setMainWidget (QWidget *main);
    void setWindowTitle (QString wtitle);
    void setTitle (QString title);
    void setClean (){setDirty(false);}
    void setDirty (bool flag = true);

public slots:
    void showReset(bool flag = true);
    void showResetAll(bool flag = true);
    void showApply(bool flat = true);
    void showApplyAll(bool flag = true);
    void showOk(bool flag = true);
    void showCancel(bool flag = true);
    void showHelp(bool flag = true);

signals:
    void Reset();
    void ResetAll();
    void Apply();
    void ApplyAll();
    void OK();
    void Cancel();
    void Help();

private:
    Ui::CommonDialog *ui;
    QWidget *mainWidget;
};


#endif // COMMONDIALOG_H
