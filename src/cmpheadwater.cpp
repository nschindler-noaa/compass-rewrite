#include "cmpheadwater.h"

cmpHeadwater::cmpHeadwater (QString hname, QString rivName, QObject *parent) :
    cmpRiverSegment (rivName, parent)
{
    name = new QString(hname);
    type = cmpRiverSegment::Headwater;

    reset ();
}

void cmpHeadwater::allocateDays(int numdays)
{
    elevChange.clear();
    for (int i = 0; i < numdays; i++)
        elevChange.append(0.0);
}

void cmpHeadwater::reset()
{
    regulated = true;  // default setting
    flowCoefficient = 0.0;
    flowMean = 0.0;
    for (int i = 0; i < elevChange.count(); i++)
        elevChange[i] = 0.0;
}

void cmpHeadwater::fillRegulated()
{
    cmpRiverSegment *downseg = down;
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
//        cmpLog::outlog->add(cmpLog::Debug, msg);


    }

}

void cmpHeadwater::fillUnRegulated()
{
    cmpRiverSegment *downseg = down;
    QString msg ("");

    if (readFlows || regulated)
        return;

    msg = QString (QString ("Filling unregulated headwater %1").arg(*name));
//    cmpLog::outlog->add(cmpLog::Debug, msg);

}

void cmpHeadwater::calculateFlow()
{
    if (!readFlows)
    {
        calculateFlows();
    }
}


void cmpHeadwater::calculateFlows()
{
    int i;
    cmpRiverSegment *downseg = down;

    while (downseg != nullptr && !downseg->getIsRegPoint() && downseg->getForkSegment() != nullptr)
        downseg = downseg->getLowerSegment();

    if (downseg != nullptr || downseg->getForkSegment())
    {
        regulated = false;
    }
    else
    {
        regulated = true;
        flowCoefficient = 0.0;
        if (downseg->getType() == cmpRiverSegment::Dam)
        {

        }
    }

}

void cmpHeadwater::calculateTemp()
{
    if (!readTemps)
    { // get temps from down stream if not read in
        calculateTempInputs();
        calculateTemps();
    }
}

void cmpHeadwater::calculateTempInputs()
{

}

void cmpHeadwater::calculateTemps()
{

}

bool cmpHeadwater::parse (cmpFile *cfile)
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

bool cmpHeadwater::parseToken (QString token, cmpFile *cfile)
{
    bool okay = true;
    QString na("");

    if (token.compare ("elevation_change", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatArray (elevChange);
    }
    else if (token.compare("flow_coefficient", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, flowCoefficient);
    }
    else
    {
        okay = cmpRiverSegment::parseToken(token, cfile);
    }

    return okay;
}