#include "cmptransport.h"

cmpTransport::cmpTransport()
{

}

cmpRiverSegment *cmpTransport::getSource() const
{
    return source;
}

void cmpTransport::setSource(cmpRiverSegment *newSource)
{
    source = newSource;
}

cmpRiverSegment *cmpTransport::getTarget() const
{
    return target;
}

void cmpTransport::setTarget(cmpRiverSegment *newTarget)
{
    target = newTarget;
}

float cmpTransport::getRate() const
{
    return rate;
}

void cmpTransport::setRate(float newRate)
{
    rate = newRate;
}

bool cmpTransport::getByDate() const
{
    return byDate;
}

void cmpTransport::setByDate(bool newByDate)
{
    byDate = newByDate;
}

int cmpTransport::getStartDate() const
{
    return startDate;
}

void cmpTransport::setStartDate(int newStartDate)
{
    startDate = newStartDate;
}

const QList<bool> &cmpTransport::getActive() const
{
    return active;
}

void cmpTransport::setActive(const QList<bool> &newActive)
{
    active = newActive;
}

int cmpTransport::getStartCount() const
{
    return startCount;
}

void cmpTransport::setStartCount(int newStartCount)
{
    startCount = newStartCount;
}

int cmpTransport::getMaxRestarts() const
{
    return maxRestarts;
}

void cmpTransport::setMaxRestarts(int newMaxRestarts)
{
    maxRestarts = newMaxRestarts;
}

int cmpTransport::getTotalRestarts() const
{
    return totalRestarts;
}

void cmpTransport::setTotalRestarts(int newTotalRestarts)
{
    totalRestarts = newTotalRestarts;
}

int cmpTransport::getEndCount() const
{
    return endCount;
}

void cmpTransport::setEndCount(int newEndCount)
{
    endCount = newEndCount;
}

int cmpTransport::getLowDays() const
{
    return lowDays;
}

void cmpTransport::setLowDays(int newLowDays)
{
    lowDays = newLowDays;
}

int cmpTransport::getEndDate() const
{
    return endDate;
}

void cmpTransport::setEndDate(int newEndDate)
{
    endDate = newEndDate;
}

float cmpTransport::getHiFlowPassedPct() const
{
    return hiFlowPassedPct;
}

void cmpTransport::setHiFlowPassedPct(float newHiFlowPassedPct)
{
    hiFlowPassedPct = newHiFlowPassedPct;
}

float cmpTransport::getHighFlow() const
{
    return highFlow;
}

void cmpTransport::setHighFlow(float newHighFlow)
{
    highFlow = newHighFlow;
}

cmpSpecies *cmpTransport::getHiFlowSpecies() const
{
    return hiFlowSpecies;
}

void cmpTransport::setHiFlowSpecies(cmpSpecies *newHiFlowSpecies)
{
    hiFlowSpecies = newHiFlowSpecies;
}
