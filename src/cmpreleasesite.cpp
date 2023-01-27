#include "cmpreleasesite.h"

cmpReleaseSite::cmpReleaseSite(QString rsname)
{
    name = rsname;
    latlon = nullptr;
    seg = nullptr;
}

cmpReleaseSite::~cmpReleaseSite ()
{
    delete latlon;
}

bool cmpReleaseSite::parseDesc(cmpFile *cfile)
{
    bool okay = true;
    QString line;
    line = cfile->getToken();
    if (line.compare("latlon") == 0)
    {
        okay = cfile->readString(line);
        cmpRiverPoint *rivpt = new cmpRiverPoint(line);
        setLatlon(rivpt);
        line = cfile->getToken();
        if (line.compare("end") != 0)
            okay = false;
        else
            okay = cfile->checkEnd ("release_site", getName());
    }
    else
    {
        okay = false;
    }
    return okay;
}

bool cmpReleaseSite::outputDesc(cmpFile *cfile)
{
    bool okay = (latlon != nullptr);
    QString namestr = name;
    if (okay)
    {
        cfile->writeString(0, QString("release_site"), namestr.replace('_', ' '));
        cfile->writeString(1, QString("latlon"), latlon->getLatLon());
        cfile->writeEnd (0, QString("release_site"), namestr);
    }
    return okay;
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
    if (latlon != nullptr)
        delete latlon;
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

