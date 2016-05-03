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
    currentPoint = -1;
    width = 0.0;
    type = (SegmentType) -1;
    output_flags = 0;
    output_settings = 0;
    flow_max = 0.0;
    flow_min = 0.0;
    for (int i = 0; i < DAYS_IN_SEASON; i++)
        flow[i] = 0.0;
    for (int i = 0; i < STEPS_IN_SEASON; i++)
        temp[i] = 0.0;
    read_temps = false;
    up = (RiverSegment *)NULL;
    down = (RiverSegment *)NULL;
    fork = (RiverSegment *)NULL;
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
        okay = cfile->readFloatOrNa(na, flow_max);
    }
    else if (token.compare("flow_min", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, flow_min);
    }
    else if (token.compare("flow", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatArray(flow);
    }
    else if (token.compare ("water_temp", Qt::CaseInsensitive) == 0)
    {
        read_temps = true;
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

RiverPoint * RiverSegment::topPoint ()
{
    currentPoint = course.count () - 1;
    return course.at (currentPoint);
}

RiverPoint * RiverSegment::bottomPoint ()
{
    currentPoint = 0;
    return course.at (currentPoint);
}

RiverPoint * RiverSegment::nextPointUp ()
{
    currentPoint++;
    if (currentPoint >= course.count ())
        currentPoint = course.count () - 1;
    return course.at (currentPoint);
}

RiverPoint * RiverSegment::nextPointDn ()
{
    currentPoint--;
    if (currentPoint <= 0)
        currentPoint = 0;
    return course.at (currentPoint);
}

void RiverSegment::calculateFlow ()
{
    calculateFlowInputs();
    calculateFlows();
}

void RiverSegment::calculateFlowInputs()
{
    if (up != NULL)
    {
        up->calculateFlow();
        for (int i = 0; i < DAYS_IN_SEASON; i++)
            flow[i] = up->flow[i];

        if (fork != NULL)
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
            Log::instance()->add(Log::Fatal, msg);
        }
    }
}

void RiverSegment::calculateFlows()
{
    // unique per segment type
}

void RiverSegment::calculateTemp ()
{
    if (!read_temps)
    {
        calculateTempInputs();
        calculateTemps();
    }
}

void RiverSegment::calculateTempInputs()
{
    if (up != NULL)
    {
        up->calculateTemp();
        for (int i = 0; i < STEPS_IN_SEASON; i++)
            temp[i] = up->temp[i];

        if (fork != NULL)
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
            Log::instance()->add(Log::Fatal, msg);
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
