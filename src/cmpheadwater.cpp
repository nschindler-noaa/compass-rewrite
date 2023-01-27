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

void cmpHeadwater::allocateDays(int numdays)
{
    elevChange.clear();
    for (int i = 0; i < numdays; i++)
        elevChange.append(0.0);
    cmpRiverSegment::allocateDays(numdays);
}

void cmpHeadwater::resetData()
{
    isRegPoint = true;  // default setting
    flowCoefficient = 0.0;
    flowMean = 0.0;
    allocateDays(366);
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

void cmpHeadwater::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    int indent2 = indent + 1;
    float fdef = outputAll? 100000: 0;
    outfile->writeString(indent, "headwater", name);
    writeConfigData(outfile, indent2, outputAll);
    outfile->writeValue(indent2, "flow_mean", getFlowMean(), fdef);// 0.00
    outfile->writeValue(indent2, "flow_max", getFlowMax(), fdef);// 26.00
    // output flow
    writeFlowData(outfile, indent2, outputAll);
    // output gas
    writeGasData(outfile, indent2, outputAll);
    // output temp
    writeTempData(outfile, indent2, outputAll);
    // output turbidity
    writeTurbidData(outfile, indent2, outputAll);
    outfile->writeEnd(indent, "headwater", name);
}
