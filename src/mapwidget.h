#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class MapWidget;
}

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = NULL);
    ~MapWidget();

signals:
    void help(QString);

private slots:
    void on_actionHelp_triggered();

private:
    Ui::MapWidget *ui;
};

#endif // MAPWIDGET_H
