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


    gasDisspExp = 0.20;
    hwFlowProp = 1.00;

    freeFlowMax = 8.000000;
    preyEnergyDensity = 5400.000000;
    lengthWeightB0 = -11.580000;
    lengthWeightB1 = 3.033000;
    forkThreshold = 0.100000;
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
    computeMuMethod = false;
    yearTraveltimeIndicator = 0;
    yearTraveltimeOffset = 1;
    mortClass = XT;
    fishReturnHyp = SARvsDate;

    debug = false;
    debugInput = false;
    debugCalib = false;

    freeFlowEqn = new cmpEquation(9);
    freeFlowEqn->setParameter(0, 0.0);
    freeFlowEqn->setParameter(1, 4.5);
    freeFlowEqn->setParameter(2, 0.0);
}

cmpDataSettings::~cmpDataSettings()
{
//    if (monte_data != nullptr)
//        delete monte_data;
    delete freeFlowEqn;
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

bool cmpDataSettings::getMigration() const
{
    return migration;
}

void cmpDataSettings::setMigration(QString name)
{
    if (name.compare("juvenile", Qt::CaseInsensitive) == 0
            || name.compare("downstream", Qt::CaseInsensitive) == 0)
        migration = true;
    else
        migration = false; // adult migration - not downstream
}

void cmpDataSettings::setMigration(bool newMigration)
{
    migration = newMigration;
}

bool cmpDataSettings::getInterrupt() const
{
    return interrupt;
}

void cmpDataSettings::setInterrupt(bool newInterrupt)
{
    interrupt = newInterrupt;
}

bool cmpDataSettings::getCompute_flow_break() const
{
    return compute_flow_break;
}

void cmpDataSettings::setCompute_flow_break(bool newCompute_flow_break)
{
    compute_flow_break = newCompute_flow_break;
}

bool cmpDataSettings::getSummary() const
{
    return summary;
}

void cmpDataSettings::setSummary(bool newSummary)
{
    summary = newSummary;
}

float cmpDataSettings::getGasDisspExp() const
{
    return gasDisspExp;
}

void cmpDataSettings::setGasDisspExp(float newGasDisspExp)
{
    gasDisspExp = newGasDisspExp;
}

float cmpDataSettings::getHwFlowProp() const
{
    return hwFlowProp;
}

void cmpDataSettings::setHwFlowProp(float newHwFlowProp)
{
    hwFlowProp = newHwFlowProp;
}

cmpEquation *cmpDataSettings::getFreeFlowEqn() const
{
    return freeFlowEqn;
}

void cmpDataSettings::setFreeFlowEqn(cmpEquation *newFreeFlowEqn)
{
    freeFlowEqn = newFreeFlowEqn;
}

float cmpDataSettings::getFreeFlowMax() const
{
    return freeFlowMax;
}

void cmpDataSettings::setFreeFlowMax(float newFreeFlowMax)
{
    freeFlowMax = newFreeFlowMax;
}

float cmpDataSettings::getPreyEnergyDensity() const
{
    return preyEnergyDensity;
}

void cmpDataSettings::setPreyEnergyDensity(float newPreyEnergyDensity)
{
    preyEnergyDensity = newPreyEnergyDensity;
}

float cmpDataSettings::getLengthWeightB0() const
{
    return lengthWeightB0;
}

void cmpDataSettings::setLengthWeightB0(float newLengthWeightB0)
{
    lengthWeightB0 = newLengthWeightB0;
}

float cmpDataSettings::getLengthWeightB1() const
{
    return lengthWeightB1;
}

void cmpDataSettings::setLengthWeightB1(float newLengthWeightB1)
{
    lengthWeightB1 = newLengthWeightB1;
}

float cmpDataSettings::getForkThreshold() const
{
    return forkThreshold;
}

void cmpDataSettings::setForkThreshold(float newForkThreshold)
{
    forkThreshold = newForkThreshold;
}

const QString &cmpDataSettings::getWaterTravelUpperSegment() const
{
    return waterTravelUpperSegment;
}

void cmpDataSettings::setWaterTravelUpperSegment(const QString &newWaterTravelUpperSegment)
{
    waterTravelUpperSegment = newWaterTravelUpperSegment;
}

const QString &cmpDataSettings::getWaterTravelLowerSegment() const
{
    return waterTravelLowerSegment;
}

void cmpDataSettings::setWaterTravelLowerSegment(const QString &newWaterTravelLowerSegment)
{
    waterTravelLowerSegment = newWaterTravelLowerSegment;
}

int cmpDataSettings::getWaterTravelFirstDay() const
{
    return waterTravelFirstDay;
}

void cmpDataSettings::setWaterTravelFirstDay(int newWaterTravelFirstDay)
{
    waterTravelFirstDay = newWaterTravelFirstDay;
}

int cmpDataSettings::getWaterTravelLastDay() const
{
    return waterTravelLastDay;
}

void cmpDataSettings::setWaterTravelLastDay(int newWaterTravelLastDay)
{
    waterTravelLastDay = newWaterTravelLastDay;
}

int cmpDataSettings::getWaterTravelTime() const
{
    return waterTravelTime;
}

void cmpDataSettings::setWaterTravelTime(int newWaterTravelTime)
{
    waterTravelTime = newWaterTravelTime;
}

float cmpDataSettings::getMigrationRateMin() const
{
    return migrationRateMin;
}

void cmpDataSettings::setMigrationRateMin(float newMigrationRateMin)
{
    migrationRateMin = newMigrationRateMin;
}

bool cmpDataSettings::getSuppressVariation() const
{
    return suppressVariation;
}

void cmpDataSettings::setSuppressVariation(bool newSuppressVariation)
{
    suppressVariation = newSuppressVariation;
}

bool cmpDataSettings::getPredVolInteraction() const
{
    return predVolInteraction;
}

void cmpDataSettings::setPredVolInteraction(bool newPredVolInteraction)
{
    predVolInteraction = newPredVolInteraction;
}

bool cmpDataSettings::getAgeDependentFge() const
{
    return ageDependentFge;
}

void cmpDataSettings::setAgeDependentFge(bool newAgeDependentFge)
{
    ageDependentFge = newAgeDependentFge;
}

bool cmpDataSettings::getTruncateTravelVect() const
{
    return truncateTravelVect;
}

void cmpDataSettings::setTruncateTravelVect(bool newTruncateTravelVect)
{
    truncateTravelVect = newTruncateTravelVect;
}

bool cmpDataSettings::getTruncateResSurvival() const
{
    return truncateResSurvival;
}

void cmpDataSettings::setTruncateResSurvival(bool newTruncateResSurvival)
{
    truncateResSurvival = newTruncateResSurvival;
}

bool cmpDataSettings::getComputeMuMethod() const
{
    return computeMuMethod;
}

void cmpDataSettings::setComputeMuMethod(bool newComputeMuMethod)
{
    computeMuMethod = newComputeMuMethod;
}

int cmpDataSettings::getYearTraveltimeIndicator() const
{
    return yearTraveltimeIndicator;
}

void cmpDataSettings::setYearTraveltimeIndicator(int newYearTraveltimeIndicator)
{
    yearTraveltimeIndicator = newYearTraveltimeIndicator;
}

int cmpDataSettings::getYearTraveltimeOffset() const
{
    return yearTraveltimeOffset;
}

void cmpDataSettings::setYearTraveltimeOffset(int newYearTraveltimeOffset)
{
    yearTraveltimeOffset = newYearTraveltimeOffset;
}

QString &cmpDataSettings::getMortClassString()
{
    return mortClassString;
}

void cmpDataSettings::setMortClass(QString &mortclass)
{
    mortClassString = mortclass;
    if (mortclass.compare("gas_and_predation", Qt::CaseInsensitive) == 0)
        mortClass = GasAndPredation;
    else if (mortclass.compare("xt", Qt::CaseInsensitive) == 0)
        mortClass = XT;
    else if (mortclass.compare("custon", Qt::CaseInsensitive) == 0)
        mortClass = Custom;
}

MortalityClass cmpDataSettings::getMortClass() const
{
    return mortClass;
}

void cmpDataSettings::setMortClass(MortalityClass newMortClass)
{
    mortClass = newMortClass;
    switch (mortClass)
    {
    case GasAndPredation: // Disovled gas effects and predation model.
        mortClassString = QString("gas_and_predation");
        break;
    case XT:            // Distance and time model.
        mortClassString = QString("xt");
        break;
    default:
    case Custom:         // Custom mortality model using average water temperature, average flow,
        mortClassString = QString("custom");
        break;
    }
}

void cmpDataSettings::setFishReturnHyp(ReturnHypothesis newFishReturnHyp)
{
    fishReturnHyp = newFishReturnHyp;
}

ReturnHypothesis cmpDataSettings::getFishReturnHyp() const
{
    return fishReturnHyp;
}
