#include "equationdialog.h"
//#include "ui_equationdialog.h"

EquationDialog::EquationDialog(QWidget *parent) : CommonDialog(parent)
{
    setTitle(QString("Generic Equation View"));
    showResetAll(false);
    showApplyAll(false);

    setEquation(new cmpEquation());

    eqnChartView = new QChartView();
    eqnChart = eqnChartView->chart();

    eqnLine = new QLineSeries(eqnChart);
    eqnArea = new QAreaSeries(eqnChart);

    setMainWidget(reinterpret_cast<QWidget *>(eqnChart));
}

EquationDialog::EquationDialog(cmpEquation *eqn, QWidget *parent) : CommonDialog(parent)
{
    setTitle(QString("Generic Equation View"));
    showResetAll(false);
    showApplyAll(false);

    setEquation(eqn);

    eqnChartView = new QChartView();
    eqnChart = eqnChartView->chart();

    eqnLine = new QLineSeries(eqnChart);
    eqnArea = new QAreaSeries(eqnChart);

    setMainWidget(reinterpret_cast<QWidget *>(eqnChart));
}

EquationDialog::~EquationDialog()
{
    delete eqnChart;
    delete eqnChartView;
    delete eqnLine;
    delete eqnArea;
}

void EquationDialog::setEquation(cmpEquation *eqn)
{
    equation = eqn;
    QString title = QString(QString("Equation %1").arg(eqn->getName()));
    setTitle((title));
}
