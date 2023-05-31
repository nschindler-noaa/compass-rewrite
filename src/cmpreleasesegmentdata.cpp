#include "cmpreleasesegmentdata.h"

#include <cmath>

cmpPassageStats::cmpPassageStats ()
{
    firstDay = 0;
    lastDay = 1;
    totFishIn = 0;
    totFishOut = 0;
    mean = 0;
    median = 0;
    mode = 0;
    stdDev = 0;
}
cmpPassageStats::~cmpPassageStats ()
{}


int cmpPassageStats::getFirstDay() const
{
    return firstDay;
}

void cmpPassageStats::setFirstDay(int value)
{
    firstDay = value;
}

int cmpPassageStats::getLastDay() const
{
    return lastDay;
}

void cmpPassageStats::setLastDay(int value)
{
    lastDay = value;
}

float cmpPassageStats::getTotFishIn() const
{
    return totFishIn;
}

void cmpPassageStats::setTotFishIn(float value)
{
    totFishIn = value;
}

float cmpPassageStats::getTotFishOut() const
{
    return totFishOut;
}

void cmpPassageStats::setTotFishOut(float value)
{
    totFishOut = value;
}

float cmpPassageStats::getMean() const
{
    return mean;
}

void cmpPassageStats::setMean(float value)
{
    mean = value;
}

float cmpPassageStats::getMedian() const
{
    return median;
}

void cmpPassageStats::setMedian(float value)
{
    median = value;
}

float cmpPassageStats::getMode() const
{
    return mode;
}

void cmpPassageStats::setMode(float value)
{
    mode = value;
}

float cmpPassageStats::getStdDev() const
{
    return stdDev;
}

void cmpPassageStats::setStdDev(float value)
{
    stdDev = value;
}

void cmpPassageStats::computeStats(QList<float> &dailyNum)
{
    float total = 0;
    float meanTotal = 0;
    int i = 0;
    // find first
    firstDay = 0;
    while (firstDay == 0)
    {
        if (dailyNum.at(i) > 0)
            firstDay = i;
    }
    // then find last
    for (; i < dailyNum.count(); i++)
    {
        total += dailyNum.at(i);
        meanTotal += total * i;
        if (dailyNum.at(i) > 0)
            lastDay = i;
    }

    // compute mean
    mean = meanTotal / total;

    // compute median
    float half = total / 2;
    float halfTotal = 0;
    i = 0;
    while (halfTotal < half)
    {
        halfTotal += dailyNum.at(i++);
    }
    median = (i - 1) + (halfTotal - half);

    // compute mode
    for (i = firstDay; i < lastDay; i++)
    {
        if (dailyNum.at(i) > dailyNum.at(i+1))
            mode = i + dailyNum.at(i) - dailyNum.at(i+1);
    }

    // compute standard deviation
    float meanSq = 0;
    float totalMeanSq = 0;
    float meanSqMean = 0;
    for (i = firstDay; i <= lastDay; i++)
    {
        meanSq = dailyNum.at(i) - mean;
        meanSq *= meanSq;
        totalMeanSq += meanSq;
    }
    meanSqMean = totalMeanSq / (lastDay - firstDay + 1);
    stdDev = sqrt(meanSqMean);
}

cmpReleaseSegmentStats::cmpReleaseSegmentStats()
{
    reset();
}

void cmpReleaseSegmentStats::reset()
{
    done = false;

//#ifdef ROUTE_TRACKING
    turbineInTotal = 0;
    turbineOutTotal = 0;
    sluicewayInTotal = 0;
    sluicewayOutTotal = 0;
    bypassInTotal = 0;
    bypassOutTotal = 0;
    spillwayInTotal = 0;
    spillwayOutTotal = 0;
    rswInTotal = 0;
    rswOutTotal = 0;
    postmortTransportTotal = 0;
    spillEfficiency = 0;
//#endif
    d = nullptr;

    inriverLatentMort = 0;
    transportLatentMort = 0;
    differentialReturn = 0;
    inriverReturn = 0;
    transportReturn = 0;
    overallReturn = 0;
    postBonnevilleTotal = 0;
    nightPassage = 0;
    flowAverage = 0;
}

bool cmpReleaseSegmentStats::getDone() const
{
    return done;
}

void cmpReleaseSegmentStats::setDone(bool newDone)
{
    done = newDone;
}

const cmpPassageStats &cmpReleaseSegmentStats::getTotalStats() const
{
    return totalStats;
}

const cmpPassageStats &cmpReleaseSegmentStats::getInriverStats() const
{
    return inriverStats;
}

const cmpPassageStats &cmpReleaseSegmentStats::getTransport() const
{
    return transportStats;
}

float cmpReleaseSegmentStats::getTurbineInTotal() const
{
    return turbineInTotal;
}

void cmpReleaseSegmentStats::setTurbineInTotal(float newTurbineInTotal)
{
    turbineInTotal = newTurbineInTotal;
}

float cmpReleaseSegmentStats::getTurbineOutTotal() const
{
    return turbineOutTotal;
}

void cmpReleaseSegmentStats::setTurbineOutTotal(float newTurbineOutTotal)
{
    turbineOutTotal = newTurbineOutTotal;
}

float cmpReleaseSegmentStats::getSluicewayInTotal() const
{
    return sluicewayInTotal;
}

void cmpReleaseSegmentStats::setSluicewayInTotal(float newSluicewayInTotal)
{
    sluicewayInTotal = newSluicewayInTotal;
}

float cmpReleaseSegmentStats::getSluicewayOutTotal() const
{
    return sluicewayOutTotal;
}

void cmpReleaseSegmentStats::setSluicewayOutTotal(float newSluicewayOutTotal)
{
    sluicewayOutTotal = newSluicewayOutTotal;
}

float cmpReleaseSegmentStats::getBypassInTotal() const
{
    return bypassInTotal;
}

void cmpReleaseSegmentStats::setBypassInTotal(float newBypassInTotal)
{
    bypassInTotal = newBypassInTotal;
}

float cmpReleaseSegmentStats::getBypassOutTotal() const
{
    return bypassOutTotal;
}

void cmpReleaseSegmentStats::setBypassOutTotal(float newBypassOutTotal)
{
    bypassOutTotal = newBypassOutTotal;
}

float cmpReleaseSegmentStats::getSpillwayInTotal() const
{
    return spillwayInTotal;
}

void cmpReleaseSegmentStats::setSpillwayInTotal(float newSpillwayInTotal)
{
    spillwayInTotal = newSpillwayInTotal;
}

float cmpReleaseSegmentStats::getSpillwayOutTotal() const
{
    return spillwayOutTotal;
}

void cmpReleaseSegmentStats::setSpillwayOutTotal(float newSpillwayOutTotal)
{
    spillwayOutTotal = newSpillwayOutTotal;
}

float cmpReleaseSegmentStats::getRswInTotal() const
{
    return rswInTotal;
}

void cmpReleaseSegmentStats::setRswInTotal(float newRswInTotal)
{
    rswInTotal = newRswInTotal;
}

float cmpReleaseSegmentStats::getRswOutTotal() const
{
    return rswOutTotal;
}

void cmpReleaseSegmentStats::setRswOutTotal(float newRswOutTotal)
{
    rswOutTotal = newRswOutTotal;
}

float cmpReleaseSegmentStats::getPostmortTransportTotal() const
{
    return postmortTransportTotal;
}

void cmpReleaseSegmentStats::setPostmortTransportTotal(float newPostmortTransportTotal)
{
    postmortTransportTotal = newPostmortTransportTotal;
}

float cmpReleaseSegmentStats::getSpillEfficiency() const
{
    return spillEfficiency;
}

void cmpReleaseSegmentStats::setSpillEfficiency(float newSpillEfficiency)
{
    spillEfficiency = newSpillEfficiency;
}

delayed_mortality_table *cmpReleaseSegmentStats::getD() const
{
    return d;
}

void cmpReleaseSegmentStats::setD(delayed_mortality_table *newD)
{
    d = newD;
}

float cmpReleaseSegmentStats::getInriverLatentMort() const
{
    return inriverLatentMort;
}

void cmpReleaseSegmentStats::setInriverLatentMort(float newInriverLatentMort)
{
    inriverLatentMort = newInriverLatentMort;
}

float cmpReleaseSegmentStats::getTransportLatentMort() const
{
    return transportLatentMort;
}

void cmpReleaseSegmentStats::setTransportLatentMort(float newTransportLatentMort)
{
    transportLatentMort = newTransportLatentMort;
}

float cmpReleaseSegmentStats::getDifferentialReturn() const
{
    return differentialReturn;
}

void cmpReleaseSegmentStats::setDifferentialReturn(float newDifferentialReturn)
{
    differentialReturn = newDifferentialReturn;
}

float cmpReleaseSegmentStats::getInriverReturn() const
{
    return inriverReturn;
}

void cmpReleaseSegmentStats::setInriverReturn(float newInriverReturn)
{
    inriverReturn = newInriverReturn;
}

float cmpReleaseSegmentStats::getTransportReturn() const
{
    return transportReturn;
}

void cmpReleaseSegmentStats::setTransportReturn(float newTransportReturn)
{
    transportReturn = newTransportReturn;
}

float cmpReleaseSegmentStats::getOverallReturn() const
{
    return overallReturn;
}

void cmpReleaseSegmentStats::setOverallReturn(float newOverallReturn)
{
    overallReturn = newOverallReturn;
}

float cmpReleaseSegmentStats::getPostBonnevilleTotal() const
{
    return postBonnevilleTotal;
}

void cmpReleaseSegmentStats::setPostBonnevilleTotal(float newPostBonnevilleTotal)
{
    postBonnevilleTotal = newPostBonnevilleTotal;
}

float cmpReleaseSegmentStats::getNightPassage() const
{
    return nightPassage;
}

void cmpReleaseSegmentStats::setNightPassage(float newNightPassage)
{
    nightPassage = newNightPassage;
}

float cmpReleaseSegmentStats::getFlowAverage() const
{
    return flowAverage;
}

void cmpReleaseSegmentStats::setFlowAverage(float newFlowAverage)
{
    flowAverage = newFlowAverage;
}

bool cmpReleaseSegmentStats::getRouteTracking() const
{
    return routeTracking;
}

void cmpReleaseSegmentStats::setRouteTracking(bool newRouteTracking)
{
    routeTracking = newRouteTracking;
}


cmpReleaseSegmentData::cmpReleaseSegmentData()
{
    rivSeg = nullptr;
    d = nullptr;
}

cmpReleaseSegmentData::~cmpReleaseSegmentData()
{
    if (d != nullptr)
        delete d;
}

void cmpReleaseSegmentData::reset()
{

}

int cmpReleaseSegmentData::allocate(int days)
{

}

cmpRiverSegment *cmpReleaseSegmentData::getRivSeg() const
{
    return rivSeg;
}

void cmpReleaseSegmentData::setRivSeg(cmpRiverSegment *newRivSeg)
{
    rivSeg = newRivSeg;
}

const QList<float> &cmpReleaseSegmentData::getOutputTotal() const
{
    return outputTotal;
}

void cmpReleaseSegmentData::setOutputTotal(const QList<float> &newOutputTotal)
{
    outputTotal = newOutputTotal;
}

const QList<float> &cmpReleaseSegmentData::getSpillExperience() const
{
    return spillExperience;
}

void cmpReleaseSegmentData::setSpillExperience(const QList<float> &newSpillExperience)
{
    spillExperience = newSpillExperience;
}

const QList<float> &cmpReleaseSegmentData::getOutputTransport() const
{
    return outputTransport;
}

void cmpReleaseSegmentData::setOutputTransport(const QList<float> &newOutputTransport)
{
    outputTransport = newOutputTransport;
}

const QList<float> &cmpReleaseSegmentData::getDelayedSurvivalRate() const
{
    return delayedSurvivalRate;
}

void cmpReleaseSegmentData::setDelayedSurvivalRate(const QList<float> &newDelayedSurvivalRate)
{
    delayedSurvivalRate = newDelayedSurvivalRate;
}

const QList<float> &cmpReleaseSegmentData::getToTransport() const
{
    return toTransport;
}

void cmpReleaseSegmentData::setToTransport(const QList<float> &newToTransport)
{
    toTransport = newToTransport;
}

const QList<float> &cmpReleaseSegmentData::getFromTransport() const
{
    return fromTransport;
}

void cmpReleaseSegmentData::setFromTransport(const QList<float> &newFromTransport)
{
    fromTransport = newFromTransport;
}

const QList<float> &cmpReleaseSegmentData::getProjectIn() const
{
    return projectIn;
}

void cmpReleaseSegmentData::setProjectIn(const QList<float> &newProjectIn)
{
    projectIn = newProjectIn;
}

const QList<float> &cmpReleaseSegmentData::getTurbineIn() const
{
    return turbineIn;
}

void cmpReleaseSegmentData::setTurbineIn(const QList<float> &newTurbineIn)
{
    turbineIn = newTurbineIn;
}

const QList<float> &cmpReleaseSegmentData::getTurbineOut() const
{
    return turbineOut;
}

void cmpReleaseSegmentData::setTurbineOut(const QList<float> &newTurbineOut)
{
    turbineOut = newTurbineOut;
}

const QList<float> &cmpReleaseSegmentData::getSluicewayIn() const
{
    return sluicewayIn;
}

void cmpReleaseSegmentData::setSluicewayIn(const QList<float> &newSluicewayIn)
{
    sluicewayIn = newSluicewayIn;
}

const QList<float> &cmpReleaseSegmentData::getSluicewayOut() const
{
    return sluicewayOut;
}

void cmpReleaseSegmentData::setSluicewayOut(const QList<float> &newSluicewayOut)
{
    sluicewayOut = newSluicewayOut;
}

const QList<float> &cmpReleaseSegmentData::getBypassIn() const
{
    return bypassIn;
}

void cmpReleaseSegmentData::setBypassIn(const QList<float> &newBypassIn)
{
    bypassIn = newBypassIn;
}

const QList<float> &cmpReleaseSegmentData::getBypassOut() const
{
    return bypassOut;
}

void cmpReleaseSegmentData::setBypassOut(const QList<float> &newBypassOut)
{
    bypassOut = newBypassOut;
}

const QList<float> &cmpReleaseSegmentData::getSpillwayIn() const
{
    return spillwayIn;
}

void cmpReleaseSegmentData::setSpillwayIn(const QList<float> &newSpillwayIn)
{
    spillwayIn = newSpillwayIn;
}

const QList<float> &cmpReleaseSegmentData::getSpillwayOut() const
{
    return spillwayOut;
}

void cmpReleaseSegmentData::setSpillwayOut(const QList<float> &newSpillwayOut)
{
    spillwayOut = newSpillwayOut;
}

const QList<float> &cmpReleaseSegmentData::getRswIn() const
{
    return rswIn;
}

void cmpReleaseSegmentData::setRswIn(const QList<float> &newRswIn)
{
    rswIn = newRswIn;
}

const QList<float> &cmpReleaseSegmentData::getRswOut() const
{
    return rswOut;
}

void cmpReleaseSegmentData::setRswOut(const QList<float> &newRswOut)
{
    rswOut = newRswOut;
}

const QList<float> &cmpReleaseSegmentData::getTransportPassed() const
{
    return transportPassed;
}

void cmpReleaseSegmentData::setTransportPassed(const QList<float> &newTransportPassed)
{
    transportPassed = newTransportPassed;
}

const QList<float> &cmpReleaseSegmentData::getTransportArrived() const
{
    return transportArrived;
}

void cmpReleaseSegmentData::setTransportArrived(const QList<float> &newTransportArrived)
{
    transportArrived = newTransportArrived;
}

delayed_mortality_table *cmpReleaseSegmentData::getD() const
{
    return d;
}

void cmpReleaseSegmentData::setD(delayed_mortality_table *newD)
{
    d = newD;
    stats.setD(newD);
}

float cmpReleaseSegmentData::getInriverLatentMort() const
{
    return inriverLatentMort;
}

void cmpReleaseSegmentData::setInriverLatentMort(float newInriverLatentMort)
{
    inriverLatentMort = newInriverLatentMort;
    stats.setInriverLatentMort(newInriverLatentMort);
}

float cmpReleaseSegmentData::getTransportLatentMort() const
{
    return transportLatentMort;
}

void cmpReleaseSegmentData::setTransportLatentMort(float newTransportLatentMort)
{
    transportLatentMort = newTransportLatentMort;
    stats.setTransportLatentMort(newTransportLatentMort);
}

float cmpReleaseSegmentData::getDifferentialReturn() const
{
    return differentialReturn;
}

void cmpReleaseSegmentData::setDifferentialReturn(float newDifferentialReturn)
{
    differentialReturn = newDifferentialReturn;
    stats.setDifferentialReturn(newDifferentialReturn);
}

float cmpReleaseSegmentData::getInriverReturn() const
{
    return inriverReturn;
}

void cmpReleaseSegmentData::setInriverReturn(float newInriverReturn)
{
    inriverReturn = newInriverReturn;
    stats.setInriverReturn(newInriverReturn);
}

float cmpReleaseSegmentData::getTransportReturn() const
{
    return transportReturn;
}

void cmpReleaseSegmentData::setTransportReturn(float newTransportReturn)
{
    transportReturn = newTransportReturn;
    stats.setTransportReturn(newTransportReturn);
}

float cmpReleaseSegmentData::getOverallReturn() const
{
    return overallReturn;
}

void cmpReleaseSegmentData::setOverallReturn(float newOverallReturn)
{
    overallReturn = newOverallReturn;
    stats.setOverallReturn(newOverallReturn);
}

float cmpReleaseSegmentData::getPostBonnevilleTotal() const
{
    return postBonnevilleTotal;
}

void cmpReleaseSegmentData::setPostBonnevilleTotal(float newPostBonnevilleTotal)
{
    postBonnevilleTotal = newPostBonnevilleTotal;
    stats.setPostBonnevilleTotal(newPostBonnevilleTotal);
}

float cmpReleaseSegmentData::getNopass() const
{
    return nopass;
}

void cmpReleaseSegmentData::setNopass(float newNopass)
{
    nopass = newNopass;
}

int cmpReleaseSegmentData::computeStats()
{

}

const cmpReleaseSegmentStats &cmpReleaseSegmentData::getStats() const
{
    return stats;
}

void cmpReleaseSegmentData::setStats(const cmpReleaseSegmentStats &newStats)
{
    stats = newStats;
}


