#include "Headwater.h"
#include "parseUtil.h"

Headwater::Headwater (QString hname, QString rivName, QObject *parent) :
    RiverSegment (rivName, parent)
{
    name = new QString(hname);
    type = RiverSegment::Headwater;

    clear ();
}

void Headwater::clear()
{
    regulated = true;  // default setting
    flow_coefficient = 0.0;
    flow_mean = 0.0;
    for (int i = 0; i < DAYS_IN_SEASON; i++)
        elev_change[i] = 0.0;
}

void Headwater::fillRegulated()
{
    RiverSegment *downseg = down;
    QString msg ("");

    if (readFlows)
        return;

    while (downseg != nullptr && !downseg->getIsRegPoint() && downseg->getForkSegment() == nullptr)
        downseg = downseg->getLowerSegment();

    if (downseg == nullptr || downseg->getForkSegment() != nullptr)
    {
        regulated = false;
    }
    else
    {
        regulated = true;
        msg = QString (QString ("Filling regulated headwater %1, regulated at %2").arg(*name, *downseg->getName()));
        Log::outlog->add(Log::Debug, msg);


    }

}

void Headwater::fillUnRegulated()
{
    RiverSegment *downseg = down;
    QString msg ("");

    if (readFlows || regulated)
        return;

    msg = QString (QString ("Filling unregulated headwater %1").arg(*name));
    Log::outlog->add(Log::Debug, msg);

}

void Headwater::calculateFlow()
{
    if (!readFlows)
    {
        calculateFlows();
    }
}


void Headwater::calculateFlows()
{
    int i;
    RiverSegment *downseg = down;

    while (downseg != nullptr && !downseg->getIsRegPoint() && downseg->getForkSegment() != nullptr)
        downseg = downseg->getLowerSegment();

    if (downseg != nullptr || downseg->getForkSegment())
    {
        regulated = false;
    }
    else
    {
        regulated = true;
        flow_coefficient = 0.0;
        if (downseg->getType() == RiverSegment::Dam)
        {

        }
    }

}

void Headwater::calculateTemp()
{
    if (!readTemps)
    { // get temps from down stream if not read in
        calculateTempInputs();
        calculateTemps();
    }
}

void Headwater::calculateTempInputs()
{

}

void Headwater::calculateTemps()
{

}

bool Headwater::parse (CompassFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Headwater data.");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->checkEnd("headwater", *name);
            end = true;
        }
        else
        {
            okay = parseToken(token, cfile);
        }
    }

    return okay;
}

bool Headwater::parseToken (QString token, CompassFile *cfile)
{
    bool okay = true;
    QString na("");

    if (token.compare ("elevation_change", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatArray (elev_change);
    }
    else if (token.compare("flow_coefficient", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, flow_coefficient);
    }
    else
    {
        okay = RiverSegment::parseToken(token, cfile);
    }

    return okay;
}
