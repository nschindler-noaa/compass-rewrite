#ifndef CMPEQUATIONDIALOG_H
#define CMPEQUATIONDIALOG_H

#include <QDialog>

namespace Ui {
class cmpEquationDialog;
}

class cmpEquationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cmpEquationDialog(QWidget *parent = nullptr);
    ~cmpEquationDialog();

private:
    Ui::cmpEquationDialog *ui;
};

#endif // CMPEQUATIONDIALOG_H
