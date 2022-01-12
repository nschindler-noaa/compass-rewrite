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
    setNumParameters(rhs.getNumParameters());
    setNumCoefficients(rhs.getNumCoefficients());
    for (i = 0; i < parameters.count(); i++)
    {
//        Parameter *par = rhs.getParameter(i);
        parameters[i]->setName(rhs.parameters[i]->getName());
        parameters[i]->setValue(rhs.parameters[i]->getValue());
        parameters[i]->setMin(rhs.parameters[i]->getMin());
        parameters[i]->setMax(rhs.parameters[i]->getMax());
    }
    for (i = 0; i < coefficients.count(); i++)
    {
        coefficients[i]->setName(rhs.coefficients[i]->getName());
        coefficients[i]->setValue(rhs.coefficients[i]->getValue());
        coefficients[i]->setMin(rhs.coefficients[i]->getMin());
        coefficients[i]->setMax(rhs.coefficients[i]->getMax());
    }

    return *this;
}

void Equation::setNumParameters(int num)
{
    while (parameters.count() < num)
        parameters.append(new Parameter());
    while (parameters.count() > num)
        parameters.takeLast();
}

int Equation::getNumParameters() const
{
    return parameters.count();
}

void Equation::setNumCoefficients(int num)
{
    while (coefficients.count() < num)
        coefficients.append(new Parameter());
    while (coefficients.count() > num)
        coefficients.takeLast();
}

int Equation::getNumCoefficients() const
{
    return coefficients.count();
}


void Equation::addParameter (QString name, double val, double mn, double mx)
{
    Parameter *par = getParameter(name);
    if (par != nullptr)
    {
        par->setValue(val);
        par->setMin(mn);
        par->setMax(mx);
    }
}

void Equation::addParameter (int num, double val, double mn, double mx)
{
    if (num < parameters.count())
    {
        setParameter(num, val, mn, mx);
    }
    else
    {
        parameters[num]->setValue(val);
        parameters[num]->setMin(mn);
        parameters[num]->setMax(mx);
    }
}

void Equation::setParameter (QString name, double val, double mn, double mx)
{
    Parameter *par = getParameter(name);
    if (par != nullptr)
    {
        par->setValue(val);
        par->setMin(mn);
        par->setMax(mx);
    }
}

void Equation::setParameter (int num, double val, double mn, double mx)
{
    if (num < validParameters)
    {
        parameters[num]->setValue(val);
        parameters[num]->setMin(mn);
        parameters[num]->setMax(mx);
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

Parameter *Equation::getParameter (int num)
{
    Parameter *parm = nullptr;
    if (num < parameters.count())
        parm = parameters[num];
    return parm;
}

Parameter *Equation::getParameter (QString name)
{
    Parameter *parm = nullptr;
    int i;

    for (i = 0; i < parameters.count(); i++)
    {
        if (parameters[i]->getName().compare (name) == 0)
        {
            parm = parameters[i];
            break;
        }
    }
    return parm;
}

Parameter *Equation::getCoefficient(int num)
{
    Parameter *parm = nullptr;
    if (num < coefficients.count())
        parm = coefficients[num];
    return parm;
}

Parameter *Equation::getCoefficient (QString name)
{
    Parameter *parm = nullptr;
    int i;

    for (i = 0; i < coefficients.count(); i++)
    {
        if (coefficients[i]->getName().compare (name) == 0)
        {
            parm = coefficients[i];
            break;
        }
    }
    return parm;
}

const QString &Equation::getName() const
{
    return name;
}

void Equation::setName(const QString &newName)
{
    name = newName;
}

int Equation::getId() const
{
    return id;
}

void Equation::setId(int newId)
{
    id = newId;
}

void Equation::setFormula(const QString &newFormula)
{
    formula = newFormula;
}


Parameter::Parameter ()
{

}

Parameter::~Parameter ()
{

}

const QString &Parameter::getName() const
{
    return name;
}

void Parameter::setName(const QString &newName)
{
    name = newName;
}

double Parameter::getValue() const
{
    return value;
}

void Parameter::setValue(double newValue)
{
    value = newValue;
}

double Parameter::getMin() const
{
    return min;
}

void Parameter::setMin(double newMin)
{
    min = newMin;
}

double Parameter::getMax() const
{
    return max;
}

void Parameter::setMax(double newMax)
{
    max = newMax;
}
