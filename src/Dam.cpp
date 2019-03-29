#include "Dam.h"


Dam::Dam(QString dname, QString rivName, QObject *parent) :
    RiverSegment (rivName, parent)
{
    name = new QString (dname);
    type = RiverSegment::Dam;
    clear ();
}

Dam::~Dam ()
{
}

void Dam::clear()
{
    basin = nullptr;
    species = nullptr;
    phouseSide =  (Location)-1;
    width = 0.0;
    lengthTailrace = 0.0;
    elevBase = 0.0;
    elevForebay = 0.0;
    elevTailrace = 0.0;
    fullHead = 0.0;
    depthForebay = 0.0;
    heightBypass = 0.0;
    collector = 0;
    numGates = 0;
    widthGates = 0.0;
    spillPerGate = 0.0;
    widthSpillway = 0.0;
    lengthBasin = 0.0;
    specGrav = 0.0;

    for (int i = 0; i < DAM_SLICES_IN_SEASON; i++)
    {
        spill[i] = 0.0;
        spillPlanned[i] = 0.0;
        daylightProportion[i] = 0.0;
    }

    spillPlannedDay = nullptr;
    spillPlannedNight = nullptr;
    spillLegacyPlanned = nullptr;
    spillLegacyFish = nullptr;
    spillMax = 0.0;
    spillSide = (Location)-1;
    rswSpillMax = 0.0;
    rswActive = nullptr;

    flowMax = 0.0;
    flowProjectMin = 0.0;
    flowRiverMin = 0.0;

    for (int i = 0; i < DAYS_IN_SEASON; i++)
    {
        flow[i] = 0.0;
        depthForebayDay[i] = 0.0;
        depthTailraceDay[i] = 0.0;
        dropRatioDay[i] = 0.0;
        dropRatioDayTR[i] = 0.0;
    }

    transport = nullptr;
}

void Dam::calculateFlow()
{
    calculateFlowInputs();
    calculateFlows();
}

void Dam::calculateFlows()
{

}

void Dam::calculateTemp()
{
    if (!readTemps)
    {
        calculateTempInputs();
        calculateTemps();
    }
}

void Dam::calculateTemps()
{

}

void Dam::calculateSpill()
{

}

void Dam::calculateFish()
{

}

void Dam::calculateStats()
{

}

void Dam::deleteSpill()
{

}

bool Dam::parse (CompassFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Dam data.");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->checkEnd("dam", *name);
            end = true;
        }
        else
        {
            okay = parseToken (token, cfile);
        }
    }

    return okay;
}

bool Dam::parseToken (QString token, CompassFile *cfile)
{
    bool okay = true;
    QString na ("");

    if (token.compare ("elevation_change", Qt::CaseInsensitive) == 0)
    {
//        okay = cfile->readFloatArray (elev_change);
    }
    else if (token.compare("loss_max", Qt::CaseInsensitive) == 0)
    {
//        okay = cfile->readFloatOrNa(na, loss_max);
    }
    else if (token.compare("loss_min", Qt::CaseInsensitive) == 0)
    {
//        okay = cfile->readFloatOrNa(na, loss_min);
    }
    else if (token.compare ("loss", Qt::CaseInsensitive) == 0)
    {
 //       okay = cfile->readFloatArray (loss);
    }
    else
    {
        okay = RiverSegment::parseToken (token, cfile);
    }

    return okay;
}
