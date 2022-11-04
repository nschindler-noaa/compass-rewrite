#include "cmpdatasettings.h"


cmpDataSettings::cmpDataSettings()
{
    numDaysInSeason = 366;
    damSlicesPerDay = 2;
    timeStepsPerDay = 2;
}


int cmpDataSettings::getNumDaysInSeason()
{
    return numDaysInSeason;
}

void cmpDataSettings::setNumDaysInSeason(int newNumDays)
{
    numDaysInSeason = newNumDays;
    damSlicesPerSeason = damSlicesPerDay * numDaysInSeason;
    timeStepsPerSeason = timeStepsPerDay * numDaysInSeason;
}

int cmpDataSettings::getNumDaysInRun()
{
    return numDaysInRun;
}

void cmpDataSettings::setNumDaysInRun(int newNumDays)
{
    numDaysInRun = newNumDays;
    damSlicesPerRun = damSlicesPerDay * numDaysInRun;
    timeStepsPerRun = timeStepsPerDay * numDaysInRun;
}

int cmpDataSettings::getDamSlicesPerDay()
{
    return damSlicesPerDay;
}

void cmpDataSettings::setDamSlicesPerDay(int newDamSlicesPerDay)
{
    damSlicesPerDay = newDamSlicesPerDay;
    damSlicesPerSeason = damSlicesPerDay * numDaysInSeason;
    damSlicesPerRun = damSlicesPerDay * numDaysInRun;
    damSlicesPerStep = damSlicesPerDay / timeStepsPerDay;
}

int cmpDataSettings::getTimeStepsPerDay()
{
    return timeStepsPerDay;
}

void cmpDataSettings::setTimeStepsPerDay(int newTimeStepsPerDay)
{
    timeStepsPerDay = newTimeStepsPerDay;
    timeStepsPerSeason = timeStepsPerDay * numDaysInSeason;
    damSlicesPerStep = damSlicesPerDay / timeStepsPerDay;
}

bool cmpDataSettings::getRouteTracking() const
{
    return routeTracking;
}

void cmpDataSettings::setRouteTracking(bool newRouteTracking)
{
    routeTracking = newRouteTracking;
}

bool cmpDataSettings::getCalcGas() const
{
    return calcGas;
}

void cmpDataSettings::setCalcGas(bool newCalcGas)
{
    calcGas = newCalcGas;
}

bool cmpDataSettings::getCalcPred() const
{
    return calcPred;
}

void cmpDataSettings::setCalcPred(bool newCalcPred)
{
    calcPred = newCalcPred;
}

bool cmpDataSettings::getCalcGrowth() const
{
    return calcGrowth;
}

void cmpDataSettings::setCalcGrowth(bool newCalcGrowth)
{
    calcGrowth = newCalcGrowth;
}

bool cmpDataSettings::getDownstream() const
{
    return !migration;
}

void cmpDataSettings::setDownstream(bool newDownstream)
{
    migration = !newDownstream;
}

bool cmpDataSettings::getUpstream() const
{
    return migration;
}

void cmpDataSettings::setUpstream(bool newUpstream)
{
    migration = newUpstream;
}


void cmpDataSettings::setDebug(bool newDebug)
{
    debug = newDebug;
    debugInput = debug;
    debugCalib = debug;
}

bool cmpDataSettings::getCalcTurbidity() const
{
    return calcTurbidity;
}

void cmpDataSettings::setCalcTurbidity(bool newCalcTurbidity)
{
    calcTurbidity = newCalcTurbidity;
}

int cmpDataSettings::getDayStart() const
{
    return dayStart;
}

void cmpDataSettings::setDayStart(int newDayStart)
{
    dayStart = newDayStart;
}

int cmpDataSettings::getNightStart() const
{
    return nightStart;
}

void cmpDataSettings::setNightStart(int newNightStart)
{
    nightStart = newNightStart;
}
