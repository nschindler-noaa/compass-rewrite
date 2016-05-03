#ifndef C_EQUATIONDIALOG_H
#define C_EQUATIONDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
class EquationDialog;
}

class EquationDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EquationDialog(QWidget *parent = 0);
    ~EquationDialog();
    
private:
    Ui::EquationDialog *ui;
};

#endif // C_EQUATIONDIALOG_H
