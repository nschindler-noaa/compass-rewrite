#include "cmpdamspecies.h"

cmpDamSpecies::cmpDamSpecies()
{
    reset();
}

cmpDamSpecies::cmpDamSpecies(cmpDamSpecies &rhs)
{
    copy(rhs);
}

void cmpDamSpecies::reset()
{
    spillMort = 0;
    bypassMort = 0;
    sluicewayMort = 0;
    turbineMort = 0;
    transportMort = 0;
    rswMort = 0;
    meanForebayTransitTime = 0;
    separationProb = 1;
//	pred.clear();
//	predPopMean = 0;
//	predPopTailrace = 0;
    spillEqn = cmpEquation(0);
    fgeEqn = cmpEquation(0);
    transEqn = cmpEquation(0);
    delayEqn = cmpEquation(0);
    rswEqn = cmpEquation(0);
    sluicewayProp = 0;
    dayFge = 0;
    nightFge = 0;
    spillDelay = 0;
    bypassDelay = 0;
    sluicewayDelay = 0;
    turbineDelay = 0;
    rswDelay = 0;
    spillDayDelay = 0;
    bypassDayDelay = 0;
    sluicewayDayDelay = 0;
    turbineDayDelay = 0;
    rswDayDelay = 0;
}

cmpDamSpecies &cmpDamSpecies::copy(cmpDamSpecies &rhs)
{
    spillMort = rhs.getSpillMort();
    bypassMort = rhs.getBypassMort();
    sluicewayMort = rhs.getSluicewayMort();
    turbineMort = rhs.getTurbineMort();
    transportMort = rhs.getTransportMort();
    rswMort = rhs.getRswMort();
    meanForebayTransitTime = rhs.getMeanForebayTransitTime();
    separationProb = rhs.getSeparationProb();
//	pred = copyArray(rhs.getPred());
//	predPopMean = rhs.getPredPopMean();
//	predPopTailrace = rhs.getPredPopTailrace();
    spillEqn = rhs.getSpillEqn();
    fgeEqn = rhs.getFgeEqn();
    transEqn = rhs.getTransEqn();
    delayEqn = rhs.getDelayEqn();
    rswEqn = rhs.getRswEqn();
    sluicewayProp = rhs.getSluicewayProp();
    dayFge = rhs.getDayFge();
    nightFge = rhs.getNightFge();
    spillDelay = rhs.getSpillDelay();
    bypassDelay = rhs.getBypassDelay();
    sluicewayDelay = rhs.getSluicewayDelay();
    turbineDelay = rhs.getTurbineDelay();
    rswDelay = rhs.getRswDelay();
    spillDayDelay = rhs.getSpillDayDelay();
    bypassDayDelay = rhs.getBypassDayDelay();
    sluicewayDayDelay = rhs.getSluicewayDayDelay();
    turbineDayDelay = rhs.getTurbineDayDelay();
    rswDayDelay = rhs.getRswDayDelay();

    return *this;
}

bool cmpDamSpecies::operator ==(cmpDamSpecies &rhs)
{
    bool equals = true;
    if      (spillMort != rhs.getSpillMort()) equals = false;
    else if (bypassMort != rhs.getBypassMort()) equals = false;
    else if (sluicewayMort != rhs.getSluicewayMort()) equals = false;
    else if (turbineMort != rhs.getTurbineMort()) equals = false;
    else if (transportMort != rhs.getTransportMort()) equals = false;
    else if (rswMort != rhs.getRswMort()) equals = false;
    else if (meanForebayTransitTime != rhs.getMeanForebayTransitTime()) equals = false;
    else if (separationProb != rhs.getSeparationProb()) equals = false;
//	else if (pred != rhs.getPred()) equals = false;
//	else if (predPopMean != rhs.getPredPopMean()) equals = false;
//	else if (predPopTailrace != rhs.getPredPopTailrace()) equals = false;
    else if (spillEqn.isEqual(rhs.getSpillEqn())) equals = false;
    else if (fgeEqn.isEqual(rhs.getFgeEqn())) equals = false;
    else if (transEqn.isEqual(rhs.getTransEqn())) equals = false;
    else if (delayEqn.isEqual(rhs.getDelayEqn())) equals = false;
    else if (rswEqn.isEqual(rhs.getRswEqn())) equals = false;
    else if (sluicewayProp != rhs.getSluicewayProp()) equals = false;
    else if (dayFge != rhs.getDayFge()) equals = false;
    else if (nightFge != rhs.getNightFge()) equals = false;
    else if (spillDelay != rhs.getSpillDelay()) equals = false;
    else if (bypassDelay != rhs.getBypassDelay()) equals = false;
    else if (sluicewayDelay != rhs.getSluicewayDelay()) equals = false;
    else if (turbineDelay != rhs.getTurbineDelay()) equals = false;
    else if (rswDelay != rhs.getRswDelay()) equals = false;
    else if (spillDayDelay != rhs.getSpillDayDelay()) equals = false;
    else if (bypassDayDelay != rhs.getBypassDayDelay()) equals = false;
    else if (sluicewayDayDelay != rhs.getSluicewayDayDelay()) equals = false;
    else if (turbineDayDelay != rhs.getTurbineDayDelay()) equals = false;
    else if (rswDayDelay != rhs.getRswDayDelay()) equals = false;
    return equals;
}

float cmpDamSpecies::getSpillMort() const
{
    return spillMort;
}

void cmpDamSpecies::setSpillMort(float newSpillMort)
{
    spillMort = newSpillMort;
}

float cmpDamSpecies::getBypassMort() const
{
    return bypassMort;
}

void cmpDamSpecies::setBypassMort(float newBypassMort)
{
    bypassMort = newBypassMort;
}

float cmpDamSpecies::getSluicewayMort() const
{
    return sluicewayMort;
}

void cmpDamSpecies::setSluicewayMort(float newSluicewayMort)
{
    sluicewayMort = newSluicewayMort;
}

float cmpDamSpecies::getTurbineMort() const
{
    return turbineMort;
}

void cmpDamSpecies::setTurbineMort(float newTurbineMort)
{
    turbineMort = newTurbineMort;
}

float cmpDamSpecies::getTransportMort() const
{
    return transportMort;
}

void cmpDamSpecies::setTransportMort(float newTransportMort)
{
    transportMort = newTransportMort;
}

float cmpDamSpecies::getRswMort() const
{
    return rswMort;
}

void cmpDamSpecies::setRswMort(float newRswMort)
{
    rswMort = newRswMort;
}

float cmpDamSpecies::getMeanForebayTransitTime() const
{
    return meanForebayTransitTime;
}

void cmpDamSpecies::setMeanForebayTransitTime(float newMeanForebayTransitTime)
{
    meanForebayTransitTime = newMeanForebayTransitTime;
}

float cmpDamSpecies::getSeparationProb() const
{
    return separationProb;
}

void cmpDamSpecies::setSeparationProb(float newSeparationProb)
{
    separationProb = newSeparationProb;
}

const cmpEquation &cmpDamSpecies::getSpillEqn() const
{
    return spillEqn;
}

void cmpDamSpecies::setSpillEqn(const cmpEquation &newSpillEqn)
{
    spillEqn = newSpillEqn;
}

const cmpEquation &cmpDamSpecies::getFgeEqn() const
{
    return fgeEqn;
}

void cmpDamSpecies::setFgeEqn(const cmpEquation &newFgeEqn)
{
    fgeEqn = newFgeEqn;
}

const cmpEquation &cmpDamSpecies::getTransEqn() const
{
    return transEqn;
}

void cmpDamSpecies::setTransEqn(const cmpEquation &newTransEqn)
{
    transEqn = newTransEqn;
}

const cmpEquation &cmpDamSpecies::getDelayEqn() const
{
    return delayEqn;
}

void cmpDamSpecies::setDelayEqn(const cmpEquation &newDelayEqn)
{
    delayEqn = newDelayEqn;
}

const cmpEquation &cmpDamSpecies::getRswEqn() const
{
    return rswEqn;
}

void cmpDamSpecies::setRswEqn(const cmpEquation &newRswEqn)
{
    rswEqn = newRswEqn;
}

float cmpDamSpecies::getSluicewayProp() const
{
    return sluicewayProp;
}

void cmpDamSpecies::setSluicewayProp(float newSluicewayProp)
{
    sluicewayProp = newSluicewayProp;
}

float cmpDamSpecies::getDayFge() const
{
    return dayFge;
}

void cmpDamSpecies::setDayFge(float newDayFge)
{
    dayFge = newDayFge;
}

float cmpDamSpecies::getNightFge() const
{
    return nightFge;
}

void cmpDamSpecies::setNightFge(float newNightFge)
{
    nightFge = newNightFge;
}

float cmpDamSpecies::getSpillDelay() const
{
    return spillDelay;
}

void cmpDamSpecies::setSpillDelay(float newSpillDelay)
{
    spillDelay = newSpillDelay;
}

float cmpDamSpecies::getBypassDelay() const
{
    return bypassDelay;
}

void cmpDamSpecies::setBypassDelay(float newBypassDelay)
{
    bypassDelay = newBypassDelay;
}

float cmpDamSpecies::getSluicewayDelay() const
{
    return sluicewayDelay;
}

void cmpDamSpecies::setSluicewayDelay(float newSluicewayDelay)
{
    sluicewayDelay = newSluicewayDelay;
}

float cmpDamSpecies::getTurbineDelay() const
{
    return turbineDelay;
}

void cmpDamSpecies::setTurbineDelay(float newTurbineDelay)
{
    turbineDelay = newTurbineDelay;
}

float cmpDamSpecies::getRswDelay() const
{
    return rswDelay;
}

void cmpDamSpecies::setRswDelay(float newRswDelay)
{
    rswDelay = newRswDelay;
}

float cmpDamSpecies::getSpillDayDelay() const
{
    return spillDayDelay;
}

void cmpDamSpecies::setSpillDayDelay(float newSpillDayDelay)
{
    spillDayDelay = newSpillDayDelay;
}

float cmpDamSpecies::getBypassDayDelay() const
{
    return bypassDayDelay;
}

void cmpDamSpecies::setBypassDayDelay(float newBypassDayDelay)
{
    bypassDayDelay = newBypassDayDelay;
}

float cmpDamSpecies::getSluicewayDayDelay() const
{
    return sluicewayDayDelay;
}

void cmpDamSpecies::setSluicewayDayDelay(float newSluicewayDayDelay)
{
    sluicewayDayDelay = newSluicewayDayDelay;
}

float cmpDamSpecies::getTurbineDayDelay() const
{
    return turbineDayDelay;
}

void cmpDamSpecies::setTurbineDayDelay(float newTurbineDayDelay)
{
    turbineDayDelay = newTurbineDayDelay;
}

float cmpDamSpecies::getRswDayDelay() const
{
    return rswDayDelay;
}

void cmpDamSpecies::setRswDayDelay(float newRswDayDelay)
{
    rswDayDelay = newRswDayDelay;
}
