#ifndef C_HELPDIALOG_H
#define C_HELPDIALOG_H

#include <QDialog>

#include "help.h"

namespace Ui {
    class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = nullptr);
    ~HelpDialog() override;

private:
    Ui::HelpDialog *ui;
};

#endif // C_HELPDIALOG_H
