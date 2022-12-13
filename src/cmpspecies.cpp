#include "cmpspecies.h"

cmpSpecies::cmpSpecies()
{
    name = QString();
    allocate(2);
    setDefaults();
}

const QString &cmpSpecies::getName() const
{
    return name;
}

void cmpSpecies::setName(const QString &newName)
{
    name = newName;
}

void cmpSpecies::allocate(int numReachClasses)
{
    while (reachPredCoef.count() < numReachClasses)
    {
        reachPredCoef.append(0);
        pprimeA.append(0);
        pprimeB.append(0);
    }
    while (reachPredCoef.count() > numReachClasses)
    {
        reachPredCoef.takeLast();
        pprimeA.takeLast();
        pprimeB.takeLast();
    }
}

void cmpSpecies::setDefaults()
{
    for (int i = 0; i < reachPredCoef.count(); i++)
    {
        reachPredCoef[i] = 0;
        pprimeA[i] = 0;
        pprimeB[i] = 0;
    }
    tailracePredCoef = 0;
    forebayPredCoef = 0;

    gasmortEqn = new cmpEquation(EQ_GMORT3);
    fishdensEqn = new cmpEquation(EQ_FDENS);

    inriverLatentMort = 0;
    transportLatentMort = 0;
    differentialReturn = 1;
    inriverLatentMortEqn = new cmpEquation(EQ_LATENT_MORT_WTT);
    inriverReturnEqn = new cmpEquation(51);
    transportReturnEqn = new cmpEquation(51);
}

bool cmpSpecies::parseData(cmpFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Stock data.");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->checkEnd("reach", name);
            end = true;
        }
        else
        {
            cfile->unknownToken(token, name);
        }
    }

    return okay;
}

const QList<float> &cmpSpecies::getReachPredCoef() const
{
    return reachPredCoef;
}

void cmpSpecies::setReachPredCoef(const QList<float> &newReachPredCoef)
{
    reachPredCoef = newReachPredCoef;
}

const QList<float> &cmpSpecies::getPprimeA() const
{
    return pprimeA;
}

void cmpSpecies::setPprimeA(const QList<float> &newPprimeA)
{
    pprimeA = newPprimeA;
}

const QList<float> &cmpSpecies::getPprimeB() const
{
    return pprimeB;
}

void cmpSpecies::setPprimeB(const QList<float> &newPprimeB)
{
    pprimeB = newPprimeB;
}

float cmpSpecies::getTailracePredCoef() const
{
    return tailracePredCoef;
}

void cmpSpecies::setTailracePredCoef(float newTailracePredCoef)
{
    tailracePredCoef = newTailracePredCoef;
}

float cmpSpecies::getForebayPredCoef() const
{
    return forebayPredCoef;
}

void cmpSpecies::setForebayPredCoef(float newForebayPredCoef)
{
    forebayPredCoef = newForebayPredCoef;
}

cmpEquation *cmpSpecies::getGasmortEqn() const
{
    return gasmortEqn;
}

void cmpSpecies::setGasmortEqn(cmpEquation *newGasmortEqn)
{
    gasmortEqn = newGasmortEqn;
}

cmpEquation *cmpSpecies::getFishdensEqn() const
{
    return fishdensEqn;
}

void cmpSpecies::setFishdensEqn(cmpEquation *newFishdensEqn)
{
    fishdensEqn = newFishdensEqn;
}

float cmpSpecies::getInriverLatentMort() const
{
    return inriverLatentMort;
}

void cmpSpecies::setInriverLatentMort(float newInriverLatentMort)
{
    inriverLatentMort = newInriverLatentMort;
}

float cmpSpecies::getTransportLatentMort() const
{
    return transportLatentMort;
}

void cmpSpecies::setTransportLatentMort(float newTransportLatentMort)
{
    transportLatentMort = newTransportLatentMort;
}

float cmpSpecies::getDifferentialReturn() const
{
    return differentialReturn;
}

void cmpSpecies::setDifferentialReturn(float newDifferentialReturn)
{
    differentialReturn = newDifferentialReturn;
}

cmpEquation *cmpSpecies::getInriverReturnEqn() const
{
    return inriverReturnEqn;
}

void cmpSpecies::setInriverReturnEqn(cmpEquation *newInriverReturnEqn)
{
    inriverReturnEqn = newInriverReturnEqn;
}

cmpEquation *cmpSpecies::getTransportReturnEqn() const
{
    return transportReturnEqn;
}

void cmpSpecies::setTransportReturnEqn(cmpEquation *newTransportReturnEqn)
{
    transportReturnEqn = newTransportReturnEqn;
}

cmpEquation *cmpSpecies::getInriverLatentMortEqn() const
{
    return inriverLatentMortEqn;
}

void cmpSpecies::setInriverLatentMortEqn(cmpEquation *newInriverLatentMortEqn)
{
    inriverLatentMortEqn = newInriverLatentMortEqn;
}

const QStringList &cmpSpecies::getReachClassNames() const
{
    return reachClassNames;
}

void cmpSpecies::setReachClassNames(const QStringList &newReachClassNames)
{
    reachClassNames = newReachClassNames;
}
