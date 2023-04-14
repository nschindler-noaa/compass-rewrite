#include "cmpriversegment.h"
#include "cmpspecies.h"
#include "cmpstock.h"
#include "cmplog.h"
//#include "cmpDam.h"
//#include "cmpreach.h"
//#include "cmpheadwater.h"
//#include "parseUtil.h"

#include "cmpriver.h"

cmpRiverSegment::cmpRiverSegment (cmpRiver *parent) :
    QObject(parent)
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

int cmpRiverSegment::getGasStepsPerDay() const
{
    return gasStepsPerDay;
}

void cmpRiverSegment::setGasStepsPerDay(int newGasStepsPerDay)
{
    if (gasStepsPerDay != newGasStepsPerDay)
    {
        gasStepsPerDay = newGasStepsPerDay;
        gasStepsPerSeason = gasStepsPerDay * daysPerSeason;
        allocateGasSteps(gasStepsPerDay);
    }
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

void cmpRiverSegment::setOutputSettings(int newOutputSettings)
{
    outputSettings = newOutputSettings;
}

int cmpRiverSegment::getStepsPerDay() const
{
    return stepsPerDay;
}

void cmpRiverSegment::setStepsPerDay(int newStepsPerDay)
{
    if (stepsPerDay != newStepsPerDay)
    {
        allocateSteps(newStepsPerDay);
    }
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
    if (daysPerSeason != newDaysPerSeason)
    {
        allocateDays(newDaysPerSeason);
    }
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
    outputFlags = 0;
    outputSettings = 0;
    flowMax = 0.0;
    flowMin = 0.0;
    temp.append(4);
    readFlows = false;
    flow.append(1);
    setDaysPerYear(366);
    setDaysPerSeason(366);
    setStepsPerDay(2);
    setGasStepsPerDay(2);
    isRegPoint = false;
    readTemps = false;
    up = nullptr;
    down = nullptr;
    fork = nullptr;
    temp_1 = -1;
    readGas = false;
    gasDistIn = nullptr;//new cmpGasDistribution();
    gasDistOut = nullptr;//new cmpGasDistribution();
    readTurbidity = false;
}

cmpRiverSegment::~cmpRiverSegment ()
{
    cmpRiverPoint *rpt;
    if (gasDistIn != nullptr)
        delete gasDistIn;
    if (gasDistOut != nullptr)
        delete gasDistOut;
    while (course.count() > 0)
    {
        rpt = course.takeLast();
        delete rpt;
    }
}

void cmpRiverSegment::resetData()
{
    outputFlags = 0;
    outputSettings = 0;
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

    if (token.compare ("output_settings", Qt::CaseInsensitive) == 0)
    {
       okay = cfile->readInt(outputSettings);
    }
    else if (token.compare ("output_flags", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readInt(outputFlags);
    }
    else if (token.compare("flow_max", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, flowMax);
    }
    else if (token.compare("flow_min", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, flowMin);
    }
    else if (token.compare("flow", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatArray(flow, daysPerSeason, Data::None, 1, "flow");
    }
    else if (token.compare ("water_temp", Qt::CaseInsensitive) == 0)
    {
        readTemps = true;
        okay = cfile->readFloatArray (temp, daysPerSeason, Data::None, stepsPerDay, "water_temp");
    }
    else if (token.compare ("gas_theta", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, gasTheta);
    }
    else if (token.compare ("output_gas", Qt::CaseInsensitive) == 0)
    {
        token = cfile->popToken ();
        if (token.compare("on", Qt::CaseInsensitive) == 0)
        {
            readGas = true;
            cfile->readFloatArray(gasInitial, gasStepsPerSeason, Data::None, stepsPerDay, "input gas");
        }
        else
        {
            readGas = false;
        }
    }
    else if (token.compare ("input_turbidity", Qt::CaseInsensitive) == 0)
    {
        token = cfile->popToken ();
        if (token.compare("on", Qt::CaseInsensitive) == 0)
        {
            readTurbidity = true;
            cfile->readFloatArray(turbidity, stepsPerSeason, Data::None, stepsPerDay, "turbidity");
        }
        else
        {
            readTurbidity = false;
        }
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
    writeTempData(outfile, indent+1, outputAll);
    writeGasData(outfile, indent+1, outputAll);
    writeTurbidData(outfile, indent+1, outputAll);
    outfile->writeEnd(indent, "segment", name);
}

void cmpRiverSegment::writeConfigData(cmpFile *outfile, int indent, bool outputAll)
{
    int dval = outputAll? 1000000: 0;
    outfile->writeValue(indent, "output_settings", outputSettings, dval);
}

void cmpRiverSegment::writeFlowData(cmpFile *outfile, int indent, bool outputAll)
{
    float fdef = (outputAll? 100000: 0);
    outfile->writeValue(indent, "flow_max", getFlowMax(), Data::Fixed, fdef);
    if (readFlows)
    {
        outfile->writeFloatArray(indent, "flow", "", flow, Data::None, stepsPerDay, Data::Fixed, fdef);
    }
}

void cmpRiverSegment::writeGasData (cmpFile *outfile, int indent, bool outputAll)
{
    float fdef = outputAll? 1000000: 0;
    if (readGas)
    {
        if (gasInitial.isEmpty())
        {
            gasDistIn->writeData(outfile, indent, outputAll);
        }
        else
        {
            outfile->writeFloatArray(indent, "output_gas", "On",  gasInitial, Data::None, stepsPerDay, Data::Fixed, fdef);
        }
    }
    else
    {
        outfile->writeString(indent, "output_gas", "Off");
    }
}

void cmpRiverSegment::writeTempData (cmpFile *outfile, int indent, bool outputAll)
{
    if (readTemps)
    {
        float fdef = (outputAll? 1000000: 0);
        outfile->writeFloatArray(indent, "water_temp", "", temp, Data::None, stepsPerDay, Data::Float, fdef);
    }
}

void cmpRiverSegment::writeTurbidData (cmpFile *outfile, int indent, bool outputAll)
{
    float fdef = (outputAll? 1000000: 0);
    if (readTurbidity)
    {
        outfile->writeFloatArray(indent, "input_turbidity", "On", turbidity, Data::DataConversion::None, 2, Data::Fixed, fdef);
    }
    else
    {
        outfile->writeString(indent, "input_turbidity", "Off");
    }
}

bool cmpRiverSegment::parseDesc(cmpFile *descfile)
{
    bool okay = true, end = false;
    QString token ("");
    QString na("");

    std::cout << "       Parsing Headwater description: " << name.toStdString() << std::endl;

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

bool cmpRiverSegment::allocateDays(int newDaysPerSeason)
{
    bool changed = false;
    daysPerSeason = newDaysPerSeason;
    if (!flow.isEmpty())
        flow.clear();
    if (!temp.isEmpty())
        temp.clear();
    for (int i = 0; i < daysPerSeason; i++)
    {
        flow.append(0.0);
        temp.append(0.0);
    }
    changed = true;
    return changed;
}

bool cmpRiverSegment::allocateSteps(int newStepsPerDay)
{
    bool changed = false;
    stepsPerDay = newStepsPerDay;
    stepsPerSeason = stepsPerDay * daysPerSeason;
    if (!turbidity.isEmpty())
        turbidity.clear();
    for (int i = 0; i < stepsPerSeason; i++)
    {
        turbidity.append(0.0);
    }
    changed = true;
    return changed;
}

bool cmpRiverSegment::allocateGasSteps(int newGasStepsPerDay)
{
    bool changed = false;
    gasStepsPerDay = newGasStepsPerDay;
    if (!gasInitial.isEmpty())
        gasInitial.clear();
    for (int i = 0; i < gasStepsPerSeason; i++)
    {
        gasInitial.append(0.0);
    }
    changed = true;
    return changed;
}

void cmpRiverSegment::allocate(int days, int steps, int gasSteps)
{
    setDaysPerSeason(days);
    setStepsPerDay(steps);
    setGasStepsPerDay(gasSteps);
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
