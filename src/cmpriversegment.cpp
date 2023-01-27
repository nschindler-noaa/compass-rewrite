#include "cmpriversegment.h"
#include "cmpspecies.h"
#include "cmpstock.h"
#include "cmplog.h"
//#include "cmpDam.h"
//#include "cmpreach.h"
//#include "cmpheadwater.h"
//#include "parseUtil.h"

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

cmpRiverSegment::cmpRiverSegment (QString segName, cmpRiver *parent) :
    QObject (parent)
{
    if (parent == nullptr)
        riverName = QString ();
    else
        riverName = parent->getName();
    name = segName;
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
    outputFlags = rhs.outputFlags;
    outputSettings = rhs.outputSettings;
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

float cmpRiverSegment::getGasTheta() const
{
    return gasTheta;
}

void cmpRiverSegment::setGasTheta(float newGasTheta)
{
    gasTheta = newGasTheta;
}

int cmpRiverSegment::getOutputSettings() const
{
    return outputSettings;
}

void cmpRiverSegment::setOutputSettings(unsigned int newOutputSettings)
{
    outputSettings = newOutputSettings;
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
    widthLower = widthAve;
    widthUpper = widthAve;
    depthLower = 1.0;
    depthUpper = 1.0;
    elevLower = 0.0;
    elevUpper = 0.0;
//    type = Reach;
    outputFlags = 0;
    outputSettings = 0;
    flowMax = 0.0;
    flowMin = 0.0;
    temp.append(0);
    readFlows = false;
    flow.append(0);
    daysPerYear = 366;
    stepsPerDay = 2;
    setDaysPerSeason(366);
    isRegPoint = false;
    readTemps = false;
    up = nullptr;
    down = nullptr;
    fork = nullptr;
    temp_1 = -1;
    readGas = false;
    gas_out = nullptr;
    readTurbidity = false;
}

cmpRiverSegment::~cmpRiverSegment ()
{
    setup ();
}

void cmpRiverSegment::resetData()
{
    outputFlags = 0;
    outputSettings = 0;
    setDaysPerSeason(366);
    temp_1 = -1;
}

bool cmpRiverSegment::parseData (cmpFile *cfile)
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
       okay = cfile->readInt(outputSettings);
    }
    else if (token.compare ("output_flags", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readInt(outputFlags);
    }
    else if (token.compare ("gas_theta", Qt::CaseInsensitive) == 0)
    {
        cfile->readFloatOrNa(na, gasTheta);
    }
    else if (token.compare ("output_gas", Qt::CaseInsensitive) == 0)
    {
        cfile->obsoleteToken(token);
    }
    else if (token.startsWith("#"))
    {
        cfile->skipLine();
    }
    else
    {
        cfile->unknownToken(token, name);
    }

    return okay;
}

void cmpRiverSegment::outputData(cmpFile *outfile, bool outputAll)
{
    writeAllData(outfile, 0, outputAll);
}

void cmpRiverSegment::writeAllData(cmpFile *outfile, int indent, bool outputAll)
{
    outfile->writeString(indent, "segment", name);
    writeConfigData(outfile, indent+1, outputAll);
    writeFlowData(outfile, indent+1, outputAll);
    writeGasData(outfile, indent+1, outputAll);
    writeTempData(outfile, indent+1, outputAll);
    writeTurbidData(outfile, indent+1, outputAll);
    outfile->writeEnd(indent, "segment", name);
}

void cmpRiverSegment::writeConfigData(cmpFile *outfile, int indent, bool outputAll)
{
    outfile->writeValue(indent, "output_settings", outputSettings);
}

void cmpRiverSegment::writeFlowData(cmpFile *outfile, int indent, bool outputAll)
{
    float fdef = (outputAll? 100000: 0);
    outfile->writeValue(indent, "flow_max", getFlowMax(), fdef);
    if (readFlows)
    {
        outfile->writeStringNR(indent, "flow");
        outfile->writeFloatArray(indent, flow, Data::None, Data::Fixed, fdef);
    }
}

void cmpRiverSegment::writeGasData (cmpFile *outfile, int indent, bool outputAll)
{
    if (readGas)
    {
//        float fdef = (outputAll? 100000: 0);
        outfile->writeString(indent, "output_gas", "on");
//        if (initialGas.isEmpty())
//        {
//            gas_out->writeData(outfile, indent, outputAll);
//        }
//        else
//        {
//            outfile->writeStringNR(indent, "initial_gas");
//            outfile->writeFloatArray(indent, &gas_out, gas_out.count(), Data::None, Data::Fixed, fdef);
//        }
    }
    else
    {
        outfile->writeString(indent, "output_gas", "off");
    }
}

void cmpRiverSegment::writeTempData (cmpFile *outfile, int indent, bool outputAll)
{
    if (readTemps)
    {
        float fdef = (outputAll? 100000: 0);
        outfile->writeStringNR(indent, "water_temp");
        outfile->writeFloatArray(indent, temp, Data::None, Data::Fixed, fdef);
    }
}

void cmpRiverSegment::writeTurbidData (cmpFile *outfile, int indent, bool outputAll)
{
//    float fdef = (outputAll? 100000: 0);
    if (readTurbidity)
    {
        outfile->writeString(indent, "input_turbidity", "on");
//        outfile->writeStringNR(indent, "input_turbidity");
//        outfile->writeFloatArray(indent, &turbidity, turbidity.count(), Data::None, Data::Fixed, fdef);
    }
    else
    {
        outfile->writeString(indent, "input_turbidity", "off");
    }
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
            descfile->printEOF("Headwater description");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            descfile->checkEnd(QString(), name);
            end = true;
        }
        else
        {
            okay = parseDescToken(token, descfile);
        }
    }
    return okay;
}

bool cmpRiverSegment::parseDescToken(QString token, cmpFile *descfile)
{
    bool okay = true;
    QString na("");

    if (token.compare("abbrev", Qt::CaseInsensitive) == 0)
    {
        okay = descfile->readString(abbrev);
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
    else if (token.startsWith("#"))
    {
        descfile->skipLine();
    }
    else
    {
        descfile->unknownToken(token, name);
    }

    return okay;
}

void cmpRiverSegment::outputDesc(cmpFile *ofile)
{
    int total = course.count();
    ofile->writeString(1, "null", name);
    for (int i = 0; i < total; i++)
        ofile->writeString(3, QString("latlon"), course.at(i)->getLatLon());
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

cmpRiver *cmpRiverSegment::getRiver()
{
    return static_cast<cmpRiver *>(parent());
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

const QString &cmpRiverSegment::getTypeStr() const
{
    return typeStr;
}

cmpRiverSegment::SegmentType cmpRiverSegment::getType()
{
    return type;
}

void cmpRiverSegment::setType(const cmpRiverSegment::SegmentType &value)
{
    type = value;
    switch (type)
    {
    case Dam:
        typeStr = QString("dam");
        break;
    case Reach:
        typeStr = QString ("reach");
        break;
    case Headwater:
        typeStr = QString ("headwater");
        break;
    }
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
            cmpLog::outlog->add(cmpLog::Fatal, msg);
        }
    }
}

void cmpRiverSegment::allocateDays(int days)
{
    if (!flow.isEmpty())
        flow.clear();
    if (!temp.isEmpty())
        temp.clear();

    for (int i = 0; i < days; i++)
    {
        flow.append(0.0);
        temp.append(0.0);
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
