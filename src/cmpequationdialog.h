#ifndef CMPEQUATIONDIALOG_H
#define CMPEQUATIONDIALOG_H

#include "cmpequation.h"

#include <QDialog>

namespace Ui {
class cmpEquationDialog;
}

class cmpEquationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cmpEquationDialog(QWidget *parent = nullptr);
    ~cmpEquationDialog() override;

    cmpEquation *getEquation() const;
    void setEquation(cmpEquation *newEquation);

private:
    Ui::cmpEquationDialog *ui;

    cmpEquation *equation;
};

#endif // CMPEQUATIONDIALOG_H
