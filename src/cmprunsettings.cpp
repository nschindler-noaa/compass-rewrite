#include "cmprunsettings.h"


cmpRunSettings::cmpRunSettings()
{
    numDaysInSeason = 366;
    damSlicesPerDay = 2;
    timeStepsPerDay = 2;
}


int cmpRunSettings::getNumDaysInSeason()
{
    return numDaysInSeason;
}

void cmpRunSettings::setNumDaysInSeason(int newNumDays)
{
    numDaysInSeason = newNumDays;
    damSlicesInSeason = damSlicesPerDay * numDaysInSeason;
    timeStepsInSeason = timeStepsPerDay * numDaysInSeason;
}

int cmpRunSettings::getNumDaysInRun()
{
    return numDaysInRun;
}

void cmpRunSettings::setNumDaysInRun(int newNumDays)
{
    numDaysInRun = newNumDays;
    damSlicesPerRun = damSlicesPerDay * numDaysInRun;
    timeStepsPerRun = timeStepsPerDay * numDaysInRun;
}

int cmpRunSettings::getDamSlicesPerDay()
{
    return damSlicesPerDay;
}

void cmpRunSettings::setDamSlicesPerDay(int newDamSlicesPerDay)
{
    damSlicesPerDay = newDamSlicesPerDay;
    damSlicesInSeason = damSlicesPerDay * numDaysInSeason;
    damSlicesPerRun = damSlicesPerDay * numDaysInRun;
    damSlicesPerStep = damSlicesPerDay / timeStepsPerDay;
}

int cmpRunSettings::getTimeStepsPerDay()
{
    return timeStepsPerDay;
}

void cmpRunSettings::setTimeStepsPerDay(int newTimeStepsPerDay)
{
    timeStepsPerDay = newTimeStepsPerDay;
    timeStepsInSeason = timeStepsPerDay * numDaysInSeason;
    damSlicesPerStep = damSlicesPerDay / timeStepsPerDay;
}

bool cmpRunSettings::getRouteTracking() const
{
    return routeTracking;
}

void cmpRunSettings::setRouteTracking(bool newRouteTracking)
{
    routeTracking = newRouteTracking;
}

bool cmpRunSettings::getCalcGas() const
{
    return calcGas;
}

void cmpRunSettings::setCalcGas(bool newCalcGas)
{
    calcGas = newCalcGas;
}

bool cmpRunSettings::getCalcPred() const
{
    return calcPred;
}

void cmpRunSettings::setCalcPred(bool newCalcPred)
{
    calcPred = newCalcPred;
}

bool cmpRunSettings::getCalcGrowth() const
{
    return calcGrowth;
}

void cmpRunSettings::setCalcGrowth(bool newCalcGrowth)
{
    calcGrowth = newCalcGrowth;
}

bool cmpRunSettings::getDownstream() const
{
    return !migration;
}

void cmpRunSettings::setDownstream(bool newDownstream)
{
    migration = !newDownstream;
}

bool cmpRunSettings::getUpstream() const
{
    return migration;
}

void cmpRunSettings::setUpstream(bool newUpstream)
{
    migration = newUpstream;
}


void cmpRunSettings::setDebug(bool newDebug)
{
    debug = newDebug;
    debugInput = debug;
    debugCalib = debug;
}

bool cmpRunSettings::getCalcTurbidity() const
{
    return calcTurbidity;
}

void cmpRunSettings::setCalcTurbidity(bool newCalcTurbidity)
{
    calcTurbidity = newCalcTurbidity;
}

int cmpRunSettings::getDayStart() const
{
    return dayStart;
}

void cmpRunSettings::setDayStart(int newDayStart)
{
    dayStart = newDayStart;
}

int cmpRunSettings::getNightStart() const
{
    return nightStart;
}

void cmpRunSettings::setNightStart(int newNightStart)
{
    nightStart = newNightStart;
}
