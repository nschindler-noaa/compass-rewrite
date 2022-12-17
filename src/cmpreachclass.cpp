#include "cmpreachclass.h"

cmpReachClass::cmpReachClass(const QString &newName)
{
    if (!newName.isEmpty())
        name = QString(newName);
    else
        name = QString("class_0");
    migrationEqn = nullptr;
    customSurvivalEqn = nullptr;
    copySurvivalEqn = nullptr;
    covmat = nullptr;
    setDefaults();
}

cmpReachClass::~cmpReachClass()
{
    deleteEqns();
}

void cmpReachClass::deleteEqns()
{
    if (migrationEqn != nullptr)
        delete migrationEqn;
    if (customSurvivalEqn != nullptr)
        delete customSurvivalEqn;
    if (copySurvivalEqn != nullptr)
        delete copySurvivalEqn;
    if (covmat != nullptr)
        delete covmat;
    migrationEqn = nullptr;
    customSurvivalEqn = nullptr;
    copySurvivalEqn = nullptr;
    covmat = nullptr;
}

void cmpReachClass::copy(const cmpReachClass &rhs)
{

}

const QString &cmpReachClass::getName() const
{
    return name;
}

void cmpReachClass::setName(QString &newName)
{
    name = newName;
}

void cmpReachClass::setDefaults()
{
    deleteEqns();
    migrVarCoef = 0;
    distCoeff = 0;
    timeCoeff = 0;
    sigmaD = 0;
    procStdDev = 0;
    vVar = 0;
    reachSurvivalCoef = 1;
    reachPredCoef = 0;

    pprimeA = 0;
    pprimeB = 0;

    migrationEqn = new cmpEquation();
    customSurvivalEqn = new cmpEquation(EQ_SURVIVAL_Z15);
    copySurvivalEqn = new cmpEquation(*customSurvivalEqn);
    covmat = new cmpMonteCarloMulti();

    allocate(732);
}

void cmpReachClass::allocate(int steps)
{
    while (migrB1Factor.count() < steps)
        migrB1Factor.append(0);
    while (migrB1Factor.count() > steps)
        migrB1Factor.takeLast();
    for (int i = 0; i < steps; i++)
        migrB1Factor[i] = 0;
}

cmpEquation *cmpReachClass::getMigrationEqn() const
{
    return migrationEqn;
}

void cmpReachClass::setMigrationEqn(cmpEquation *newMigrationEqn)
{
    migrationEqn = newMigrationEqn;
}

float cmpReachClass::getMigrVarCoef() const
{
    return migrVarCoef;
}

void cmpReachClass::setMigrVarCoef(float newMvCoef)
{
    migrVarCoef = newMvCoef;
}

float cmpReachClass::getDistCoeff() const
{
    return distCoeff;
}

void cmpReachClass::setDistCoeff(float newDistanceCoeff)
{
    distCoeff = newDistanceCoeff;
}

float cmpReachClass::getTimeCoeff() const
{
    return timeCoeff;
}

void cmpReachClass::setTimeCoeff(float newTimeCoeff)
{
    timeCoeff = newTimeCoeff;
}

float cmpReachClass::getSigmaD() const
{
    return sigmaD;
}

void cmpReachClass::setSigmaD(float newSigmaD)
{
    sigmaD = newSigmaD;
}

float cmpReachClass::getProcStdDev() const
{
    return procStdDev;
}

void cmpReachClass::setProcStdDev(float newProcStdDev)
{
    procStdDev = newProcStdDev;
}

float cmpReachClass::getVvar() const
{
    return vVar;
}

void cmpReachClass::setVvar(float newVvar)
{
    vVar = newVvar;
}

float cmpReachClass::getMigrB1Factor(int index) const
{
    return migrB1Factor.at(index);
}

void cmpReachClass::setMigrB1Factor(int index, float value)
{
    while (index >= migrB1Factor.count())
        migrB1Factor.append(1);
    migrB1Factor[index] = value;
}

const QList<float> &cmpReachClass::getMigrB1Factors() const
{
    return migrB1Factor;
}

void cmpReachClass::setMigrB1Factors(const QList<float> &newMigrB1Factor)
{
    migrB1Factor = newMigrB1Factor;
}

cmpEquation *cmpReachClass::getCustomSurvivalEqn() const
{
    return customSurvivalEqn;
}

void cmpReachClass::setCustomSurvivalEqn(cmpEquation *newCustomSurvivalEqn)
{
    if (customSurvivalEqn != nullptr)
        delete customSurvivalEqn;
    customSurvivalEqn = newCustomSurvivalEqn;
    copySurvivalEqn = new cmpEquation(*customSurvivalEqn);
}

float cmpReachClass::getReachSurvivalCoef() const
{
    return reachSurvivalCoef;
}

void cmpReachClass::setReachSurvivalCoef(float newReachSurvivalCoef)
{
    reachSurvivalCoef = newReachSurvivalCoef;
}

cmpMonteCarloMulti *cmpReachClass::getCovmat() const
{
    return covmat;
}

void cmpReachClass::setCovmat(cmpMonteCarloMulti *newCovmat)
{
    if (covmat != nullptr)
        delete covmat;
    covmat = newCovmat;
}

cmpEquation *cmpReachClass::getCopySurvivalEqn() const
{
    return copySurvivalEqn;
}

void cmpReachClass::setCopySurvivalEqn(cmpEquation *newCopySurvivalEqn)
{
    if (copySurvivalEqn != nullptr)
        delete copySurvivalEqn;
    copySurvivalEqn = new cmpEquation(*newCopySurvivalEqn);
}

float cmpReachClass::getReachPredCoef() const
{
    return reachPredCoef;
}

void cmpReachClass::setReachPredCoef(float newReachPredCoef)
{
    reachPredCoef = newReachPredCoef;
}

float cmpReachClass::getPprimeA() const
{
    return pprimeA;
}

void cmpReachClass::setPprimeA(float newPprimeA)
{
    pprimeA = newPprimeA;
}

float cmpReachClass::getPprimeB() const
{
    return pprimeB;
}

void cmpReachClass::setPprimeB(float newPprimeB)
{
    pprimeB = newPprimeB;
}
