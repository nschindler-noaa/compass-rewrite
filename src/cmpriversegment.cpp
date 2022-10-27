#include "cmpriversegment.h"
#include "cmpspecies.h"
#include "cmpstock.h"
//#include "cmpdam.h"
//#include "cmpreach.h"
//#include "cmpheadwater.h"
#include "parseUtil.h"

#include "cmpriver.h"

cmpRiverSegment::cmpRiverSegment (cmpRiver *parent) : QObject(parent)
{
    if (parent == nullptr)
        riverName = QString ();
    else
        riverName = parent->getName();
    name = QString ();
    abbrev = QString ();
    setup ();
}

cmpRiverSegment::cmpRiverSegment (QString reachName, cmpRiver *parent) :
    QObject (parent)
{
    if (parent == nullptr)
        riverName = QString ();
    else
        riverName = parent->getName();
    name = QString ();
    abbrev = QString ();

    setup ();
}

cmpRiverSegment::cmpRiverSegment (const cmpRiverSegment &rhs) :
    QObject (rhs.parent ())
{
    copy(rhs);
}

cmpRiverSegment &cmpRiverSegment::operator =(const cmpRiverSegment &rhs)
{
    copy(rhs);
    return *this;
}

cmpRiverSegment &cmpRiverSegment::copy (const cmpRiverSegment &rhs)
{
    riverName = QString (rhs.riverName);
    name = QString (rhs.name);
    abbrev = QString (rhs.abbrev);
    currentPointIndex = -1;
    currentPoint = rhs.currentPoint;
    widthAve = rhs.widthAve;
    type = rhs.type;
    output_flags = rhs.output_flags;
    output_settings = rhs.output_settings;
    flowMax = rhs.flowMax;
    flowMin = rhs.flowMin;
    for (int i = 0, total = rhs.temp.count(); i < total; i++)
        temp[i] = rhs.temp.at(i);
    for (int i = 0, total = rhs.flow.count(); i < total; i++)
        flow[i] = rhs.flow.at(i);
    setDaysPerSeason(rhs.daysPerSeason);
    readTemps = rhs.readTemps;
    up = rhs.up;
    down = rhs.down;
    fork = rhs.fork;
    temp_1 = -1;

    return *this;
}

int cmpRiverSegment::getStepsPerDay() const
{
    return stepsPerDay;
}

void cmpRiverSegment::setStepsPerDay(int newStepsPerDay)
{
    stepsPerDay = newStepsPerDay;
}

int cmpRiverSegment::getDaysPerYear() const
{
    return daysPerYear;
}

void cmpRiverSegment::setDaysPerYear(int newDaysPerYear)
{
    daysPerYear = newDaysPerYear;
}

int cmpRiverSegment::getDaysPerSeason() const
{
    return daysPerSeason;
}

void cmpRiverSegment::setDaysPerSeason(int newDaysPerSeason)
{
    daysPerSeason = newDaysPerSeason;
    allocateDays(daysPerSeason);
}

void cmpRiverSegment::setup ()
{
    currentPointIndex = -1;
    currentPoint = nullptr;
    widthAve = 1.0;
    type = Null;
    output_flags = 0;
    output_settings = 0;
    flowMax = 0.0;
    flowMin = 0.0;
    temp.append(0);
    flow.append(0);
    setDaysPerSeason(366);
    readTemps = false;
    up = nullptr;
    down = nullptr;
    fork = nullptr;
    temp_1 = -1;
}

cmpRiverSegment::~cmpRiverSegment ()
{
    setup ();
}

bool cmpRiverSegment::parse (cmpFile *cfile)
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
            cfile->checkEnd(QString(), name);
            end = true;
        }
        else
        {
            okay = parseToken(token, cfile);
        }
    }
    return okay;
}

bool cmpRiverSegment::parseToken(QString token, cmpFile *cfile)
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
        okay = cfile->readFloatList(flow, daysPerSeason, Data::Space, 1, "flow");
    }
    else if (token.compare ("water_temp", Qt::CaseInsensitive) == 0)
    {
        readTemps = true;
        okay = cfile->readFloatList (temp, daysPerSeason, Data::Duplicate, stepsPerDay, "water_temp");
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
        cfile->obsoleteToken(token, name);
    }
    else
    {
        cfile->unknownToken(token, name);
    }

    return okay;
}

bool cmpRiverSegment::parseDesc(cmpFile *descfile)
{
    bool okay = true, end = false;
    QString token ("");
    QString na("");

    while (okay && !end)
    {
        token = descfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            descfile->printEOF("Headwater data.");
            okay = false;
        }
        else if (token.compare("flow_max", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, flowMax);
        }
        else if (token.compare("flow_min", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, flowMin);
        }
        else if (token.compare("latlon", Qt::CaseInsensitive) == 0)
        {
            cmpRiverPoint *pt = new cmpRiverPoint();
            okay = descfile->readString(token);
            pt->parse(token);
            addCoursePoint(pt);
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            descfile->checkEnd(QString(), name);
            end = true;
        }
        else
        {
            descfile->unknownToken(token, name);
        }
    }
    return okay;
}

void cmpRiverSegment::outputDesc(cmpFile *ofile)
{
    ofile->writeString(1, "null", name);
    ofile->writeEnd(1, "null", name);
}

cmpRiverPoint * cmpRiverSegment::getCurrentPoint()
{
    return course.at (currentPointIndex);
}

cmpRiverPoint * cmpRiverSegment::getTopPoint ()
{
    currentPointIndex = course.count () - 1;
    return course.at (currentPointIndex);
}

cmpRiverPoint * cmpRiverSegment::getBottomPoint ()
{
    currentPointIndex = 0;
    return course.at (currentPointIndex);
}

cmpRiverPoint * cmpRiverSegment::getNextPointUp ()
{
    currentPointIndex++;
    if (currentPointIndex >= course.count ())
        currentPointIndex = course.count () - 1;
    return course.at (currentPointIndex);
}

cmpRiverPoint * cmpRiverSegment::getNextPointDn ()
{
    currentPointIndex--;
    if (currentPointIndex <= 0)
        currentPointIndex = 0;
    return course.at (currentPointIndex);
}

QString cmpRiverSegment::getRiverName() const
{
    return riverName;
}

void cmpRiverSegment::setRiverName(QString value)
{
    riverName = value;
}

QString cmpRiverSegment::getName() const
{
    return name;
}

void cmpRiverSegment::setName(QString value)
{
    name = value;
}

QString cmpRiverSegment::getAbbrev() const
{
    return abbrev;
}

void cmpRiverSegment::setAbbrev(QString value)
{
    abbrev = value;
}

const QList<cmpTributary *> &cmpRiverSegment::getTributaries() const
{
    return tributaries;
}

QList<cmpRiverPoint *> cmpRiverSegment::getCourse() const
{
    return course;
}

bool cmpRiverSegment::addCoursePoint(cmpRiverPoint *pt)
{
    bool okay = true;
    int num = course.count();
    course.append(pt);
    if (course.count() == num)
        okay = false;
    return okay;
}

bool cmpRiverSegment::getRegPoint() const
{
    return isRegPoint;
}

void cmpRiverSegment::setRegPoint(bool value)
{
    isRegPoint = value;
}

bool cmpRiverSegment::getReadFlows() const
{
    return readFlows;
}

void cmpRiverSegment::setReadFlows(bool value)
{
    readFlows = value;
}

float cmpRiverSegment::getFlowMax() const
{
    return flowMax;
}

void cmpRiverSegment::setFlowMax(float value)
{
    flowMax = value;
}

float cmpRiverSegment::getFlowMin() const
{
    return flowMin;
}

void cmpRiverSegment::setFlowMin(float value)
{
    flowMin = value;
}

cmpRiverSegment::FlowLocation cmpRiverSegment::getMainFlow() const
{
    return mainFlow;
}

void cmpRiverSegment::setMainFlow(FlowLocation loc)
{
    mainFlow = loc;
    if (loc == FlowLocation::FlowRight)
        otherFlow = FlowLocation::FlowLeft;
    else
        otherFlow = FlowLocation::FlowRight;
}

cmpRiverSegment::FlowLocation cmpRiverSegment::getOtherFlow() const
{
    return otherFlow;
}

void cmpRiverSegment::setOtherFlow(FlowLocation loc)
{
    otherFlow = loc;
    if (loc == FlowLocation::FlowRight)
        mainFlow = FlowLocation::FlowLeft;
    else
        mainFlow = FlowLocation::FlowRight;
}

bool cmpRiverSegment::getReadTemps() const
{
    return readTemps;
}

void cmpRiverSegment::setReadTemps(bool value)
{
    readTemps = value;
}

cmpRiverSegment::SegmentType cmpRiverSegment::getType() const
{
    return type;
}

void cmpRiverSegment::setType(const cmpRiverSegment::SegmentType &value)
{
    type = value;
}

bool cmpRiverSegment::getIsRegPoint() const
{
    return isRegPoint;
}

void cmpRiverSegment::setIsRegPoint(bool value)
{
    isRegPoint = value;
}

float cmpRiverSegment::getWidthUpper() const
{
    return widthUpper;
}

void cmpRiverSegment::setWidthUpper(float value)
{
    widthUpper = value;
}

float cmpRiverSegment::getWidthAve() const
{
    return widthAve;
}

void cmpRiverSegment::setWidthAve(float value)
{
    widthAve = value;
}

float cmpRiverSegment::getWidthLower() const
{
    return widthLower;
}

void cmpRiverSegment::setWidthLower(float value)
{
    widthLower = value;
}

float cmpRiverSegment::getElevUpper() const
{
    return elevUpper;
}

void cmpRiverSegment::setElevUpper(float value)
{
    elevUpper = value;
}

float cmpRiverSegment::getElevLower() const
{
    return elevLower;
}

void cmpRiverSegment::setElevLower(float value)
{
    elevLower = value;
}

int cmpRiverSegment::getTemp_1() const
{
    return temp_1;
}

void cmpRiverSegment::setTemp_1(int newTemp_1)
{
    temp_1 = newTemp_1;
}

void cmpRiverSegment::calculateFlow ()
{
    calculateFlowInputs();
    calculateFlows();
}

void cmpRiverSegment::calculateFlowInputs()
{
    if (up != nullptr)
    {
        up->calculateFlow();
        for (int i = 0; i < daysPerSeason; i++)
            flow[i] = up->flow[i];

        if (fork != nullptr)
        {
            fork->calculateFlow();
            for (int i = 0; i < daysPerSeason; i++)
                flow[i] = up->flow[i] + fork->flow[i];
        }
    }
    else // ends at all headwaters - already calculated
    {
        if (type != cmpRiverSegment::Headwater)
        {
            QString msg (QString ("Segment %1 is not a headwater and has no upstream segment.")
                         .arg (name));
//            cmpLog::outlog->add(Log::Fatal, msg);
        }
    }
}

void cmpRiverSegment::allocateDays(int days)
{
    while (flow.count() < days)
        flow.append(0);
    while (flow.count() > days)
        flow.takeLast();
    while (temp.count() < days)
        temp.append(0);
    while (temp.count() > days)
        temp.takeLast();
    for (int i = 0; i < days; i++)
    {
        flow[i] = 0;
        temp[i] = 0;
    }
}

void cmpRiverSegment::calculateFlows()
{
    // unique per segment type
    if (up != nullptr)
    {
        up->calculateFlows();
        for (int i = 0; i < flow.count(); i++)
            flow[i] = up->flow[i];
        if (fork != nullptr)
        {
            fork->calculateFlows();
            for (int i = 0; i < flow.count(); i++)
                flow[i] += fork->flow[i];
        }
    }
}

void cmpRiverSegment::calculateTemp ()
{
    if (!readTemps)
    {
        calculateTempInputs();
        calculateTemps();
    }
}

void cmpRiverSegment::calculateTempInputs()
{
    int day = 0;
    int step = 0;
    int steps = temp.count();

    if (up != nullptr)
    {
        up->calculateTemp();
        for (step = 0; step < steps; step++)
        {
            temp[step] = up->temp[step];
        }

        if (fork != nullptr)
        {
            fork->calculateTemp();
            for (step = 0; step < steps; step++)
            {
                day = step / stepsPerDay;
                temp[step] = ((up->temp[step] * up->flow[day]) +
                             (fork->temp[step] * fork->flow[day])) /
                             (up->flow[day] + fork->flow[day]);
            }
        }
    }
    else  // ends at all headwaters - already calculated
    {
        if (type != cmpRiverSegment::Headwater)
        {
            QString msg (QString ("Segment %1 is not a headwater and has no upstream segment.")
                         .arg (name));
//            cmpLog::outlog->add(Log::Fatal, msg);
        }
    }
}

void cmpRiverSegment::calculateTemps()
{
    // unique per segment type
}

void cmpRiverSegment::calculateFish ()
{
    // do nothing in default case
}

void cmpRiverSegment::calculateStats ()
{
    // do nothing in default case
}
