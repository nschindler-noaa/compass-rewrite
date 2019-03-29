#include "RiverSegment.h"
#include "parseUtil.h"
#include "Dam.h"
#include "Species.h"
#include "Reach.h"
#include "Headwater.h"
#include "Stock.h"

RiverSegment::RiverSegment (QString rivName, QObject *parent) :
    QObject (parent)
{
    if (!rivName.isEmpty ())
    {
        riverName = new QString (rivName);
    }
    name = new QString ("");
    abbrev = new QString ("");

    setup ();
}

RiverSegment::RiverSegment (const RiverSegment &rhs) :
    QObject (rhs.parent ())
{
    riverName = new QString (*rhs.riverName);
    name = new QString (*rhs.name);
    abbrev = new QString (*rhs.abbrev);

    setup ();
}
void RiverSegment::setup ()
{
    currentPointIndex = -1;
    currentPoint = (RiverPoint *) nullptr;
    widthAve = 1.0;
    type = (SegmentType) -1;
    output_flags = 0;
    output_settings = 0;
    flowMax = 0.0;
    flowMin = 0.0;
    for (int i = 0; i < DAYS_IN_SEASON; i++)
        flow[i] = 0.0;
    for (int i = 0; i < STEPS_IN_SEASON; i++)
        temp[i] = 0.0;
    readTemps = false;
    up = (RiverSegment *)nullptr;
    down = (RiverSegment *)nullptr;
    fork = (RiverSegment *)nullptr;
    temp_1 = -1;
}

RiverSegment::~RiverSegment ()
{
    delete riverName;
    delete name;
    delete abbrev;
    setup ();
}

bool RiverSegment::parse (CompassFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");
    QString na("");

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
            cfile->checkEnd(QString(""), *name);
            end = true;
        }
    }
    return okay;
}

bool RiverSegment::parseToken(QString token, CompassFile *cfile)
{
    bool okay = true;
    QString na("");

    if (token.compare("flow_max", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, flowMax);
    }
    else if (token.compare("flow_min", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, flowMin);
    }
    else if (token.compare("flow", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatArray(flow);
    }
    else if (token.compare ("water_temp", Qt::CaseInsensitive) == 0)
    {
        readTemps = true;
        okay = cfile->readFloatArray (temp);
    }
    else if (token.compare ("output_settings", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readUnsigned (output_settings);
    }
    else if (token.compare ("output_flags", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readUnsigned (output_flags);
    }
    else if (token.compare ("output_gas", Qt::CaseInsensitive) == 0)
    {
        handle_obsolete_token(token);
    }

    else
    {
        handle_unknown_token(token);
    }

    return okay;
}

RiverPoint * RiverSegment::getCurrentPoint()
{
    return course.at (currentPointIndex);
}

RiverPoint * RiverSegment::getTopPoint ()
{
    currentPointIndex = course.count () - 1;
    return course.at (currentPointIndex);
}

RiverPoint * RiverSegment::getBottomPoint ()
{
    currentPointIndex = 0;
    return course.at (currentPointIndex);
}

RiverPoint * RiverSegment::getNextPointUp ()
{
    currentPointIndex++;
    if (currentPointIndex >= course.count ())
        currentPointIndex = course.count () - 1;
    return course.at (currentPointIndex);
}

RiverPoint * RiverSegment::getNextPointDn ()
{
    currentPointIndex--;
    if (currentPointIndex <= 0)
        currentPointIndex = 0;
    return course.at (currentPointIndex);
}

QString *RiverSegment::getRiverName() const
{
    return riverName;
}

void RiverSegment::setRiverName(QString *value)
{
    riverName = value;
}

QString *RiverSegment::getName() const
{
    return name;
}

void RiverSegment::setName(QString *value)
{
    name = value;
}

QString *RiverSegment::getAbbrev() const
{
    return abbrev;
}

void RiverSegment::setAbbrev(QString *value)
{
    abbrev = value;
}

QList<Tributary *> RiverSegment::getTributaries() const
{
    return tributaries;
}

QList<RiverPoint *> RiverSegment::getCourse() const
{
    return course;
}

bool RiverSegment::getRegPoint() const
{
    return isRegPoint;
}

void RiverSegment::setRegPoint(bool value)
{
    isRegPoint = value;
}

bool RiverSegment::getReadFlows() const
{
    return readFlows;
}

void RiverSegment::setReadFlows(bool value)
{
    readFlows = value;
}

float RiverSegment::getFlowMax() const
{
    return flowMax;
}

void RiverSegment::setFlowMax(float value)
{
    flowMax = value;
}

float RiverSegment::getFlowMin() const
{
    return flowMin;
}

void RiverSegment::setFlowMin(float value)
{
    flowMin = value;
}

RiverSegment::FlowLocation RiverSegment::getMainFlow() const
{
    return mainFlow;
}

void RiverSegment::setMainFlow(FlowLocation loc)
{
    mainFlow = loc;
    if (loc == FlowLocation::Right)
        otherFlow = FlowLocation::Left;
    else
        otherFlow = FlowLocation::Right;
}

RiverSegment::FlowLocation RiverSegment::getOtherFlow() const
{
    return otherFlow;
}

void RiverSegment::setOtherFlow(FlowLocation loc)
{
    otherFlow = loc;
    if (loc == FlowLocation::Right)
        mainFlow = FlowLocation::Left;
    else
        mainFlow = FlowLocation::Right;
}

bool RiverSegment::getReadTemps() const
{
    return readTemps;
}

void RiverSegment::setReadTemps(bool value)
{
    readTemps = value;
}

RiverSegment::SegmentType RiverSegment::getType() const
{
    return type;
}

void RiverSegment::setType(const RiverSegment::SegmentType &value)
{
    type = value;
}

bool RiverSegment::getIsRegPoint() const
{
    return isRegPoint;
}

void RiverSegment::setIsRegPoint(bool value)
{
    isRegPoint = value;
}

float RiverSegment::getWidthUpper() const
{
    return widthUpper;
}

void RiverSegment::setWidthUpper(float value)
{
    widthUpper = value;
}

float RiverSegment::getWidthAve() const
{
    return widthAve;
}

void RiverSegment::setWidthAve(float value)
{
    widthAve = value;
}

float RiverSegment::getWidthLower() const
{
    return widthLower;
}

void RiverSegment::setWidthLower(float value)
{
    widthLower = value;
}

float RiverSegment::getElevUpper() const
{
    return elevUpper;
}

void RiverSegment::setElevUpper(float value)
{
    elevUpper = value;
}

float RiverSegment::getElevLower() const
{
    return elevLower;
}

void RiverSegment::setElevLower(float value)
{
    elevLower = value;
}

void RiverSegment::calculateFlow ()
{
    calculateFlowInputs();
    calculateFlows();
}

void RiverSegment::calculateFlowInputs()
{
    if (up != nullptr)
    {
        up->calculateFlow();
        for (int i = 0; i < DAYS_IN_SEASON; i++)
            flow[i] = up->flow[i];

        if (fork != nullptr)
        {
            fork->calculateFlow();
            for (int i = 0; i < DAYS_IN_SEASON; i++)
                flow[i] = up->flow[i] + fork->flow[i];
        }
    }
    else // ends at all headwaters - already calculated
    {
        if (type != RiverSegment::Headwater)
        {
            QString msg (QString ("Segment %1 is not a headwater and has no upstream segment.")
                         .arg (*(name)));
            Log::outlog->add(Log::Fatal, msg);
        }
    }
}

void RiverSegment::calculateFlows()
{
    // unique per segment type
}

void RiverSegment::calculateTemp ()
{
    if (!readTemps)
    {
        calculateTempInputs();
        calculateTemps();
    }
}

void RiverSegment::calculateTempInputs()
{
    if (up != nullptr)
    {
        up->calculateTemp();
        for (int i = 0; i < STEPS_IN_SEASON; i++)
            temp[i] = up->temp[i];

        if (fork != nullptr)
        {
            fork->calculateTemp();
            for (int i = 0; i < STEPS_IN_SEASON; i++)
                temp[i] = ((up->temp[i] * up->flow[i/2]) +
                           (fork->temp[i] * fork->flow[i/2])) /
                           (up->flow[i/2] + fork->flow[i/2]);
        }
    }
    else  // ends at all headwaters - already calculated
    {
        if (type != RiverSegment::Headwater)
        {
            QString msg (QString ("Segment %1 is not a headwater and has no upstream segment.")
                         .arg (*(name)));
            Log::outlog->add(Log::Fatal, msg);
        }
    }
}

void RiverSegment::calculateTemps()
{
    // unique per segment type
}

void RiverSegment::calculateFish ()
{
    // do nothing in default case
}

void RiverSegment::calculateStats ()
{
    // do nothing in default case
}
