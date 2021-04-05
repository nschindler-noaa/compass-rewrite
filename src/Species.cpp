#include "Species.h"

Species::Species (QString nm) :
    Stock(nm)
{

    allocate(1);
}

Species::~Species ()
{
    delete name;
}

void Species::allocate(int num)
{
    while (pprimeA.count() < num) {
        pprimeA.append(0);
        pprimeB.append(0);
        reachPredCoef.append(0);
    }
    while (pprimeA.count() > num) {
        pprimeA.takeLast();
        pprimeB.takeLast();
        reachPredCoef.takeLast();
    }
}

Equation Species::getInriverLatentMortEqn() const
{
    return inriverLatentMortEqn;
}

void Species::setInriverLatentMortEqn(const Equation &value)
{
    inriverLatentMortEqn = value;
}

float Species::getDifferentialReturn() const
{
    return differentialReturn;
}

void Species::setDifferentialReturn(float value)
{
    differentialReturn = value;
}

float Species::getTransportLatentMort() const
{
    return transportLatentMort;
}

void Species::setTransportLatentMort(float value)
{
    transportLatentMort = value;
}

float Species::getInriverLatentMort() const
{
    return inriverLatentMort;
}

void Species::setInriverLatentMort(float value)
{
    inriverLatentMort = value;
}

Equation Species::getFishDensEqn() const
{
    return fishDensEqn;
}

void Species::setFishDensEqn(const Equation &value)
{
    fishDensEqn = value;
}

Equation Species::getGasMortEqn() const
{
    return gasMortEqn;
}

void Species::setGasMortEqn(const Equation &value)
{
    gasMortEqn = value;
}

float Species::getForebayPredCoef() const
{
    return forebayPredCoef;
}

void Species::setForebayPredCoef(float value)
{
    forebayPredCoef = value;
}

float Species::getTailracePredCoef() const
{
    return tailracePredCoef;
}

void Species::setTailracePredCoef(float value)
{
    tailracePredCoef = value;
}

float Species::getPprimeB(int index) const
{
    return pprimeB[index];
}

void Species::setPprimeB(int index, const float &value)
{
    pprimeB[index] = value;
}

float Species::getPprimeA(int index) const
{
    return pprimeA[index];
}

void Species::setPprimeA(int index, const float &value)
{
    pprimeA[index] = value;
}

float Species::getReachPredCoef(int index) const
{
    return reachPredCoef[index];
}

void Species::setReachPredCoef(int index, const float &value)
{
    reachPredCoef[index] = value;
}
