#ifndef C_RELEASETOOL_H
#define C_RELEASETOOL_H

#include <QWidget>

namespace Ui {
class ReleaseTool;
}

class ReleaseTool : public QWidget
{
    Q_OBJECT

public:
    explicit ReleaseTool(QWidget *parent = 0);
    ~ReleaseTool();

private:
    Ui::ReleaseTool *ui;
};

#endif // C_RELEASETOOL_H
