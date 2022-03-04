#include "cmpstock.h"

#include "cmprunsettings.h"

cmpStock::cmpStock()
{
    inriverReturnEqn = new cmpEquation();
    transportReturnEqn = new cmpEquation();
    allocate(732, 2);
    setDefaults();
}

cmpStock::~cmpStock()
{
    cmpEquation *cmpeqn = nullptr;
    mvCoef.clear();
    distanceCoeff.clear();
    timeCoeff.clear();
    sigmaD.clear();
    procStdDev.clear();
    vvar.clear();
    while (migrB1Factor.count() > 0)
    {
        migrB1Factor.last().clear();
        migrB1Factor.takeLast();
        cmpeqn = migrationEqn.takeLast();
        delete cmpeqn;
        cmpeqn = customSurvivalEqn.takeLast();
        delete cmpeqn;
        cmpeqn = copySurvivalEqn.takeLast();
        delete cmpeqn;
        cmpMonteCarloMulti *cmpmc = covmat.takeLast();
        delete cmpmc;
    }
}

void cmpStock::allocate(int steps, int numReachClasses)
{
    while (mvCoef.count() < numReachClasses)
    {
        migrationEqn.append(new cmpEquation());
        mvCoef.append(0);
        distanceCoeff.append(0);
        timeCoeff.append(0);
        sigmaD.append(0);
        procStdDev.append(0);
        vvar.append(0);
        customSurvivalEqn.append(new cmpEquation(EQ_SURVIVAL_Z15));
        reachSurvivalCoef.append(0);
        covmat.append(new cmpMonteCarloMulti());
        copySurvivalEqn.append(new cmpEquation(EQ_SURVIVAL_Z15));
        for (int i = 0; i < steps; i++)
            migrB1Factor[i].append(0);
    }
    while (mvCoef.count() > numReachClasses)
    {
        cmpEquation *cmpeqn = nullptr;
        mvCoef.takeLast();
        distanceCoeff.takeLast();
        timeCoeff.takeLast();
        sigmaD.takeLast();
        procStdDev.takeLast();
        vvar.takeLast();
        migrB1Factor.last().clear();
        migrB1Factor.takeLast();
        cmpeqn = migrationEqn.takeLast();
        delete cmpeqn;
        cmpeqn = customSurvivalEqn.takeLast();
        delete cmpeqn;
        cmpeqn = copySurvivalEqn.takeLast();
        delete cmpeqn;
        cmpMonteCarloMulti *cmpmc = covmat.takeLast();
        delete cmpmc;
    }
}

void cmpStock::setDefaults()
{
    int steps = 2;
    int rc = mvCoef.count();
    for (int i = 0; i < rc; i++)
    {
        mvCoef[i] = 0;
        distanceCoeff[i] = 0;
        timeCoeff[i] = 0;
        sigmaD[i] = 0;
        procStdDev[i] = 0;
        vvar[i] = 0;
        migrationEqn[i] = new cmpEquation();
        customSurvivalEqn[i] = new cmpEquation(EQ_SURVIVAL_Z15);
        copySurvivalEqn[i] = new cmpEquation(*customSurvivalEqn[i]);
        covmat[i] = new cmpMonteCarloMulti();
        for (int j = 0; j < steps; j++)
            migrB1Factor[i][j] = 0;
    }
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
    return migrationEqn[rc];
}

void cmpStock::setMigrationEqn(int rc, cmpEquation *newMigrationEqn)
{
    migrationEqn[rc] = newMigrationEqn;
}

float cmpStock::getMvCoef(int rc) const
{
    return mvCoef[rc];
}

void cmpStock::setMvCoef(int rc, const float newMvCoef)
{
    mvCoef[rc] = newMvCoef;
}

float cmpStock::getDistanceCoeff(int rc) const
{
    return distanceCoeff[rc];
}

void cmpStock::setDistanceCoeff(int rc, float newDistanceCoeff)
{
    distanceCoeff[rc] = newDistanceCoeff;
}

float cmpStock::getTimeCoeff(int rc) const
{
    return timeCoeff[rc];
}

void cmpStock::setTimeCoeff(int rc, float newTimeCoeff)
{
    timeCoeff[rc] = newTimeCoeff;
}

float cmpStock::getSigmaD(int rc) const
{
    return sigmaD[rc];
}

void cmpStock::setSigmaD(int rc, float newSigmaD)
{
    sigmaD[rc] = newSigmaD;
}

float cmpStock::getProcStdDev(int rc) const
{
    return procStdDev[rc];
}

void cmpStock::setProcStdDev(int rc, float newProcStdDev)
{
    procStdDev[rc] = newProcStdDev;
}

float cmpStock::getMigrB1Factor(int rc, int step) const
{
    return migrB1Factor[rc][step];
}

void cmpStock::setMigrB1Factor(int rc, int step, float newMigrB1Factor)
{
    migrB1Factor[rc][step] = newMigrB1Factor;
}

float cmpStock::getVvar(int rc) const
{
    return vvar[rc];
}

void cmpStock::setVvar(int rc, float newVvar)
{
    vvar[rc] = newVvar;
}

cmpEquation *cmpStock::getCustomSurvivalEqn(int rc) const
{
    return customSurvivalEqn[rc];
}

void cmpStock::setCustomSurvivalEqn(int rc, cmpEquation *newCustomSurvivalEqn)
{
    customSurvivalEqn[rc] = newCustomSurvivalEqn;
}

cmpEquation *cmpStock::getInriverReturnEqn() const
{
    return inriverReturnEqn;
}

void cmpStock::setInriverReturnEqn(cmpEquation *newInriverReturnEqn)
{
    inriverReturnEqn = newInriverReturnEqn;
}

cmpEquation *cmpStock::getTransportReturnEqn() const
{
    return transportReturnEqn;
}

void cmpStock::setTransportReturnEqn(cmpEquation *newTransportReturnEqn)
{
    transportReturnEqn = newTransportReturnEqn;
}

float cmpStock::getReachSurvivalCoef(int rc) const
{
    return reachSurvivalCoef[rc];
}

void cmpStock::setReachSurvivalCoef(int rc, float newReachSurvivalCoef)
{
    reachSurvivalCoef[rc] = newReachSurvivalCoef;
}

const cmpMonteCarloMulti *cmpStock::getCovmat(int rc) const
{
    return covmat[rc];
}

void cmpStock::setCovmat(int rc, cmpMonteCarloMulti *newCovmat)
{
    covmat[rc] = newCovmat;
}
