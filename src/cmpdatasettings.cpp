#include "cmpdatasettings.h"


cmpDataSettings::cmpDataSettings()
{
    migration = true;
    interrupt = false;

    compute_flow_break = false;
    summary = false;
//    monte_data = nullptr;
    iterations = 2;
    tt_calib = false;
    surv_calib = false;
    consolidate_on_stock = false;

    numDaysInSeason = 366;
    numDaysInRun = 366;
    timeStepsPerDay = 2;
    timeStepsPerSeason = 732;
    timeStepsPerRun = 732;
    seasonStartStep = 1;
    damSlicesPerDay = 2;
    damSlicesPerStep = 1;
    damSlicesPerSeason = 732;
    damSlicesPerRun = 732;
    gasStepsPerDay = 2;
    spillStepsPerDay = 2;

    dayStart = 600;
    nightStart = 2000;

    realTime = false;
    routeTracking = true;

    calcGas = true;
    calcTurbidity = false;
    calcPred = false;
    calcGrowth = false;


    gas_dissp_exp = 0.20;
    hw_flow_prop = 1.00;

    freeFlowMax = 8.000000;
    preyEnergyDensity = 5400.000000;
    lengthWeightB0 = -11.580000;
    lengthWeightB1 = 3.033000;
    fork_threshold = 0.100000;
    waterTravelUpperSegment = QString("Little_Goose_Pool");
    waterTravelLowerSegment = QString("Estuary");
    waterTravelFirstDay = 1;
    waterTravelLastDay = 365;
    waterTravelTime = 1.000;
    migrationRateMin = 1.000;
    suppressVariation = true;
    predVolInteraction = false;
    ageDependentFge = false;
    truncateTravelVect = true;
    truncateResSurvival = true;
    compute_mu_method = false;
    yearTraveltimeIndicator = 0;
    yearTraveltimeOffset = 1;
    mortClass = XT;
    fishReturnHyp = SARvsDate;

    debug = false;
    debugInput = false;
    debugCalib = false;

    freeFlow = new cmpEquation(9);
    freeFlow->setParameter(0, 0.0);
    freeFlow->setParameter(1, 4.5);
    freeFlow->setParameter(2, 0.0);
}

cmpDataSettings::~cmpDataSettings()
{
//    if (monte_data != nullptr)
//        delete monte_data;
    delete freeFlow;
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
