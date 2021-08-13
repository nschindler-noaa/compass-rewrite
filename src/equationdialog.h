#ifndef C_EQUATIONDIALOG_H
#define C_EQUATIONDIALOG_H

#include "commondialog.h"
#include "cmpequation.h"

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QAreaSeries>


class EquationDialog : public CommonDialog
{
    Q_OBJECT

public:
    explicit EquationDialog(QWidget *parent = nullptr);
    EquationDialog(cmpEquation *eqn, QWidget *parent = nullptr);
    ~EquationDialog() override;

    void setEquation(cmpEquation *eqn);

//    void showParameters();

private:
    cmpEquation *equation;
    QChart      *eqnChart;
    QChartView  *eqnChartView;
    QLineSeries *eqnLine;
    QAreaSeries *eqnArea;
};

#endif // C_EQUATIONDIALOG_H
