#include "cmpequation.h"

cmpEquation::cmpEquation()
{
    name = QString();
    id = 0;
    formula = QString();
    numEqnParams = 0;
    numValParams = 0;
}

cmpEquation::cmpEquation(const cmpEquation &rhs)
{
    copy (rhs);
}

cmpEquation::~cmpEquation()
{
    while(parameters.count() > 0)
        delete parameters.takeLast();
}

cmpEquation *cmpEquation::copy(const cmpEquation &rhs)
{
    name = rhs.getName();
    id = rhs.getId();
    formula = rhs.getFormula();
    numEqnParams = rhs.getNumEqnParams();
    numValParams = rhs.getNumValParams();
    int totalParams = numEqnParams + numValParams;
    for (int i = 0; i < totalParams; i++)
        addParameter(rhs.getParameter(i));
    return this;
}

const QString &cmpEquation::getName() const
{
    return name;
}

void cmpEquation::setName(const QString &newName)
{
    name = newName;
}

int cmpEquation::getId() const
{
    return id;
}

void cmpEquation::setId(int newId)
{
    id = newId;
}

const QString cmpEquation::getFormula() const
{
    return formula;
}

void cmpEquation::setFormula(const QString &newFormula)
{
    formula = newFormula;
}

double cmpEquation::value(double xval)
{
    return xval;
}

void cmpEquation::addParameter(cmpEqnParameter *param)
{
    parameters.append(param);
}

void cmpEquation::addParameter (int num, double val, double mn, double mx, QString name)
{
    cmpEqnParameter *param = new cmpEqnParameter(num, val, mn, mx, name);
    addParameter(param);
}

void cmpEquation::setParameter (QString name, double val, double mn, double mx)
{
    int num = 0;
    for (int num = 0; num < parameters.count(); num++)
    {
        if (name.compare(parameters.at(num)->getName()) == 0)
            break;
    }
    setParameter(num, val, mn, mx, name);
}

void cmpEquation::setParameter (int num, double val, double mn, double mx, QString name)
{
    if (parameters.count() > num)
    {
        cmpEqnParameter *param = new cmpEqnParameter(num, val, mn, mx, name);
        parameters.takeAt(num);
        parameters.insert(num, param);
    }
}

cmpEqnParameter *cmpEquation::getParameter (int num) const
{
    return parameters.at(num);
}

cmpEqnParameter *cmpEquation::getParameter (QString name) const
{
    int num = 0;
    for (num = 0; num < parameters.count(); num++)
    {
        if (name.compare(parameters.at(num)->getName()) == 0)
            break;
    }
    return getParameter(num);
}

int cmpEquation::getNumEqnParams() const
{
    return numEqnParams;
}

void cmpEquation::setNumEqnParams(int newNumEqnParams)
{
    numEqnParams = newNumEqnParams;
}

int cmpEquation::getNumValParams() const
{
    return numValParams;
}

void cmpEquation::setNumValParams(int newNumValParams)
{
    numValParams = newNumValParams;
}

cmpEqnParameter::cmpEqnParameter()
{
    name = QString();
    id = 0;
    value = 0;
    min = 0;
    max = 100;
}

cmpEqnParameter::cmpEqnParameter(int id_, double value_, double min_, double max_, QString name_)
{
    name = name_;
    id = id_;
    value = value_;
    min = min_;
    max = max_;
}


const QString &cmpEqnParameter::getName() const
{
    return name;
}

void cmpEqnParameter::setName(const QString &newName)
{
    name = newName;
}

double cmpEqnParameter::getValue() const
{
    return value;
}

void cmpEqnParameter::setValue(double newValue)
{
    value = newValue;
}

double cmpEqnParameter::getMin() const
{
    return min;
}

void cmpEqnParameter::setMin(double newMin)
{
    min = newMin;
}

double cmpEqnParameter::getMax() const
{
    return max;
}

void cmpEqnParameter::setMax(double newMax)
{
    max = newMax;
}
