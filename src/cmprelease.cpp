#include "cmprelease.h"

cmpRelease::cmpRelease()
{
    active = false;
    site = nullptr;
    species = nullptr;
    stock = nullptr;
    startDay = 0;
#ifdef REALTIME
    rtinfo = nullptr;
#endif
    addSetting = 0;
    dirtyFlag = false;
    initialSpillExperience = 1;
    totalReleased = 0;
    migrOnsetMedian = 0;
}

const QString &cmpRelease::getName() const
{
    return name;
}

void cmpRelease::setName(const QString &newName)
{
    name = newName;
}

void cmpRelease::setSite(cmpReleaseSite *newSite)
{
    site = newSite;
}

void cmpRelease::setSpecies(cmpSpecies *newSpecies)
{
    species = newSpecies;
}

void cmpRelease::setStock(cmpStock *newStock)
{
    stock = newStock;
}

int cmpRelease::getStartDay() const
{
    return startDay;
}

void cmpRelease::setStartDay(int newStartDay)
{
    startDay = newStartDay;
}

RtInfo *cmpRelease::getRtinfo() const
{
    return rtinfo;
}

void cmpRelease::setRtinfo(RtInfo *newRtinfo)
{
    rtinfo = newRtinfo;
}

const float &cmpRelease::getNumber(int i) const
{
    if (i < number.count())
       return number.at(i);
    return -1.;
}

void cmpRelease::setNumber(int i, const float &newNumber)
{
    while (number.count() < i)
        number.append(0);

    number[i] = newNumber;
}

int cmpRelease::getAddSetting() const
{
    return addSetting;
}

void cmpRelease::setAddSetting(int newAddSetting)
{
    addSetting = newAddSetting;
}

bool cmpRelease::getDirtyFlag() const
{
    return dirtyFlag;
}

void cmpRelease::setDirtyFlag(bool newDirtyFlag)
{
    dirtyFlag = newDirtyFlag;
}

float cmpRelease::getInitialSpillExperience() const
{
    return initialSpillExperience;
}

void cmpRelease::setInitialSpillExperience(float newInitialSpillExperience)
{
    initialSpillExperience = newInitialSpillExperience;
}

float cmpRelease::getTotalReleased() const
{
    return totalReleased;
}

void cmpRelease::setTotalReleased(float newTotalReleased)
{
    totalReleased = newTotalReleased;
}

const QList<cmpReleaseSegmentData *> &cmpRelease::getRelSegments() const
{
    return relSegments;
}

void cmpRelease::setRelSegments(const QList<cmpReleaseSegmentData *> &newRelSegments)
{
    relSegments = newRelSegments;
}

const cmpReleaseSegmentStats &cmpRelease::getStats() const
{
    return stats;
}

void cmpRelease::setStats(const cmpReleaseSegmentStats &newStats)
{
    stats = newStats;
}

float cmpRelease::getMigrOnsetMedian() const
{
    return migrOnsetMedian;
}

void cmpRelease::setMigrOnsetMedian(float newMigrOnsetMedian)
{
    migrOnsetMedian = newMigrOnsetMedian;
}
