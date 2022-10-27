#include "cmpreachclass.h"

cmpReachClass::cmpReachClass(const QString &newName)
{
    name = new QString(newName);
    migrationEqn = nullptr;
    customSurvivalEqn = nullptr;
    copySurvivalEqn = nullptr;
    covmat = nullptr;
    setDefaults();
}

cmpReachClass::~cmpReachClass()
{
    delete name;
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
}

void cmpReachClass::setDefaults()
{
    deleteEqns();
    mvCoef = 0;
    distanceCoeff = 0;
    timeCoeff = 0;
    sigmaD = 0;
    procStdDev = 0;
    vvar = 0;

    migrationEqn = new cmpEquation();
    customSurvivalEqn = new cmpEquation(EQ_SURVIVAL_Z15);
    copySurvivalEqn = new cmpEquation(*customSurvivalEqn);
    covmat = new cmpMonteCarloMulti();

    allocate(migrB1Factor.count());
}

void cmpReachClass::allocate(int steps)
{
    while (migrB1Factor.count() < steps)
        migrB1Factor.append(0);
    while (migrB1Factor.count() > steps)
        migrB1Factor.takeLast();
    for (int i = 0; i < migrB1Factor.count(); i++)
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

float cmpReachClass::getMvCoef() const
{
    return mvCoef;
}

void cmpReachClass::setMvCoef(float newMvCoef)
{
    mvCoef = newMvCoef;
}

float cmpReachClass::getDistanceCoeff() const
{
    return distanceCoeff;
}

void cmpReachClass::setDistanceCoeff(float newDistanceCoeff)
{
    distanceCoeff = newDistanceCoeff;
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
    return vvar;
}

void cmpReachClass::setVvar(float newVvar)
{
    vvar = newVvar;
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
    customSurvivalEqn = newCustomSurvivalEqn;
    copySurvivalEqn = customSurvivalEqn;
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
    covmat = newCovmat;
}

cmpEquation *cmpReachClass::getCopySurvivalEqn() const
{
    return copySurvivalEqn;
}

void cmpReachClass::setCopySurvivalEqn(cmpEquation *newCopySurvivalEqn)
{
    copySurvivalEqn = newCopySurvivalEqn;
}

QString *cmpReachClass::getName() const
{
    return name;
}

void cmpReachClass::setName(QString *newName)
{
    name = newName;
}
