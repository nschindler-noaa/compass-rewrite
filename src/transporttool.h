#ifndef TRANSPORTTOOL_H
#define TRANSPORTTOOL_H

#include <QWidget>

namespace Ui {
class TransportTool;
}

class TransportTool : public QWidget
{
    Q_OBJECT

public:
    explicit TransportTool(QWidget *parent = nullptr);
    ~TransportTool();

private:
    Ui::TransportTool *ui;
};

#endif // TRANSPORTTOOL_H
