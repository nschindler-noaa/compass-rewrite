#include "cmpdam.h"

cmpDam::cmpDam(QObject *parent) : cmpRiverSegment(parent)
{

}

cmpDam::cmpDam(QString dname, QString rivName, QObject *parent) :
    cmpRiverSegment (rivName, parent)
{
    name = new QString (dname);
    type = cmpRiverSegment::Dam;
    clear ();
}

cmpDam::~cmpDam ()
{
}

void cmpDam::clear()
{
    basin = nullptr;
    species = nullptr;
    phouseSide =  Right;
    lengthTailrace = 0.0;
    elevBase = 0.0;
    elevForebay = 0.0;
    elevTailrace = 0.0;
    fullHead = 0.0;
    depthForebay = 0.0;
    heightBypass = 0.0;
    collector = 0;

    lengthBasin = 0.0;
    specGrav = 0.0;
    spillway = new cmpSpillway();

    spillPlannedDay = nullptr;
    spillPlannedNight = nullptr;
    spillLegacyPlanned = nullptr;
    spillLegacyFish = nullptr;
    spillMax = 0.0;
    spillSide = Left;

    flowMax = 0.0;
    flowProjectMin = 0.0;
    flowRiverMin = 0.0;

    allocateDays();

    transport = nullptr;
}

void cmpDam::allocateDays()
{
    while (depthForebayDay.count() < DAYS_IN_SEASON)
    {
        depthForebayDay.append(0);
        depthTailraceDay.append(0);
        dropRatioDay.append(0);
        dropRatioDayTR.append(0);
    }
    while (depthForebayDay.count() > DAYS_IN_SEASON)
    {
        depthForebayDay.takeLast();
        depthTailraceDay.takeLast();
        dropRatioDay.takeLast();
        dropRatioDayTR.takeLast();
    }

    while (daylightProportion.count() < DAM_SLICES_IN_SEASON)
    {
        spill.append(0);
        spillPlanned.append(0);
        daylightProportion.append(0);
    }
    while (daylightProportion.count() > DAM_SLICES_IN_SEASON)
    {
        spill.takeLast();
        spillPlanned.takeLast();
        daylightProportion.takeLast();
    }

    for (int i = 0; i < DAYS_IN_SEASON; i++)
    {
        depthForebayDay[i] = 0.0;
        depthTailraceDay[i] = 0.0;
        dropRatioDay[i] = 0.0;
        dropRatioDayTR[i] = 0.0;
    }
    for (int i = 0; i < DAM_SLICES_IN_SEASON; i++)
    {
        spill[i] = 0.0;
        spillPlanned[i] = 0.0;
        daylightProportion[i] = 0;
    }
}

void cmpDam::calculateFlow()
{
    calculateFlowInputs();
    calculateFlows();
}

void cmpDam::calculateFlows()
{

}

void cmpDam::calculateTemp()
{
    if (!readTemps)
    {
        calculateTempInputs();
        calculateTemps();
    }
}

void cmpDam::calculateTemps()
{

}

void cmpDam::calculateSpill()
{

}

void cmpDam::calculateFish()
{

}

void cmpDam::calculateStats()
{

}

void cmpDam::deleteSpill()
{

}

bool cmpDam::parse (cmpFile *cfile)
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

bool cmpDam::parseToken (QString token, cmpFile *cfile)
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
        okay = cmpRiverSegment::parseToken (token, cfile);
    }

    return okay;
}

void cmpDam::outputDesc(cmpFile *outfile)
{
    if (!outfile->isOpen())
        outfile->open(QIODevice::WriteOnly);

    if (outfile->isOpen())
    {
        int num = getNumPowerhouses();
        outfile->writeString(0, "dam", *name);
        outfile->writeString(1, "abbrev", *getAbbrev());
        for (int i = 0; i < num; i++)
        {
            if (i == 0)
                outfile->writeStringNR(1, "powerhouse_capacity ");

            else
                outfile->writeStringNR(1, QString("powerhouse_%1_capacity ").arg(QString::number(i+1)));

            outfile->writeFloatOrNa(powerhouses.at(i)->getCapacity());
        }

        outfile->writeStringNR(1, "floor_elevation ");
        outfile->writeFloatOrNa(getElevBase());
        outfile->writeStringNR(1, "forebay_elevation ");
        outfile->writeFloatOrNa(getElevForebay());
        outfile->writeStringNR(1, "tailrace_elevation ");
        outfile->writeFloatOrNa(getElevTailrace());
        outfile->writeStringNR(1, "bypass_elevation ");
        outfile->writeFloatOrNa(getHeightBypass());
        outfile->writeStringNR(1, "spillway_width ");
        outfile->writeFloatOrNa(getSpillway()->getWidth());
        outfile->writeStringNR(1, "spill_side ");
        outfile->writeFloatOrNa(getSpillSide());
        outfile->writeStringNR(1, "pergate ");
        outfile->writeFloatOrNa(getSpillway()->getPerGate());
        outfile->writeStringNR(1, "numgates ");
        outfile->writeFloatOrNa(getSpillway()->getNumGates());
        outfile->writeStringNR(1, "gate_width ");
        outfile->writeFloatOrNa(getSpillway()->getGateWidth());
        outfile->writeStringNR(1, "basin_length ");
        outfile->writeFloatOrNa(getLengthBasin());
        outfile->writeStringNR(1, "sgr ");
        outfile->writeFloatOrNa(specGrav);
        outfile->writeStringNR(1, "numgates ");
        outfile->writeFloatOrNa(getSpillway()->getNumGates());
        outfile->writeStringNR(1, "numgates ");
        outfile->writeFloatOrNa(getSpillway()->getNumGates());
        if (getFishway() != nullptr) {
            outfile->writeString(1, "fishway");//, getFishway()->getTypeString());
            outfile->writeString(2, "type", getFishway()->getTypeString());
            outfile->writeValue(2, "length", getFishway()->getLength());
            outfile->writeValue(2, "capacity", getFishway()->getCapacity());
            outfile->writeValue(2, "velocity", getFishway()->getVelocity());
            outfile->writeString(1, "end", "fishway");
        }
        outfile->writeString(1, "latlon", getCourse().at(0)->getLatLon());

        outfile->writeString(0, "end", QString("(%1)").arg(*name));
    }
}

cmpSpillway *cmpDam::getSpillway() const
{
    return spillway;
}

void cmpDam::setSpillway(cmpSpillway *value)
{
    spillway = value;
}

cmpBasin *cmpDam::getBasin() const
{
    return basin;
}

void cmpDam::setBasin(cmpBasin *value)
{
    basin = value;
}

cmpDamSpecies *cmpDam::getSpecies() const
{
    return species;
}

void cmpDam::setSpecies(cmpDamSpecies *spec)
{
    species = spec;
}

cmpPowerhouse *cmpDam::getPowerhouse(int index)
{
    cmpPowerhouse *phouse = nullptr;
    if (index >= 0 && index < powerhouses.count())
        phouse = powerhouses.at(index);
    return phouse;
}

const QList<cmpPowerhouse *> &cmpDam::getPowerhouses() const
{
    return powerhouses;
}

void cmpDam::setPowerhouses(const QList<cmpPowerhouse *> &value)
{
    powerhouses = value;
}

int cmpDam::setPowerhouse(cmpPowerhouse *house, int num)
{
    if (house != nullptr)
    {
        num = getNumPowerhouses();
        house->setNumber(num);
        powerhouses.append(house);
    }
    return num;
}

int cmpDam::getNumPowerhouses()
{
    int num = 0;
    if (!powerhouses.isEmpty())
        num = powerhouses.count();
    return num;
}

cmpDam::Location cmpDam::getPhouseSide() const
{
    return phouseSide;
}

void cmpDam::setPhouseSide(const Location &value)
{
    phouseSide = value;
}

float cmpDam::getWidthTailrace() const
{
    return widthTailrace;
}

void cmpDam::setWidthTailrace(float value)
{
    widthTailrace = value;
}

float cmpDam::getLengthTailrace() const
{
    return lengthTailrace;
}

void cmpDam::setLengthTailrace(float value)
{
    lengthTailrace = value;
}

float cmpDam::getElevBase() const
{
    return elevBase;
}

void cmpDam::setElevBase(float value)
{
    elevBase = value;
}

float cmpDam::getElevForebay() const
{
    return elevForebay;
}

void cmpDam::setElevForebay(float value)
{
    elevForebay = value;
}

float cmpDam::getElevTailrace() const
{
    return elevTailrace;
}

void cmpDam::setElevTailrace(float value)
{
    elevTailrace = value;
}

float cmpDam::getFullHead() const
{
    return fullHead;
}

void cmpDam::setFullHead(float value)
{
    fullHead = value;
}

float cmpDam::getDepthForebay() const
{
    return depthForebay;
}

void cmpDam::setDepthForebay(float value)
{
    depthForebay = value;
}

float cmpDam::getDepthTailrace() const
{
    return depthTailrace;
}

void cmpDam::setDepthTailrace(float value)
{
    depthTailrace = value;
}

float cmpDam::getHeightBypass() const
{
    return heightBypass;
}

void cmpDam::setHeightBypass(float value)
{
    heightBypass = value;
}

int cmpDam::getCollector() const
{
    return collector;
}

void cmpDam::setCollector(int value)
{
    collector = value;
}

float cmpDam::getLengthBasin() const
{
    return lengthBasin;
}

void cmpDam::setLengthBasin(float value)
{
    lengthBasin = value;
}

float cmpDam::getSpecGrav() const
{
    return specGrav;
}

void cmpDam::setSpecGrav(float value)
{
    specGrav = value;
}

QList<float> cmpDam::getSpill() const
{
    return spill;
}

void cmpDam::setSpill(const QList<float> &value)
{
    spill = value;
}

QList<float> cmpDam::getSpillPlanned() const
{
    return spillPlanned;
}

void cmpDam::setSpillPlanned(const QList<float> &value)
{
    spillPlanned = value;
}

FloatPeriodList *cmpDam::getSpillPlannedDay() const
{
    return spillPlannedDay;
}

void cmpDam::setSpillPlannedDay(FloatPeriodList *value)
{
    spillPlannedDay = value;
}

FloatPeriodList *cmpDam::getSpillPlannedNight() const
{
    return spillPlannedNight;
}

void cmpDam::setSpillPlannedNight(FloatPeriodList *value)
{
    spillPlannedNight = value;
}

FloatPeriodList *cmpDam::getSpillLegacyPlanned() const
{
    return spillLegacyPlanned;
}

void cmpDam::setSpillLegacyPlanned(FloatPeriodList *value)
{
    spillLegacyPlanned = value;
}

FloatPeriodList *cmpDam::getSpillLegacyFish() const
{
    return spillLegacyFish;
}

void cmpDam::setSpillLegacyFish(FloatPeriodList *value)
{
    spillLegacyFish = value;
}

float cmpDam::getSpillMax() const
{
    return spillMax;
}

void cmpDam::setSpillMax(float value)
{
    spillMax = value;
}

QString &cmpDam::getSpillSideText()
{
    return spillSideText;
}

void cmpDam::setSpillSideText(const QString &text)
{
    spillSideText = QString(text);
    if (text.contains("right", Qt::CaseInsensitive))
        spillSide = Right;
    else if (text.contains("left", Qt::CaseInsensitive))
        spillSide = Left;
    else if (text.contains("middle", Qt::CaseInsensitive))
        spillSide = Middle;
    else
        spillSide = None;
}

cmpDam::Location cmpDam::getSpillSide() const
{
    return spillSide;
}

void cmpDam::setSpillSide(const Location &value)
{
    spillSide = value;
    switch (spillSide) {
    case Right:
        spillSideText = QString("right");
        break;
    case Left:
        spillSideText = QString("left");
        break;
    case Middle:
        spillSideText = QString("middle");
        break;
    case None:
        spillSideText = QString("none");
    }
}

float cmpDam::getRswSpillMax() const
{
    return spillWeir->getMaxSpill();
}

void cmpDam::setRswSpillMax(float value)
{
    spillWeir->setMaxSpill(value);
}

cmpRSW *cmpDam::getRswActive() const
{
    return spillWeir;
}

void cmpDam::setRswActive(cmpRSW *value)
{
    spillWeir = value;
}

float cmpDam::getFlowProjectMin() const
{
    return flowProjectMin;
}

void cmpDam::setFlowProjectMin(float value)
{
    flowProjectMin = value;
}

float cmpDam::getFlowRiverMin() const
{
    return flowRiverMin;
}

void cmpDam::setFlowRiverMin(float value)
{
    flowRiverMin = value;
}

cmpTransport *cmpDam::getTransport() const
{
    return transport;
}

void cmpDam::setTransport(cmpTransport *value)
{
    transport = value;
}

QList<float> cmpDam::getDepthForebayDay() const
{
    return depthForebayDay;
}

void cmpDam::setDepthForebayDay(const QList<float> &value)
{
    depthForebayDay = value;
}

QList<float> cmpDam::getDepthTailraceDay() const
{
    return depthTailraceDay;
}

void cmpDam::setDepthTailraceDay(const QList<float> &value)
{
    depthTailraceDay = value;
}

QList<float> cmpDam::getDropRatioDay() const
{
    return dropRatioDay;
}

void cmpDam::setDropRatioDay(const QList<float> &value)
{
    dropRatioDay = value;
}

QList<float> cmpDam::getDropRatioDayTR() const
{
    return dropRatioDayTR;
}

void cmpDam::setDropRatioDayTR(const QList<float> &value)
{
    dropRatioDayTR = value;
}

QList<float> cmpDam::getDaylightProportion() const
{
    return daylightProportion;
}

void cmpDam::setDaylightProportion(const QList<float> &value)
{
    daylightProportion = value;
}

cmpFishway *cmpDam::getFishway() const
{
    return fishway;
}

void cmpDam::setFishway(cmpFishway *value)
{
    fishway = value;
}
