#include "cmpriversegment.h"
#include "cmpspecies.h"
#include "cmpstock.h"
#include "cmplog.h"
//#include "cmpDam.h"
//#include "cmpreach.h"
//#include "cmpheadwater.h"
//#include "parseUtil.h"

cmpRiverSegment::cmpRiverSegment (QObject *parent) : QObject(parent)
{
    riverName = new QString ();
    name = new QString ();
    abbrev = new QString ();
    setup ();
}

cmpRiverSegment::cmpRiverSegment (QString rivName, QObject *parent) :
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

cmpRiverSegment::cmpRiverSegment (const cmpRiverSegment &rhs) :
    QObject (rhs.parent ())
{
    riverName = new QString (*rhs.riverName);
    name = new QString (*rhs.name);
    abbrev = new QString (*rhs.abbrev);

    setup ();
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
    allocateDays();
    readTemps = false;
    up = nullptr;
    down = nullptr;
    fork = nullptr;
    temp_1 = -1;
}

cmpRiverSegment::~cmpRiverSegment ()
{
    delete riverName;
    delete name;
    delete abbrev;
    setup ();
}

bool cmpRiverSegment::parse (cmpFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");
    QString na("");

    while (okay && !end)
    {
//        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
 //           cfile->printEOF("Headwater data.");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
 //           cfile->checkEnd(QString(""), *name);
            end = true;
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
 //       okay = cfile->readFloatOrNa(na, flowMax);
    }
    else if (token.compare("flow_min", Qt::CaseInsensitive) == 0)
    {
 //       okay = cfile->readFloatOrNa(na, flowMin);
    }
    else if (token.compare("flow", Qt::CaseInsensitive) == 0)
    {
//        okay = cfile->readFloatList(flow);
    }
    else if (token.compare ("water_temp", Qt::CaseInsensitive) == 0)
    {
        readTemps = true;
//        okay = cfile->readFloatList (temp);
    }
    else if (token.compare ("output_settings", Qt::CaseInsensitive) == 0)
    {
 //       okay = cfile->readUnsigned (output_settings);
    }
    else if (token.compare ("output_flags", Qt::CaseInsensitive) == 0)
    {
 //       okay = cfile->readUnsigned (output_flags);
    }
    else if (token.compare ("output_gas", Qt::CaseInsensitive) == 0)
    {
        cfile->handle_obsolete_token(token);
    }
    else if (token.compare("latlon", Qt::CaseInsensitive) == 0)
    {
        cmpRiverPoint *pt = new cmpRiverPoint();
 //       parse_latlon(cfile, pt);
 //       addCoursePoint(pt);
    }

    else
    {
        cfile->handle_unknown_token(token);
    }

    return okay;
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

QString *cmpRiverSegment::getRiverName() const
{
    return riverName;
}

void cmpRiverSegment::setRiverName(QString *value)
{
    riverName = value;
}

QString *cmpRiverSegment::getName() const
{
    return name;
}

void cmpRiverSegment::setName(QString *value)
{
    name = value;
}

QString *cmpRiverSegment::getAbbrev() const
{
    return abbrev;
}

void cmpRiverSegment::setAbbrev(QString *value)
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
        if (type != cmpRiverSegment::Headwater)
        {
            QString msg (QString ("Segment %1 is not a headwater and has no upstream segment.")
                         .arg (*(name)));
            cmpLog::outlog->add(cmpLog::Fatal, msg);
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
}

void cmpRiverSegment::calculateTemp ()
{
    if (!readTemps)
    {
        calculateTempInputs();
        calculateTemps();
    }
}

void cmpRiverSegment::calculateTempInputs(int steps, int daysteps)
{
    int day = 0;
    int step = 0;
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
                day = step / daysteps;
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
                         .arg (*(name)));
            cmpLog::outlog->add(cmpLog::Fatal, msg);
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
