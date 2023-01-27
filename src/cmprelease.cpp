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

bool cmpRelease::parseData(cmpFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Release data.");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->checkEnd("release", name);
            end = true;
        }
        else
        {
            okay = parseToken (token, cfile);
        }
    }
    return okay;
}

bool cmpRelease::parseToken(QString token, cmpFile *cfile)
{
    bool okay = true;
    float tempFloat;
    QString na("");

    if (token.compare ("stock", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readString(na);
        setStockName(na);
    }
    else if (token.compare ("intial_spill_experience", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, tempFloat);
        setInitialSpillExperience(tempFloat);
    }
    else if (token.compare ("length", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, tempFloat);
        setFishLength(tempFloat);
    }
    else if (token.compare ("number", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, tempFloat);
        setNumber(1, tempFloat);
    }
    return okay;
}

void cmpRelease::writeData(cmpFile *ofile, bool outputAll)
{
    ofile->writeStringNR(0, "release", species->getName());
    ofile->writeValue(0, site->getName(), startDay);
    ofile->writeString(1, "stock", stock->getName());
    ofile->writeValue(1, "initial_spill_experience", initialSpillExperience);
    ofile->writeValue(1, "length", fishLength);
    ofile->writeStringNR(1, "number");
    ofile->writeFloatArray(1, number, Data::None, Data::Float, 0);
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

const QString &cmpRelease::getStockName() const
{
    return stockName;
}

float cmpRelease::getFishLength() const
{
    return fishLength;
}

void cmpRelease::setFishLength(float newFishLength)
{
    fishLength = newFishLength;
}

void cmpRelease::setStockName(QString stkName)
{
    stockName = stkName;
}
