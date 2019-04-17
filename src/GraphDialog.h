#ifndef C_GRAPHDIALOG_H
#define C_GRAPHDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QChart>

QT_CHARTS_USE_NAMESPACE

struct series {
    bool show;
    QList <double> values;
    QPixmap tick;
    QColor tickColor;
    QPen linePen;
    QPen tickPen;
};
typedef struct series GraphSeries;

namespace Ui {
    class GraphDialog;
}

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphDialog(QWidget *parent = nullptr);
    ~GraphDialog();

private:
    Ui::GraphDialog *ui;
    QList<GraphSeries> series;
    QString titleGraph;
    QString titleAxisX;
    QString titleAxisY;
    int yMin;
    int yMax;
    int xMin;
    int xMax;
//    QChart chart;



public slots:
    bool addSeries (GraphSeries gSeries);
    bool removeSeries (int index);
    bool hideSeries (int index);
    bool showSeries (int index);

};

#endif // C_GRAPHDIALOG_H
