#include "cmpreleasesite.h"

cmpReleaseSite::cmpReleaseSite(QString rsname)
{
    name = rsname;
    latlon = new cmpRiverPoint ();
    seg = nullptr;
}

cmpReleaseSite::~cmpReleaseSite ()
{
    delete latlon;
}

const QString &cmpReleaseSite::getName() const
{
    return name;
}

void cmpReleaseSite::setName(QString &value)
{
    name = value;
}

cmpRiverPoint *cmpReleaseSite::getLatlon() const
{
    return latlon;
}

void cmpReleaseSite::setLatlon(cmpRiverPoint *value)
{
    latlon = value;
}

void cmpReleaseSite::setSeg(cmpReach *value)
{
    seg = value;
}

cmpReach *cmpReleaseSite::getSeg() const
{
    return seg;
}

float cmpReleaseSite::getSeg_mile() const
{
    return seg_mile;
}

void cmpReleaseSite::setSeg_mile(float value)
{
    seg_mile = value;
}

