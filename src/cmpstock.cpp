#include "cmpstock.h"

#include "cmpdatasettings.h"

cmpStock::cmpStock()
{
    name = new QString ("generic");
    inriverReturnEqn = new cmpEquation();
    transportReturnEqn = new cmpEquation();
    reachClasses.append(new cmpReachClass("0"));
    allocate(732, 1);

}

cmpStock::~cmpStock()
{
    delete name;
    delete inriverReturnEqn;
    delete transportReturnEqn;
    while (reachClasses.count() > 0)
        delete reachClasses.takeLast();
}

void cmpStock::allocate(int steps, int numReachClasses)
{
    for (int i = 0; i < numReachClasses; i++)
        reachClasses.at(i)->allocate(steps);
}

void cmpStock::setDefaults()
{
    for (int i = 0; i < reachClasses.count(); i++)
        reachClasses.at(i)->setDefaults();

}


QString *cmpStock::getName() const
{
    return name;
}

void cmpStock::setName(QString *newName)
{
    name = newName;
}

const cmpEquation *cmpStock::getMigrationEqn(int rc) const
{
    return reachClasses[rc]->getMigrationEqn();
}

void cmpStock::setMigrationEqn(int rc, cmpEquation *newMigrationEqn)
{
    reachClasses[rc]->setMigrationEqn(newMigrationEqn);
}

float cmpStock::getMvCoef(int rc) const
{
    return reachClasses.at(rc)->getMvCoef();
}

void cmpStock::setMvCoef(int rc, const float newMvCoef)
{
    reachClasses.at(rc)->setMvCoef(newMvCoef);
}

float cmpStock::getDistanceCoeff(int rc) const
{
    return reachClasses.at(rc)->getDistanceCoeff();
}

void cmpStock::setDistanceCoeff(int rc, float newDistanceCoeff)
{
    reachClasses.at(rc)->setDistanceCoeff(newDistanceCoeff);
}

float cmpStock::getTimeCoeff(int rc) const
{
    return reachClasses.at(rc)->getTimeCoeff();
}

void cmpStock::setTimeCoeff(int rc, float newTimeCoeff)
{
    reachClasses.at(rc)->setTimeCoeff(newTimeCoeff);
}

float cmpStock::getSigmaD(int rc) const
{
    return reachClasses.at(rc)->getSigmaD();
}

void cmpStock::setSigmaD(int rc, float newSigmaD)
{
    reachClasses.at(rc)->setSigmaD(newSigmaD);
}

float cmpStock::getProcStdDev(int rc) const
{
    return reachClasses.at(rc)->getProcStdDev();
}

void cmpStock::setProcStdDev(int rc, float newProcStdDev)
{
    reachClasses.at(rc)->setProcStdDev(newProcStdDev);
}

float cmpStock::getMigrB1Factor(int rc, int step) const
{
    return reachClasses.at(rc)->getMigrB1Factor(step);
}

void cmpStock::setMigrB1Factor(int rc, int step, float newMigrB1Factor)
{
    reachClasses.at(rc)->setMigrB1Factor(step, newMigrB1Factor);
}

float cmpStock::getVvar(int rc) const
{
    return reachClasses.at(rc)->getVvar();
}

void cmpStock::setVvar(int rc, float newVvar)
{
    reachClasses.at(rc)->setVvar(newVvar);
}

cmpEquation *cmpStock::getCustomSurvivalEqn(int rc) const
{
    return reachClasses.at(rc)->getCustomSurvivalEqn();
}

void cmpStock::setCustomSurvivalEqn(int rc, cmpEquation *newCustomSurvivalEqn)
{
    reachClasses.at(rc)->setCustomSurvivalEqn(newCustomSurvivalEqn);
}

float cmpStock::getReachSurvivalCoef(int rc) const
{
    return reachClasses.at(rc)->getReachSurvivalCoef();
}

void cmpStock::setReachSurvivalCoef(int rc, float newReachSurvivalCoef)
{
   reachClasses.at(rc)->setReachSurvivalCoef(newReachSurvivalCoef);
}

const cmpMonteCarloMulti *cmpStock::getCovmat(int rc) const
{
    return reachClasses.at(rc)->getCovmat();
}

void cmpStock::setCovmat(int rc, cmpMonteCarloMulti *newCovmat)
{
    reachClasses.at(rc)->setCovmat(newCovmat);
}
