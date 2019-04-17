#include "Equation.h"


Equation::Equation()
{
}

Equation::Equation(const Equation &rhs)
{
    copy (rhs);
}

Equation::~Equation ()
{

}

Equation Equation::copy (const Equation &rhs)
{
    int i = 0;
    name = rhs.name;
    id = rhs.id;
    formula = rhs.formula;
    validParameters = rhs.validParameters;
    for (i = 0; i < rhs.validParameters; i++)
    {
//        const Parameter p = rhs.parameters[i];
        parameters[i].name = rhs.parameters[i].name;
        parameters[i].value = rhs.parameters[i].value;
        parameters[i].min = rhs.parameters[i].min;
        parameters[i].max = rhs.parameters[i].max;
    }
    for (; i < MAX_PARAMETERS; i++)
    {
//        const Parameter p = *rhs.getParameter(i);
        parameters[i].name = rhs.parameters[i].name;
        parameters[i].value = rhs.parameters[i].value;
        parameters[i].min = rhs.parameters[i].min;
        parameters[i].max = rhs.parameters[i].max;
    }

    return *this;
}

void Equation::addParameter (QString name, double val, double mn, double mx)
{
    Parameter *par = getParameter(name);
    if (par != nullptr)
    {
        par->value = val;
        par->min = mn;
        par->max = mx;
    }
}

void Equation::addParameter (int num, double val, double mn, double mx)
{
    if (num < validParameters)
    {
        parameters[num].value = val;
        parameters[num].min = mn;
        parameters[num].max = mx;
    }
}

void Equation::setParameter (QString name, double val, double mn, double mx)
{
    Parameter *par = getParameter(name);
    if (par != nullptr)
    {
        par->value = val;
        par->min = mn;
        par->max = mx;
    }
}

void Equation::setParameter (int num, double val, double mn, double mx)
{
    if (num < validParameters)
    {
        parameters[num].value = val;
        parameters[num].min = mn;
        parameters[num].max = mx;
    }
}

QString Equation::getFormula ()
{
    return formula;
}

double Equation::value (double xval)
{
    xval = -1.0;

    return xval;
}

Equation::Parameter *Equation::getParameter (int num)
{
    Equation::Parameter *parm = nullptr;
    if (num < MAX_PARAMETERS)
        parm = &parameters[num];
    return parm;
}

Equation::Parameter *Equation::getParameter (QString name)
{
    Equation::Parameter *parm = nullptr;
    int i;

    for (i = 0; i < MAX_PARAMETERS; i++)
    {
        if (parameters[i].name.compare (name) == 0)
        {
            parm = &parameters[i];
            break;
        }
    }
    return parm;
}

