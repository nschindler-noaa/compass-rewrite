#include "cmpequation.h"

static void setEquationNames();
static QStringList equationNames;


cmpEquation::cmpEquation()
{
    setEquationNames();
    setId(0);
}

cmpEquation::cmpEquation(int eqid)
{
    setEquationNames();
    setId(eqid);
}

cmpEquation::cmpEquation(QString eqname)
{
    setEquationNames();
    setName(eqname);
}

cmpEquation::cmpEquation(const cmpEquation &rhs)
{
    copy (rhs);
}

cmpEquation &cmpEquation::operator=(const cmpEquation &rhs)
{
    copy (rhs);
    return *this;
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
    setNumParameters(totalParams);
    for (int i = 0; i < totalParams; i++)
        setParameter(i, rhs.getParameter(i));
    return this;
}

bool cmpEquation::operator==(const cmpEquation &rhs)
{
    return isEqual(rhs);
}

bool cmpEquation::operator==(cmpEquation &rhs)
{
    return isEqual(rhs);
}

bool cmpEquation::isEqual(const cmpEquation &rhs)
{
    bool equal = true;
    if (rhs.getId() != id) {
        equal = false;
    }
    else if (rhs.getNumEqnParams() != numEqnParams) {
        equal = false;
    }
    else if (rhs.getNumValParams() != numValParams) {
        equal = false;
    }
    else
    {
        int tot = numEqnParams + numValParams;
        for (int i = 0; i < tot && equal; i++)
        {
            if (rhs.getParameter(i)->isEqual(*getParameter(i)))
                equal = false;
        }
    }
    return equal;
}

const QString &cmpEquation::getName() const
{
    return name;
}

void cmpEquation::setName(const QString &newName)
{
    bool okay = true;
    int id = equationNames.indexOf(newName);
    if (id < 0)
        id = newName.toInt(&okay);
    if (okay)
        setId(id);
    else
        setId(0);
}

const QString &cmpEquation::getDescription() const
{
    return description;
}

void cmpEquation::setDescription(const QString &newDesc)
{
    description = newDesc;
}

int cmpEquation::getId() const
{
    return id;
}

void cmpEquation::setId(int newId)
{
    if (id != newId)
        id = newId;
    setupEquation();
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

void cmpEquation::setNumParameters(int newNum)
{
    int curnum = parameters.isEmpty()? 0: parameters.count();
    int i = curnum;
    for (i = curnum; i < newNum; i++)
        addParameter(new cmpEqnParameter());
    while (i < parameters.count())
        delete parameters.takeLast();
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

void cmpEquation::setParameter(int num, cmpEqnParameter *param)
{
    if (num > parameters.count())
        setNumParameters(num);
    setParameter(num, param->getValue(), param->getMin(), param->getMax(), param->getName());
}

void cmpEquation::setParameter (QString name, double val, double mn, double mx)
{
    int num = 0;
    for (num = 0; num < parameters.count(); num++)
    {
        if (name.compare(parameters.at(num)->getName()) == 0)
            break;
    }
    setParameter(num, val, mn, mx, name);
}

void cmpEquation::setParameter (int num, double val, double mn, double mx, QString name)
{
    cmpEqnParameter *param = new cmpEqnParameter(num, val, mn, mx, name);
    if (parameters.count() > num)
    {
        delete parameters.takeAt(num);
        parameters.insert(num, param);
    }
    else
    {
        for (int i = parameters.count(); i < num; i++)
        {
            parameters.append(new cmpEqnParameter());
        }
        parameters.append(param);
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
    clear();
}

cmpEqnParameter::cmpEqnParameter(const cmpEqnParameter &rhs)
{
    name = rhs.getName();
    id = rhs.getId();
    value = rhs.getValue();
    min = rhs.getMin();
    max = rhs.getMax();
}

cmpEqnParameter::cmpEqnParameter(int id_, double value_, QString name_)
{
    name = name_;
    id = id_;
    value = value_;
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

cmpEqnParameter::~cmpEqnParameter()
{
    clear();
}

void cmpEqnParameter::clear()
{
    name = QString();
    id = 0;
    value = 0;
    min = 0;
    max = 100;
}

bool cmpEqnParameter::isEqual(const cmpEqnParameter &rhs)
{
    bool equal = true;
    if (rhs.getName() != name)
        equal = false;
    else if (rhs.getValue() != value)
        equal = false;
    else if (rhs.getMin() != min)
        equal = false;
    else if (rhs.getMax() != max)
        equal = false;
    return equal;
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

int cmpEqnParameter::getId() const
{
    return id;
}

void cmpEqnParameter::setId(int newId)
{
    id = newId;
}

bool cmpEquation::parseData(cmpFile *cfile, QString type)
{
    bool okay = true, end = false;
    QString token;
    float tmpVal = 0;
    int tmpInt = 0;

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Equation data.");
            okay = false;
        }
        else if (token.compare("parameter", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readInt(tmpInt);
            if (okay)
            {
                if (tmpInt > numEqnParams)
                {
                    cfile->printError(QString("Too many parameters for equation %1").arg(name));
                }
                else
                {
                    okay = cfile->readFloatOrNa(token, tmpVal);
                    if (okay)
                    {
                        setParameter(tmpInt, tmpVal);
                    }
                    else
                    {
                        cfile->incorrectValue(tmpVal, name);
                    }
                }
            }
            else
            {
                cfile->incorrectValue(tmpInt, name);
            }
            if (!okay)
                cfile->skipToEnd();
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            if (type.isEmpty())
                okay = cfile->checkEnd("equation", name);
            else
                okay = cfile->checkEnd("equation", type);
            end = true;
        }
        else
        {
            cfile->unknownToken(token, name);
        }
    }
    return okay;
}

void cmpEquation::writeParameters(cmpFile *outfile, int indent, bool outputAll)
{
    cmpEquation *def = new cmpEquation(id);
    if (outputAll)
    {
        for (int i = 0; i < numEqnParams; i++)
        {
            outfile->writeStringNR(indent, "parameter ");
            outfile->writeValue(0, QString::number(i), getParameter(i)->getValue());
        }
    }
    else
    {
        for (int i = 0; i < numEqnParams; i++)
        {
            outfile->writeStringNR(indent, "parameter ");
            outfile->writeValue(0, QString::number(i), getParameter(i)->getValue(), def->getParameter(i)->getValue());
        }
    }
}

void cmpEquation::setupEquation()
{
    switch (id)
    {
    // linear equations
    default:
    case EQ_LIN0:
        name = QString("LIN0");
        formula = QString("Y = a + b*T");
        numEqnParams = 2;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.00, 0.00, 100.00, "a");
        setParameter(1, 0.00, 0.00,  10.00, "b");
        break;
    case EQ_LIN1:
        name = QString("LIN1");
        formula = QString("Y = a + b*T + e");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.00, 0.00,  20.00, "a");
        setParameter(1, 0.00, 0.00,   5.00, "b");
        setParameter(2, 0.00, 0.00, 100.00, "e");
        break;
    case EQ_LIN2:
        name = QString("LIN2");
        formula = QString("Y = a + b*X + e*X");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.00, 0.00,  20.00, "a");
        setParameter(1, 0.00, 0.00,   5.00, "b");
        setParameter(2, 0.00, 0.00, 100.00, "e");
        break;
    case EQ_LIN3:
        name = QString("LIN3");
        formula = QString("Y = a + b*X + e*(X**2)");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.00, 0.00,  20.00, "a");
        setParameter(1, 0.00, 0.00,   5.00, "b");
        setParameter(2, 0.00, 0.00, 100.00, "e");
        break;

    // exponential equations
    case EQ_EXP0:
        name = QString("EXP0");
        formula = QString("Y = b*exp(a*X + e)");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.05, 0.00,  0.30, "a");
        setParameter(1, 1.00, 0.00,  5.00, "b");
        setParameter(2, 0.25, 0.00,  2.00, "e");
        break;
    case EQ_EXP1:
        name = QString("EXP1");
        formula = QString("Y = b*exp(a*X) + c");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.05, 0.00,  0.30, "a");
        setParameter(1, 1.00, 0.00,  5.00, "b");
        setParameter(2, 0.25, 0.00,  2.00, "c");
        break;
    case EQ_EXP2:
        name = QString("EXP2");
        formula = QString("Y = b*exp(a*X) + e*X");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.05, 0.00,  0.30, "a");
        setParameter(1, 1.00, 0.00,  5.00, "b");
        setParameter(2, 0.03, 0.00,  0.05, "e");
        break;
    case EQ_EXP3:
        name = QString("EXP3");
        formula = QString("Y = b*exp(a*X) + e*(X**2)");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.05, 0.00,  0.30, "a");
        setParameter(1, 1.00, 0.00,  5.00, "b");
        setParameter(2, 0.03, 0.00,  0.05, "e");
        break;

    // power equations
    case EQ_POW0:
        name = QString("POW0");
        formula = QString("Y = b*(X**(a + e))");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 1.40, 0.00,  5.00, "a");
        setParameter(1, 0.17, 0.00,  5.00, "b");
        setParameter(2, 0.005, 0.00,  0.05, "e");
        break;
    case EQ_POW1:
        name = QString("POW1");
        formula = QString("Y = b*(X**a) + e");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 1.40, 0.00,  5.00, "a");
        setParameter(1, 0.17, 0.00,  5.00, "b");
        setParameter(2, 0.005, 0.00,  0.05, "e");
        break;
    case EQ_POW2:
        name = QString("POW2");
        formula = QString("Y = b*((X/100)**a) + e*X");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 1.00, 0.00,  3.00, "a");
        setParameter(1, 1.00, 0.00, 100.00, "b");
        setParameter(2, 0.025, 0.00,  0.50, "e");
        break;
    case EQ_POW3:
        name = QString("POW3");
        formula = QString("Y = b*(X**a) + e*(X**2)");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 1.00, 0.00,  3.00, "a");
        setParameter(1, 1.00, 0.00, 100.00, "b");
        setParameter(2, 0.025, 0.00,  0.50, "e");
        break;

    // application specific equations
    case EQ_NSAT0:
        name = QString("NSAT0");
        formula = QString("Y = b * spill + a * (1 - exp(-k * spill))");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 30.00,  0.00, 50.00, "a"); // gas saturation coeff 1
        setParameter(1,  0.025, 0.00,  0.05, "b"); // gas saturation coeff 2
        setParameter(2,  0.03,  0.00,  0.10, "k"); // gas saturation exponent
        break;
    case EQ_NSAT1:
        name = QString("NSAT1");
        formula = QString("");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  2.469, -2.00,  3.00, "a"); // entrainment 1
        setParameter(1,  1.108,  0.00,  5.00, "b"); // entrainment 2
        setParameter(2, -1.103, -3.00,  2.00, "c"); // entrainment 3
        break;
    case EQ_NSAT2:
        name = QString("NSAT2");
        formula = QString("Y = b * spill + a * (1 - exp(-k * spill))");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  3.31, -10.00, 50.00, "a"); // entrainment 1
        setParameter(1,  0.41,  -2.00,  5.00, "b"); // entrainment 2
        setParameter(2, -0.032,  -2.00,  1.00, "c"); // entrainment 3
        break;
    case EQ_NSAT3:
        name = QString("NSAT3");
        description = QString("Gas Spill 3");
        formula = QString("");
        numEqnParams = 2;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  1.00,  0.00,  5.00, "a"); // entrainment 1
        setParameter(1,  2.00,  0.00,  5.00, "b"); // entrainment 2
        break;
    case EQ_NSAT4:
        name = QString("NSAT4");
        description = QString("Gas Spill 3");
        formula = QString("Y = b * spill + a * spill / (h + spill)");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  30.0,    0.0,   50.00, "a"); // gas saturation coeff 1
        setParameter(1,   0.025,  0.0,    0.12, "b"); // gas saturation coeff 2
        setParameter(2,   6.0,    0.0,  100.00, "h"); // gas saturation exponent
        break;
    case EQ_NSAT5:
        name = QString("NSAT5");
        description = QString("Gas Spill 3");
        formula = QString("Y = b * spill + a * spill / (h + spill)");
        numEqnParams = 4;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   5.0,    0.0,   50.0,  " b"); // y intercept
        setParameter(1,   0.27,   0.0,   10.0,  " m"); // slope
        setParameter(2,   0.0,    0.0,   50.0,  "lb"); // lower bound
        setParameter(3, 400.0,   10.0,  500.0,  "ub"); // upper bound
        break;
    case EQ_NSAT6:
        name = QString("NSAT6");
        description = QString("Bounded Exponential");
        formula = QString();
        numEqnParams = 5;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  50.0, -100.0,  100.0,  " a"); // a
        setParameter(1, -50.0, -100.0,  100.0,  " b"); // b
        setParameter(2, -0.006, -100.0, 100.0,  " c"); // c
        setParameter(3,   0.0, -100.0,  100.0,  "lb"); // lower bound
        setParameter(4, 200.0,    0.0, 2000.0,  "ub"); // upper bound
        break;
    case EQ_NSAT7:
        name = QString("NSAT7");
        description = QString();
        formula = QString("Y = a + b*spill + c*flow + lb*spill*flow");
        numEqnParams = 4;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  38.0, -100.0,  100.0,  " a"); // a
        setParameter(1, -36.0, -100.0,  100.0,  " b"); // b
        setParameter(2,  -0.02, -100.0, 100.0,  " c"); // c
        setParameter(3,   0.0,    0.0,   50.0,  "lb"); // lower bound
        break;
    case EQ_FL_ACT:
        name = QString("FL_ACT");
        formula = QString("Y = a - b * X / 100");
        numEqnParams = 2;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  6.00,  0.00, 100.00, "a");
        setParameter(1, 12.00,  0.00, 200.00, "b");
        break;
    case EQ_LOG0:
        name = QString("LOG0");
        formula = QString("Y = a + b*ln(X) + e");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  0.00,  0.00, 20.00, "a"); //
        setParameter(1,  1.00,  0.00, 20.00, "b"); //
        setParameter(2, 20.00,  0.00, 50.00, "e"); //
        break;
    case EQ_DELAY0:
        name = QString("DELAY0");
        formula = QString("Y = exp(-a * (X**b))");
        numEqnParams = 2;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 20.00,  0.00, 100.00, "a");
        setParameter(1,  5.00,  0.00,  10.00, "b");
        break;
    case EQ_DELAY1:
        name = QString("DELAY1");
        description = QString("Dam Delay Probability");
        formula = QString();
        numEqnParams = 4;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  75.0,    0.0,  250.0, " flow");
        setParameter(1, 100.0,    0.0,  250.0,  "depth");
        setParameter(2, 140.0,    0.0,  365.0,  " day "); //
        setParameter(3,   1.0,    0.5,   10.0,  "scale"); //
        break;
    case EQ_DELAY3:
        name = QString("DELAY3");
        description = QString("Forebay Delay Probability");
        formula = QString();
        numEqnParams = 8;
        numValParams = 4;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.1,    0.005, 100.0,  "A day");
        setParameter(1,   0.2,    0.005, 100.0,  "A night");
        setParameter(2,   0.1,    0.0,     1.0,  "B flow"); //
        setParameter(3,   0.3,    0.0,     2.0,  "B Spill"); //
        setParameter(4,   0.0,    0.0,     2.0,  "B date");
        setParameter(5,   0.0,    0.0,     1.0,  "B RSW");
        setParameter(6,   0.0,    0.0,     1.0,  "B spillprob"); //
        setParameter(7,   0.0,    0.0,    10.0,  "B intercept"); //
        setParameter(8,   0.0,    0.0,   100.0,  "River vel");
        setParameter(9,   0.0,    0.0,     1.0,  "Spill fraction");
        setParameter(10,  0.0,    0.0,   365.0,  "Julian date"); //
        setParameter(11,  0.0,    0.0,     1.0,  "RSW active"); //
        break;
    case EQ_GMORT0:
        name = QString("GMORT0");
        description = QString("Archaic Gas Mort");
        formula = QString();
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.000019,  0.00,  0.001, "mLow");
        setParameter(1, 0.00558,   0.00,  0.01,  " mHi");
        setParameter(2, 20.00,     0.00, 25.00,  "crit"); //
        break;
    case EQ_GMORT1:
        name = QString("GMORT1");
        description = QString("Depth Dependent Gas Mort");
        formula = QString();
        numEqnParams = 4;
        numValParams = 3;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.000019, 0.0,    0.001, "mLow");
        setParameter(1, 0.00558,  0.0,    0.01,  " mHi");
        setParameter(2,  10.9,    5.0,   15.0,  "crit"); //
        setParameter(3,   2.96,   2.0,    4.0,  " mcr"); //
        setParameter(4,  40.0,   20.0,   80.0,  " len"); //
        setParameter(5,  10.0,    1.0,   20.0,  " vel"); // river velocity
        setParameter(6, 100.0,   20.0,  100.0,  "dpth"); // fish depth
        break;
    case EQ_GMORT2:
        name = QString("GMORT2");
        description = QString("Obsolete Density/Depth Dependent Gas Mort");
        formula = QString();
        numEqnParams = 4;
        numValParams = 2;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  0.000019, 0.0,   0.001, "mLow");
        setParameter(1,  0.00558,	0.0,   0.01,  " mHi");
        setParameter(2,  10.9,    5.0,   15.0,  "crit"); //
        setParameter(3,   2.96,   2.0,    4.0,  " mcr"); //
        setParameter(4,  40.0,   20.0,   80.0,  " len"); //
        setParameter(5,  10.0,    1.0,   20.0,  " vel"); // river velocity
        break;
    case EQ_GMORT3:
        name = QString("GMORT3");
        description = QString("Density/Depth Dependent Gas Mort");
        formula = QString();
        numEqnParams = 3;
        numValParams = 5;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0,    0.0,    0.001, "mlow");
        setParameter(1,   0.0,    0.0,    0.02,  " mhi");
        setParameter(2,   0.0,    5.0,   15.0,   "crit"); //
        setParameter(3,   0.0,    0.0,   50.0,   "TDG2"); //
        setParameter(4,   0.5,    0.0,    1.0,   "SplFr"); //
        setParameter(5,   0.075,  0.0,   10.0,   "theta"); //
        setParameter(6,  40.0,    5.0,   80.0,   "Reach length"); //
        setParameter(7,  10.0,    1.0,   50.0,   "River vel"); // river velocity
        break;
    case EQ_FDENS:
        name = QString("FDENS");
        description = QString("Population Density vs Depth");
        formula = QString();
        numEqnParams = 2;
        numValParams = 1;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  39.0,    0.01,  100.0,  "Dmode"); // max density depth
        setParameter(1, 100.0,    0.01,  100.0,  "Dbot");  // max fish depth
        setParameter(2, 100.0,    0.01,  100.0,  "depth"); // fish depth
        break;
    case EQ_DRIFT:
        name = QString("DRIFT");
        description = QString("Fractional Drift Velocity");
        formula = QString();
        numEqnParams = 4;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.000019,  0.00,  0.001, "B0");
        setParameter(1, 0.00558,   0.00,  0.01,  "B1");
        setParameter(2, 20.00,     0.00, 25.00,  " R"); //
        setParameter(3, 20.00,     0.00, 25.00,  " R"); //
        break;
    case EQ_FGE0:
        name = QString("FGE0");
        description = QString("Night FGE");
        formula = QString();
        numEqnParams = 5;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0,    0.0,  100.0,  "B0"); // initial depth
        setParameter(1,   0.0,    0.0,  100.0,  "B1"); // final depth
        setParameter(2,   0.0,    0.0,  100.0,  "B2"); //
        setParameter(3,   0.0,    0.0,  100.0,  "B3"); //
        setParameter(4,   0.0,    0.0,  100.0,  "B4"); // smolt date
        break;
    case EQ_FGE1:
        name = QString("FGE1");
        description = QString();
        formula = QString("Y = exp(B0 + (B1*is_night) + (B2*ph_flow) + (B3*day) + (B4*temp) + (B5*total_flow)");
        numEqnParams = 6;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0,    0.0,  100.0,  "B0"); //
        setParameter(1,   0.0,    0.0,  100.0,  "B1"); //
        setParameter(2,   0.0,    0.0,  100.0,  "B2"); //
        setParameter(3,   0.0,    0.0,  100.0,  "B3"); //
        setParameter(4,   0.0,    0.0,  100.0,  "B4"); //
        setParameter(5,   0.0,    0.0,  500.0,  "B5"); //
        break;
    case EQ_FGE2:
        name = QString("FGE2");
        description = QString();
        formula = QString("Y = probit(B0 + (B1*is_night) + (B2*ph_flow) + (B3*day) + (B4*temp) + (B5*total_flow))");
        numEqnParams = 6;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0,    0.0,  100.0,  "B0"); // intercept
        setParameter(1,   0.0,    0.0,  100.0,  "B1"); // night
        setParameter(2,   0.0,    0.0,  100.0,  "B2"); // powerhouse flow
        setParameter(3,   0.0,    0.0,  100.0,  "B3"); // julian day
        setParameter(4,   0.0,    0.0,  100.0,  "B4"); // water temp
        setParameter(5,   0.0,    0.0,  500.0,  "B5"); // total flow
        break;
    case EQ_MIGR:
        name = QString("MIGR");
        description = QString("Seasonal Flow & exper Migration");
        formula = QString();
        numEqnParams = 6;
        numValParams = 2;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.2,  -10.0,   20.0,  "Bmin"); //
        setParameter(1,  17.0,    0.0,   30.0,  "Bmax"); //
        setParameter(2,   0.5,    0.0,    2.0,  "Bflow"); //
        setParameter(3,   0.2,    0.0,    2.0,  "Alpha2"); //
        setParameter(4, 110.0,   50.0,  250.0,  "Tseas"); //
        setParameter(5,   0.1,    0.0,    1.0,  "Alpha1"); //
        setParameter(6,  10.0,    0.0,   20.0,  "River vel"); //
        setParameter(7, 100.0,    0.0,  300.0,  "Rels date"); //
        break;
    case EQ_MIGR1:
        name = QString("MIGR1");
        description = QString("Constant Migration");
        formula = QString();
        numEqnParams = 1;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   5.0,    0.0,  100.0,  "B0"); //
        break;
    case EQ_MIGR2:
        name = QString("MIGR2");
        description = QString("Seasonal Flow & Exper Migration w/ Length");
        formula = QString();
        numEqnParams = 2;
        numValParams = 1;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   5.0,   -5.0,   50.0,  "B0"); //
        setParameter(1,   0.5,    0.0,    2.0,  "Bflow"); //
        setParameter(2,  10.0,    0.0,  100.0,  "River vel"); //
        break;
    case EQ_MIGR3:
        name = QString("MIGR3");
        description = QString("Linear Migration by Velocity");
        formula = QString();
        numEqnParams = 6;
        numValParams = 2;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.2,  -10.0,   20.0,  "Bmin"); //
        setParameter(1,  17.0,    0.0,   30.0,  "Bmax"); //
        setParameter(2,   0.5,    0.0,    2.0,  "Bflow"); //
        setParameter(3,   0.2,    0.0,    2.0,  "Alpha2"); //
        setParameter(4, 110.0,   50.0,  250.0,  "Tseas"); //
        setParameter(5,   0.1,    0.0,    1.0,  "Alpha1"); //
        setParameter(6,  10.0,    0.0,   20.0,  "River vel"); //
        setParameter(7, 100.0,    0.0,  300.0,  "Rels date"); //
        break;
    case EQ_MIGR4:
        name = QString("MIGR4");
        description = QString("Seasonal Flow Migration");
        formula = QString();
        numEqnParams = 4;
        numValParams = 1;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.2,   -5.0,   50.0,  "B0"); //
        setParameter(1,   0.5,    0.0,    2.0,  "Bflow"); //
        setParameter(2,   0.2,    0.0,    2.0,  "Alpha2"); //
        setParameter(3, 110.0,   50.0,  250.0,  "Tseas"); //
        setParameter(4,  10.0,    0.0,   20.0,  "River vel"); //
        break;
    case EQ_MIGR5:
        name = QString("MIGR5");
        description = QString("Linear Length Migration");
        formula = QString();
        numEqnParams = 2;
        numValParams = 1;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.2,   -5.0,   50.0,  "B0"); //
        setParameter(1,   0.1,    0.0,    1.0,  "Blen"); //
        setParameter(2,   0.0,    0.0,  300.0,  "Fish len"); //
        break;
    case EQ_MIGR6:
        name = QString("MIGR6");
        description = QString("Linear Length + Flow Migration");
        formula = QString();
        numEqnParams = 3;
        numValParams = 2;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.2,   -5.0,   50.0,  "B0"); //
        setParameter(1,   0.1,    0.0,    1.0,  "Blen"); //
        setParameter(2,   0.5,    0.0,    2.0,  "Bflow"); //
        setParameter(3,   0.0,    0.0,  300.0,  "Fish len"); //
        setParameter(4,  10.0,    0.0,  100.0,  "River vel"); //
        break;
    case EQ_MIGR7:
        name = QString("MIGR7");
        description = QString("Linear Temperature Migration");
        formula = QString();
        numEqnParams = 2;
        numValParams = 1;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   5.0,   -5.0,   50.0,  "B0"); //
        setParameter(1,   1.0,   -3.0,    3.0,  "Btemp"); //
        setParameter(2,  15.0,   10.0,   30.0,  "River temp"); //
        break;
    case EQ_MIGR8:
        name = QString("MIGR8");
        description = QString("Linear Flow & Temp Migration");
        formula = QString();
        numEqnParams = 3;
        numValParams = 2;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   5.0,   -5.0,   50.0,  "B0"); //
        setParameter(1,   0.5,    0.0,    2.0,  "Bflow"); //
        setParameter(2,   1.0,   -3.0,    3.0,  "Btemp"); //
        setParameter(3,  10.0,    0.0,  100.0,  "River vel"); //
        setParameter(4,  15.0,   10.0,   30.0,  "River temp"); //
        break;
    case EQ_MIGR9:
        name = QString("MIGR9");
        description = QString("Seasonal Flow & Temp Migration");
        formula = QString();
        numEqnParams = 5;
        numValParams = 1;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.2,   -5.0,   50.0,  "B0"); //
        setParameter(1,   0.5,    0.0,    2.0,  "Bflow"); //
        setParameter(2,   0.0,   -3.0,    3.0,  "Btemp"); //
        setParameter(3,   0.2,    0.0,    2.0,  "Alpha"); //
        setParameter(4, 110.0,   50.0,  250.0,  "Tseas"); //
        setParameter(5,  10.0,    0.0,  100.0,  "River vel"); //
        break;
    case EQ_MIGR10:
        name = QString("MIGR10");
        description = QString("Seasonal Flow, Exper, & Temp Migration");
        formula = QString();
        numEqnParams = 7;
        numValParams = 2;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.2,  -10.0,   20.0,  "Bmin"); //
        setParameter(1,  17.0,    0.0,   30.0,  "Bmax"); //
        setParameter(2,   0.5,    0.0,    2.0,  "Bflow"); //
        setParameter(3,   0.0,   -3.0,    3.0,  "Btemp"); //
        setParameter(4,   0.2,    0.0,    2.0,  "Alpha2"); //
        setParameter(5, 110.0,   50.0,  250.0,  "Tseas"); //
        setParameter(6,   0.1,    0.0,    1.0,  "Alpha1"); //
        setParameter(7,  10.0,    0.0,   20.0,  "River vel"); //
        setParameter(8, 100.0,    0.0,  300.0,  "Rels date"); //
        break;
    case EQ_MIGR12:
        name = QString("MIGR12");
        description = QString("Migration with spill included");
        formula = QString("B0 + Bfspill*pfishspill + Bflow*velocity*(1 / (1 + exp(-alpha*(t - Tinf))) + Btemp*temp + Btemp2*temp*temp");
        numEqnParams = 11;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0,   -3.0,    3.0,  "B0"); //
        setParameter(1,   0.0,   -3.0,    3.0,  "Bfspill"); //
        setParameter(2,   0.0,   -3.0,    3.0,  "Bflow"); //
        setParameter(3,   0.0,   -3.0,    3.0,  "alpha"); //
        setParameter(4,   0.0,   -3.0,    3.0,  "Tinf"); //
        setParameter(5,   0.0,   -3.0,    3.0,  "Btemp"); //
        setParameter(6,   0.0,   -3.0,    3.0,  "Btemp^2"); //
        setParameter(7,   0.0,   -3.0,    3.0,  "Boffset"); //
        setParameter(8,   0.0,   -3.0,    3.0,  "Btemp3"); //
        setParameter(9, 100.0,  -10.0,   20.0,  "C"); //
        setParameter(10,  0.0,    0.0,    1.0,  "Bzspill"); //
        break;
    case EQ_MIGR13:
        name = QString("MIGR13");
        description = QString("Seasonal Flow & Exper Migration w/ Spill parameter");
        formula = QString();
        numEqnParams = 7;
        numValParams = 2;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.2,  -10.0,   20.0,  "Bmin"); //
        setParameter(1,  17.0,    0.0,   30.0,  "Bmax"); //
        setParameter(2,   0.5,    0.0,    2.0,  "Bflow"); //
        setParameter(3,   0.2,    0.0,    2.0,  "Alpha1"); //
        setParameter(4, 110.0,   50.0,  250.0,  "Tseas"); //
        setParameter(5,   0.1,    0.0,    1.0,  "Alpha2"); //
        setParameter(6,   0.0,    0.0,  100.0,  "Bfspill"); //
        setParameter(7,  10.0,    0.0,   20.0,  "River vel"); //
        setParameter(8, 100.0,    0.0,  300.0,  "Rels date"); //
        break;
    case EQ_MIGR14:
        name = QString("MIGR14");
        description = QString("Migration equation with spill included");
        formula = QString("B0 + Bfspill*pfishspill + Bvel*velocity*(1 / (1 + exp(-alpha1*(flow-Qcrit)-alpha2*(day-Dcrit)))");
        numEqnParams = 7;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0,    0.0,    3.0,  "B0"); //
        setParameter(1,   0.0,    0.0,    3.0,  "Bfspill"); //
        setParameter(2,   1.0,    0.0,    3.0,  "Bvel"); //
        setParameter(3,   0.0,   -3.0,    3.0,  "alpha1"); //
        setParameter(4,   0.0,    0.0,  100.0,  "Qcrit"); //
        setParameter(5,   0.0,   -3.0,    3.0,  "alpha2"); //
        setParameter(6,   0.0,    0.0,  365.0,  "Dcrit"); //
        break;
    case EQ_TRANS:
        name = QString("TRANS");
        description = QString("Transport Mortality");
        formula = QString();
        numEqnParams = 4;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  15.7,    0.0,   50.0,  "tt1"); // travel time 1
        setParameter(1,   0.0,    0.0,    0.5,  "m1+"); // delta mort 1
        setParameter(2,  30.0,    0.0,  150.0,  "tt2"); // travel time 2
        setParameter(3,   0.0,    0.0,    0.5,  "m2+"); // delta mort 2
        break;
    case EQ_FLUSHTRANS:
        name = QString("FLUSHTRANS");
        description = QString("FLUSH Transport Merit");
        formula = QString();
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   5.8259, 5.0,    6.0,  "a"); //
        setParameter(1,   5.3533, 5.0,    6.0,  "b"); //
        setParameter(2,   0.98,   0.0,    1.0,  "Vt"); //
        break;
    case EQ_FLUSHTRANS5:
        name = QString("FLUSHTRANS5");
        description = QString("FLUSH TURB5 Transport Merit");
        formula = QString();
        numEqnParams = 2;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.3292, 0.0,    1.0,  "a"); //
        setParameter(1,   0.1868, 0.0,    1.0,  "b"); //
        break;
    case EQ_GAS_DISP:
        name = QString("GAS_DISP");
        description = QString("TDG Saturation > 100% by Distance");
        formula = QString();
        numEqnParams = 1;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 150.0,    0.0,  365.0,  "day"); //
        break;
    case EQ_GAS_DISP_RIGHT:
        name = QString("GAS_DISP_RIGHT");
        description = QString("TDG Saturation > 100% by Distance");
        formula = QString();
        numEqnParams = 1;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 150.0,    0.0,  365.0,  "day"); //
        break;
    case EQ_PRED_TEMP1:
        name = QString("PRED_TEMP1");
        description = QString();
        formula = QString("Y = a * exp(b * T)");
        numEqnParams = 2;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.0,    0.0001,  5.0,  "a"); //
        setParameter(1,   0.207,  0.0001,  2.0,  "b"); //
        break;
    case EQ_PRED_TEMP2:
        name = QString("PRED_TEMP2");
        description = QString();
        formula = QString("Y = C_max / (1 + exp( -alpha * (T - T_inf))");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  63.617,  0.0, 25000.0, "C_max"); //
        setParameter(1,   0.8425, 0.0,   10.0,  "alpha"); //
        setParameter(2,  18.5,    0.0,   50.0,  "T_inf"); //
        break;
    case EQ_SPILL_EFFICIENCY:
        name = QString("SPILL_EFFICIENCY");
        description = QString();
        formula = QString("Y = exp(B0 + B1*log((spill/(1-spill) + B2*flow + B3*flow*log(spill/(1-spill) + B4*(RSW on) + B5*flow*(RSW on))/(1+(same term)) + B6*RSW_flow_prop + B7*RSW_flow_prop*log(spill/(1-spill)");
        numEqnParams = 8;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  14.07,  10.0,   18.0,  "B0"); // constant parameter
        setParameter(1,   0.182,  0.0,    0.3,  "B1"); // logit spill parameter
        setParameter(2,   0.0,    0.0,    1.0,  "B2"); // flow parameter
        setParameter(3,   0.0,    0.0,    1.0,  "B3"); // flow and logit spill interaction
        setParameter(4,   0.0,    0.0,    1.0,  "B4"); // RSW indicator
        setParameter(5,   0.0,    0.0,    1.0,  "B5"); // RSW and flow interaction
        setParameter(6,   0.0,    0.0,    1.0,  "B6"); // RSW proportion of flow
        setParameter(7,   0.0,    0.0,    1.0,  "B7"); // RSW prop and logit spill interaction
        break;
    case EQ_SPILL_EFF_PROBIT:
        name = QString("SPILL_EFF_PROBIT");
        description = QString();
        formula = QString("Y = probit(B0 + B1*probit(spill) + B2*flow + B3*flow*probit(spill) + B4*(RSW on) + B5*flow*(RSW on) + B6*RSW_flow_prop + B7*RSW_flow_prop*probit(spill))");
        numEqnParams = 8;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,  14.07,  10.0,   18.0,  "B0"); // intercept
        setParameter(1,   0.182,  0.0,    0.3,  "B1"); // probit spill parameter
        setParameter(2,   0.0,    0.0,    1.0,  "B2"); // flow parameter
        setParameter(3,   0.0,    0.0,    1.0,  "B3"); // flow and probit spill interaction
        setParameter(4,   0.0,    0.0,    1.0,  "B4"); // RSW indicator
        setParameter(5,   0.0,    0.0,    1.0,  "B5"); // RSW and flow interaction
        setParameter(6,   0.0,    0.0,    1.0,  "B6"); // RSW proportion of flow
        setParameter(7,   0.0,    0.0,    1.0,  "B7"); // RSW prop and probit spill interaction
        break;
    case EQ_SIMPLEMORT5:
        name = QString("SIMPLEMORT5");
        description = QString("Simple TURB5 Mortality");
        formula = QString();
        numEqnParams = 2;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0, 0.00000887, 0.0,  0.001, "A"); //
        setParameter(1, 3.017,      0.0, 10.0,   "B"); //
        break;
    case EQ_RSW:
        name = QString("RSW");
        description = QString("RSW Efficiency");
        formula = QString();
        numEqnParams = 2;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0,    0.0,   10.0,  "B0"); //
        setParameter(1,   0.0,    0.0,    5.0,  "B1"); //
        break;

    case EQ_SURVIVAL_LINEAR:
        name = QString("SURVIVAL_LINEAR");
        description = QString("Linear Survival");
        formula = QString("Y = a + b*theta_hat + c*X + d*T");
        numEqnParams = 4;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0,  -10.0,   10.0,  "a"); //
        setParameter(1,   0.0,  -10.0,   10.0,  "b"); //
        setParameter(2,   0.0,  -10.0,   10.0,  "c"); //
        setParameter(3,   0.0,  -10.0,   10.0,  "d"); //
        break;
    case EQ_SURVIVAL_Z15:
        name = QString("SURVIVAL_Z15");
        description = QString("Linear Survival");
        formula = QString("Y = exp(-((a0 + a1*F + a2*F^2 + a3*T + a4*T^2 + a5*S + a6*S^2)*x + (B0 + B1*F + B2*F^2 + B3*T + B4*T^2 + B5*S + B6*S^2)*t + r*x*t))");
        numEqnParams = 15;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter( 0,   0.0,  -10.0,   10.0,  "a0"); //
        setParameter( 1,   0.0,  -10.0,   10.0,  "a1"); //
        setParameter( 2,   0.0,  -10.0,   10.0,  "a2"); //
        setParameter( 3,   0.0,  -10.0,   10.0,  "a3"); //
        setParameter( 4,   0.0,  -10.0,   10.0,  "a4"); //
        setParameter( 5,   0.0,  -10.0,   10.0,  "a5"); //
        setParameter( 6,   0.0,  -10.0,   10.0,  "a6"); //
        setParameter( 7,   0.0,  -10.0,   10.0,  "B0"); //
        setParameter( 8,   0.0,  -10.0,   10.0,  "B1"); //
        setParameter( 9,   0.0,  -10.0,   10.0,  "B2"); //
        setParameter(10,   0.0,  -10.0,   10.0,  "B3"); //
        setParameter(11,   0.0,  -10.0,   10.0,  "B4"); //
        setParameter(12,   0.0,  -10.0,   10.0,  "B5"); //
        setParameter(13,   0.0,  -10.0,   10.0,  "B6"); //
        setParameter(14,   0.0,  -10.0,   10.0,  " r"); //
        break;
    case EQ_SURVIVAL_Z15_PRED_1:
        name = QString("SURVIVAL_Z15_PRED_1");
        description = QString();
        formula = QString("Y = exp(-((a0 + a1*F + a2*F^2 + a3*T + a4*T^2 + a5*S + a6*S^2)*x + (B0 + B1*F + B2*F^2 + B3*T + B4*T^2 + B5*S + B6*S^2)*t + r*x*t))");
        numEqnParams = 15;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter( 0,   0.0,  -10.0,   10.0,  "a0"); //
        setParameter( 1,   0.0,  -10.0,   10.0,  "a1"); //
        setParameter( 2,   0.0,  -10.0,   10.0,  "a2"); //
        setParameter( 3,   0.0,  -10.0,   10.0,  "a3"); //
        setParameter( 4,   0.0,  -10.0,   10.0,  "a4"); //
        setParameter( 5,   0.0,  -10.0,   10.0,  "a5"); //
        setParameter( 6,   0.0,  -10.0,   10.0,  "a6"); //
        setParameter( 7,   0.0,  -10.0,   10.0,  "B0"); //
        setParameter( 8,   0.0,  -10.0,   10.0,  "B1"); //
        setParameter( 9,   0.0,  -10.0,   10.0,  "B2"); //
        setParameter(10,   0.0,  -10.0,   10.0,  "B3"); //
        setParameter(11,   0.0,  -10.0,   10.0,  "B4"); //
        setParameter(12,   0.0,  -10.0,   10.0,  "B5"); //
        setParameter(13,   0.0,  -10.0,   10.0,  "B6"); //
        setParameter(14,   0.0,  -10.0,   10.0,  " r"); //
        break;
    case EQ_SURVIVAL_Z15_PRED_2:
        name = QString("SURVIVAL_Z15_PRED_2");
        description = QString();
        formula = QString("Y = exp(-(a0 + a1*F + a2*F^2 + a3*T + a4*T^2 + a5*S + a6*S^2)*x + (B0 + B1*F + B2*F^2 + B3*T + B4*T^2 + B5*S + B6*S^2)*t + r*x*t))");
        numEqnParams = 15;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter( 0,-999999.0, -10.0,  10.0,  "phi"); // phi    - distance intercept
        setParameter( 1,-999999.0, -10.0,  10.0,  "B1");  // Beta1  - travel time intercept
        setParameter( 2,   0.0,  -10.0,   10.0,  "B2");   // Beta2  - flow
        setParameter( 3,   0.0,  -10.0,   10.0,  "B3");   // Beta3  - flow^2
        setParameter( 4,   0.0,  -10.0,   10.0,  "B4");   // Beta4  - temperature
        setParameter( 5,   0.0,  -10.0,   10.0,  "B5");   // Beta5  - temperature^2
        setParameter( 6,   0.0,  -10.0,   10.0,  "B6");   // Beta6  - fish spill
        setParameter( 7,   0.0,  -10.0,   10.0,  "B7");   // Beta7  - (fish spill)^2
        setParameter( 8,-999999.0, -10.0,  10.0,  "a1");  // alpha1 - predation rate bird 1
        setParameter( 9,-100.0,  -10.0,   10.0,  "D1");   // Delta1 - half saturation bird 1
        setParameter(10,-999999.0, -10.0,  10.0,  "a2");  // alpha2 - predation rate bird 2
        setParameter(11,-100.0,  -10.0,   10.0,  "D2");   // Delta2 - half saturation bird 2
        setParameter(12,   0.0,  -10.0,   10.0,  "B8");   // Beta8  - temp change in slope param at threshold
        setParameter(13, 100.0,  -10.0,   10.0,  "C");    // C      - temp threshold value
        setParameter(14,   0.0,  -10.0,   10.0,  "B9");   // Beta9  - proportion zero spill
        break;
    case EQ_SURVIVAL_Z15_PRED_3:
        name = QString("SURVIVAL_Z15_PRED_3");
        description = QString();
        formula = QString("Y = exp(-((a0 + a1*F + a2*F^2 + a3*T + a4*T^2 + a5*S + a6*S^2)*x + (B0 + B1*F + B2*F^2 + B3*T + B4*T^2 + B5*S + B6*S^2)*t + r*x*t))");
        numEqnParams = 15;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter( 0,-999999.0, -10.0,  10.0,  "phi"); // phi    - distance intercept
        setParameter( 1,-999999.0, -10.0,  10.0,  "B1");  // Beta1  - travel time intercept
        setParameter( 2,   0.0,  -10.0,   10.0,  "B2");   // Beta2  - flow
        setParameter( 3,   0.0,  -10.0,   10.0,  "B3");   // Beta3  - flow^2
        setParameter( 4,   0.0,  -10.0,   10.0,  "B4");   // Beta4  - temperature
        setParameter( 5,   0.0,  -10.0,   10.0,  "B5");   // Beta5  - temperature^2
        setParameter( 6,   0.0,  -10.0,   10.0,  "B6");   // Beta6  - fish spill
        setParameter( 7,   0.0,  -10.0,   10.0,  "B7");   // Beta7  - (fish spill)^2
        setParameter( 8,-999999.0, -10.0,  10.0,  "a1");  // alpha1 - predation rate bird 1
        setParameter( 9,-100.0,  -10.0,   10.0,  "D1");   // Delta1 - half saturation bird 1
        setParameter(10,-999999.0, -10.0,  10.0,  "a2");  // alpha2 - predation rate bird 2
        setParameter(11,-100.0,  -10.0,   10.0,  "D2");   // Delta2 - half saturation bird 2
        setParameter(12,   0.0,  -10.0,   10.0,  "B8");   // Beta8  - temp change in slope param at threshold
        setParameter(13, 100.0,  -10.0,   10.0,  "C");    // C      - temp threshold value
        setParameter(14,   0.0,  -10.0,   10.0,  "B9");   // Beta9  - proportion zero spill
        break;
    case EQ_SURVIVAL_XT_1:
        name = QString("SURV_XT_1");
        description = QString("Survival");
        formula = QString("Y = exp(-B0*sqrt(length^2 + B1*exp(-B2*flow)*days^2))");
        numEqnParams = 6;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.010,   0.0,   2.0,  "B0"); // intercept
        setParameter(1,   1.0,     0.0, 400.0,  "B1"); // flow-time scaling
        setParameter(2,   0.010,   0.0,   1.0,  "B2"); // flow
        setParameter(3,  40.0,    5.0,   80.0,  "Reach length"); //
        setParameter(4,  10.0,    1.0,   50.0,  "Flow"); //
        setParameter(5,   1.0,    0.5,    2.0,  "Days"); //
        break;

    case EQ_SWITCH0:
        name = QString("SWITCH");
        description = QString("Switch between day and night values");
        formula = QString("Y = is_night(t)? p2: p1");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0,   0.0,   1.0,  "t"); // time - to determine if is-night
        setParameter(1,   0.5,   0.0,   1.0,  "p1"); // p1  - day time value
        setParameter(2,   0.5,   0.0,   1.0,  "p2"); // p2  - night time value
        break;

    case EQ_RETURN_LOG:
        name = QString("RETURN_LOG");
        description = QString("Return probability");
        formula = QString("exp(B0+B1*day+B2*day^2)/1+exp(B0+B1*day+B2*day^2)");
        numEqnParams = 3;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0, -10.0,  10.0,  "B0");
        setParameter(1,   0.0, -10.0,  10.0,  "B1");
        setParameter(2,   0.0, -10.0,  10.0,  "B2");
        break;
    case EQ_LATENT_MORT_WTT:
        name = QString("LATENT_MORT_WTT");
        description = QString("Latent Mortality (S3 vs WTT)");
        formula = QString("Y = 1.0 - exp(-b*(WTT - 2))");
        numEqnParams = 1;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   0.0, -100.0,  100.0,  "b"); //
        break;

    // cubic equations
    case EQ_CUB0:
        name = QString("CUB0");
        description = QString();
        formula = QString("Y = a*X + b*(X**2) + C*(X**3) + e");
        numEqnParams = 4;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   1.0,    0.0,   50.0,  "a"); //
        setParameter(1,   1.0,    0.0,   50.0,  "b"); //
        setParameter(2,   1.0,    0.0,   50.0,  "c"); //
        setParameter(3,  70.0,    0.0,  100.0,  "e"); //
        break;
    case EQ_CUB1:
        name = QString("CUB1");
        description = QString();
        formula = QString("Y = (a*X/100 + b*(X/100)^2 + c*(X/100)^3)*100 + e");
        numEqnParams = 4;
        numValParams = 0;
        setNumParameters(numEqnParams + numValParams);
        setParameter(0,   2.583, -5.0,    5.0,  "a"); //
        setParameter(1,  -3.250, -5.0,    5.0,  "b"); //
        setParameter(2,   1.667, -5.0,    5.0,  "c"); //
        setParameter(3,  70.0,    0.0,  100.0,  "e"); //
        break;

    }
}

void setEquationNames()
{
    equationNames.append("LIN0");
    equationNames.append("LIN1");
    equationNames.append("LIN2");
    equationNames.append("LIN3");
    equationNames.append("EXP0");
    equationNames.append("EXP1");
    equationNames.append("EXP2");
    equationNames.append("EXP3");
    equationNames.append("POW0");
    equationNames.append("POW1");
    equationNames.append("POW2");
    equationNames.append("POW3");
    equationNames.append("NSAT0");
    equationNames.append("FL_ACT");
    equationNames.append("DELAY0");
    equationNames.append("NSAT3");
    equationNames.append("LOG0");
    equationNames.append("NSAT1");
    equationNames.append("NSAT2");
    equationNames.append("GMORT0");
    equationNames.append("DRIFT");
    equationNames.append("DELAY1");
    equationNames.append("FGE0");
    equationNames.append("NSAT4");
    equationNames.append("MIGR");
    equationNames.append("GMORT1");
    equationNames.append("GMORT2");
    equationNames.append("FDENS");
    equationNames.append("TRANS");
    equationNames.append("NSAT5");
    equationNames.append("NSAT6");
    equationNames.append("GAS_DISP");
    equationNames.append("GAS_DISP_RIGHT");  // Unused
    equationNames.append("MIGR2");
    equationNames.append("GMORT3");
    equationNames.append("CUB0");
    equationNames.append("CUB1");
    equationNames.append("PRED_TEMP1");
    equationNames.append("PRED_TEMP2");
    equationNames.append("MIGR1");
    equationNames.append("MIGR3");
    equationNames.append("MIGR4");
    equationNames.append("MIGR5");
    equationNames.append("MIGR6");
    equationNames.append("SPILL_EFFICIENCY");
    equationNames.append("FLUSHTRANS");  // Unused
    equationNames.append("DELAY3");
    equationNames.append("MIGR7");
    equationNames.append("MIGR8");
    equationNames.append("MIGR9");
    equationNames.append("MIGR10");
    equationNames.append("RETURN_LOG");
    equationNames.append("MIGR11");
    equationNames.append("SIMPLEMORT5");  // Unused
    equationNames.append("FLUSHTRANS5");  // Unused
    equationNames.append("RSW");
    equationNames.append("SURVIVAL_LINEAR");
    equationNames.append("SURVIVAL_Z15");
    equationNames.append("LATENT_MORT_WTT");
    equationNames.append("MIGR12");
    equationNames.append("MIGR13");
    equationNames.append("FGE1");
    equationNames.append("NSAT7");
    equationNames.append("SURVIVAL_Z15_PRED_1");
    equationNames.append("SPILL_EFF_PROBIT");
    equationNames.append("FGE2");
    equationNames.append("SURVIVAL_Z15_PRED_2");
    equationNames.append("SURVIVAL_Z15_PRED_3");
    equationNames.append("SWITCH0");
    equationNames.append("MIGR14");
    equationNames.append("SURVIVAL_XT_1");
}

