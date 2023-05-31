#include "cmpheadwater.h"
#include "cmpriver.h"

cmpHeadwater::cmpHeadwater (cmpRiver *parent) :
    cmpRiverSegment(parent)
{
    if (parent != nullptr)
        riverName = parent->getName();
    name = QString();
    type = cmpRiverSegment::Headwater;
    typeStr = "headwater";
    resetData();
}

cmpHeadwater::cmpHeadwater (QString hname, cmpRiver *parent) :
    cmpRiverSegment (parent)
{
    if (parent != nullptr)
        riverName = parent->getName();
    name = QString(hname);
    type = cmpRiverSegment::Headwater;
    typeStr = "headwater";
    resetData ();
}

void cmpHeadwater::allocate(int numdays, int numsteps, int gasSteps)
{
    elevChange.clear();
    for (int i = 0; i < numdays; i++)
        elevChange.append(0.0);
    cmpRiverSegment::allocate(numdays, numsteps, gasSteps);
}

void cmpHeadwater::resetData()
{
    isRegPoint = true;  // default setting
    flowCoefficient = 0.0;
    flowMean = 0.0;
    allocate(daysPerSeason, stepsPerDay, gasStepsPerDay);
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
        isRegPoint = false;
    }
    else
    {
        isRegPoint = true;
        msg = QString (QString ("Filling regulated headwater %1, regulated at %2").arg(name, downseg->getName()));
//        cmpLog::outlog->add(cmpLog::Debug, msg);


    }

}

void cmpHeadwater::fillUnRegulated()
{
    cmpRiverSegment *downseg = down;
    QString msg ("");

    if (readFlows || isRegPoint)
        return;

    msg = QString (QString ("Filling unregulated headwater %1").arg(name));
//    cmpLog::outlog->add(cmpLog::Debug, msg);

}

int cmpHeadwater::calculateFlow()
{
    if (!readFlows)
    {
        return calculateFlows();
    }
}


int cmpHeadwater::calculateFlows()
{
    int i;
    cmpRiverSegment *downseg = down;

    while (downseg != nullptr && !downseg->getIsRegPoint() && downseg->getForkSegment() != nullptr)
        downseg = downseg->getLowerSegment();

    if (downseg != nullptr || downseg->getForkSegment())
    {
        isRegPoint = false;
    }
    else
    {
        isRegPoint = true;
        flowCoefficient = 0.0;
        if (downseg->getType() == cmpRiverSegment::Dam)
        {

        }
    }

}

int cmpHeadwater::calculateTemp()
{
    int retval = 0;
    if (!readTemps)
    { // get temps from down stream if not read in
//        calculateTempInputs();
        retval = calculateTemps();
    }
    return retval;
}

void cmpHeadwater::calculateTempInputs()
{

}

int cmpHeadwater::calculateTemps()
{

}

float cmpHeadwater::getFlowMean() const
{
    return flowMean;
}

void cmpHeadwater::setFlowMean(float newFlowMean)
{
    flowMean = newFlowMean;
}

void cmpHeadwater::outputDesc(cmpFile *ofile)
{
    int total = course.count();
    if (total > 1)
    {
        QString namestr = name;
        ofile->writeString(1, "headwater", namestr.replace('_', ' '));
        for (int i = 0; i < total; i++)
            ofile->writeString(3, QString("latlon"), course.at(i)->getLatLon());
        ofile->writeEnd(1, "null", namestr);
    }
}



bool cmpHeadwater::parseData (cmpFile *cfile)
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
            okay = cfile->checkEnd("headwater", name);
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
        okay = cfile->readFloatArray (elevChange, daysPerSeason, Data::None, stepsPerDay, "elevation_change");
    }
    else if (token.compare("flow_mean", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, flowMean);
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

void cmpHeadwater::writeFlowData(cmpFile *outfile, int indent, bool outputAll)
{
    float fdef = (outputAll? 100000: 0);
    outfile->writeValue(indent, "flow_mean", getFlowMean(), Data::Fixed, fdef);
    cmpRiverSegment::writeFlowData(outfile, indent, outputAll);
//    outfile->writeValue(indent, "flow_max", getFlowMax(), Data::Fixed, fdef);
//    if (readFlows)
//    {
//        outfile->writeFloatArray(indent, "flow", "", flow, Data::None, stepsPerDay, Data::Float, fdef);
//    }
}


void cmpHeadwater::writeRivData (cmpFile *outfile, int indent, bool outputAll)
{
    float fdef = outputAll? 1000000: 0.0;
    // output flow array
    writeFlowData(outfile, indent, outputAll);
    // output temp
    writeTempData(outfile, indent, outputAll);
    // output gas
    writeGasData(outfile, indent, outputAll);
    // output turbidity
    writeTurbidData(outfile, indent, outputAll);
}

void cmpHeadwater::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    int indent2 = indent + 1;
    float fdef = outputAll? 1000000: 0;
    outfile->writeString(indent, "headwater", name);
    writeConfigData(outfile, indent2, outputAll);
    writeRivData(outfile, indent2, outputAll);
    outfile->writeEnd(indent, "headwater", name);
}
