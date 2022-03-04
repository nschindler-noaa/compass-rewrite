#include "cmprsw.h"

cmpRSW::cmpRSW()
{
    maxSpill = 0;
    mortality = 0;
    efficiency = nullptr;
    delay = 0;
    delayDay = 0;
    allocate(366);
}

void cmpRSW::allocate(int days)
{
    while (active.count() < days)
        active.append(false);
    while (active.count() > days)
        active.takeLast();
}

float cmpRSW::getMaxSpill() const
{
    return maxSpill;
}

void cmpRSW::setMaxSpill(float newMaxSpill)
{
    maxSpill = newMaxSpill;
}

float cmpRSW::getMortality() const
{
    return mortality;
}

void cmpRSW::setMortality(float newMortality)
{
    mortality = newMortality;
}

cmpEquation *cmpRSW::getEfficiency() const
{
    return efficiency;
}

void cmpRSW::setEfficiency(cmpEquation *newEfficiency)
{
    efficiency = newEfficiency;
}

float cmpRSW::getDelay() const
{
    return delay;
}

void cmpRSW::setDelay(float newDelay)
{
    delay = newDelay;
}

float cmpRSW::getDelayDay() const
{
    return delayDay;
}

void cmpRSW::setDelayDay(float newDelayDay)
{
    delayDay = newDelayDay;
}

bool cmpRSW::getActive(int index) const
{
    return active[index];
}

void cmpRSW::setActive(int index, const bool &newActive)
{
    int count = index + 1;
    while (active.count() < count)
        active.append(false);
    active[index] = newActive;
}
