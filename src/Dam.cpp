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
    phouseSide =  Right;
//    width = 0.0;
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
    spillway = new Spillway();

    spillPlannedDay = nullptr;
    spillPlannedNight = nullptr;
    spillLegacyPlanned = nullptr;
    spillLegacyFish = nullptr;
    spillMax = 0.0;
    spillSide = Left;
    rswSpillMax = 0.0;
    rswActive = nullptr;

    flowMax = 0.0;
    flowProjectMin = 0.0;
    flowRiverMin = 0.0;

    allocate();

    transport = nullptr;
}

void Dam::allocate()
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

void Dam::outputDesc(CompassFile *outfile)
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
        if (getNumFishways() > 0) {
            outfile->writeString(1, "fishway");//, getFishway()->getTypeString());
            for (int i = 0; i < getNumFishways(); i++) {
                outfile->writeString(2, "type", QString::number(i), getFishway(i)->getTypeString());
                outfile->writeNumberedValue(2, "length", i, getFishway(i)->getLength());
                outfile->writeNumberedValue(2, "capacity", i, getFishway(i)->getCapacity());
                outfile->writeNumberedValue(2, "velocity", i, getFishway(i)->getVelocity());
            }
            outfile->writeString(1, "end", "fishway");
        }
        outfile->writeString(1, "latlon", getCourse().at(0)->getLatLon());

        outfile->writeString(0, "end", QString("(%1)").arg(*name));
    }
}

Spillway *Dam::getSpillway() const
{
    return spillway;
}

void Dam::setSpillway(Spillway *value)
{
    spillway = value;
}

Basin *Dam::getBasin() const
{
    return basin;
}

void Dam::setBasin(Basin *value)
{
    basin = value;
}

dam_species *Dam::getSpecies() const
{
    return species;
}

QList<PowerHouse *> Dam::getPowerhouses() const
{
    return powerhouses;
}

void Dam::setPowerhouses(const QList<PowerHouse *> &value)
{
    powerhouses = value;
}

int Dam::setPowerhouse(PowerHouse *house, int num)
{
    if (house != nullptr)
    {
        num = getNumPowerhouses();
        house->setNumber(num);
        powerhouses.append(house);
    }
    return num;
}

int Dam::getNumPowerhouses()
{
    int num = 0;
    if (!powerhouses.isEmpty())
        num = powerhouses.count();
    return num;
}

Dam::Location Dam::getPhouseSide() const
{
    return phouseSide;
}

void Dam::setPhouseSide(const Location &value)
{
    phouseSide = value;
}

float Dam::getWidthTailrace() const
{
    return widthTailrace;
}

void Dam::setWidthTailrace(float value)
{
    widthTailrace = value;
}

float Dam::getLengthTailrace() const
{
    return lengthTailrace;
}

void Dam::setLengthTailrace(float value)
{
    lengthTailrace = value;
}

float Dam::getElevBase() const
{
    return elevBase;
}

void Dam::setElevBase(float value)
{
    elevBase = value;
}

float Dam::getElevForebay() const
{
    return elevForebay;
}

void Dam::setElevForebay(float value)
{
    elevForebay = value;
}

float Dam::getElevTailrace() const
{
    return elevTailrace;
}

void Dam::setElevTailrace(float value)
{
    elevTailrace = value;
}

float Dam::getFullHead() const
{
    return fullHead;
}

void Dam::setFullHead(float value)
{
    fullHead = value;
}

float Dam::getDepthForebay() const
{
    return depthForebay;
}

void Dam::setDepthForebay(float value)
{
    depthForebay = value;
}

float Dam::getDepthTailrace() const
{
    return depthTailrace;
}

void Dam::setDepthTailrace(float value)
{
    depthTailrace = value;
}

float Dam::getHeightBypass() const
{
    return heightBypass;
}

void Dam::setHeightBypass(float value)
{
    heightBypass = value;
}

int Dam::getCollector() const
{
    return collector;
}

void Dam::setCollector(int value)
{
    collector = value;
}

float Dam::getLengthBasin() const
{
    return lengthBasin;
}

void Dam::setLengthBasin(float value)
{
    lengthBasin = value;
}

float Dam::getSpecGrav() const
{
    return specGrav;
}

void Dam::setSpecGrav(float value)
{
    specGrav = value;
}

QList<float> Dam::getSpill() const
{
    return spill;
}

void Dam::setSpill(const QList<float> &value)
{
    spill = value;
}

QList<float> Dam::getSpillPlanned() const
{
    return spillPlanned;
}

void Dam::setSpillPlanned(const QList<float> &value)
{
    spillPlanned = value;
}

FloatPeriodList *Dam::getSpillPlannedDay() const
{
    return spillPlannedDay;
}

void Dam::setSpillPlannedDay(FloatPeriodList *value)
{
    spillPlannedDay = value;
}

FloatPeriodList *Dam::getSpillPlannedNight() const
{
    return spillPlannedNight;
}

void Dam::setSpillPlannedNight(FloatPeriodList *value)
{
    spillPlannedNight = value;
}

FloatPeriodList *Dam::getSpillLegacyPlanned() const
{
    return spillLegacyPlanned;
}

void Dam::setSpillLegacyPlanned(FloatPeriodList *value)
{
    spillLegacyPlanned = value;
}

FloatPeriodList *Dam::getSpillLegacyFish() const
{
    return spillLegacyFish;
}

void Dam::setSpillLegacyFish(FloatPeriodList *value)
{
    spillLegacyFish = value;
}

float Dam::getSpillMax() const
{
    return spillMax;
}

void Dam::setSpillMax(float value)
{
    spillMax = value;
}

QString &Dam::getSpillSideText()
{
    return spillSideText;
}

void Dam::setSpillSideText(const QString &text)
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

Dam::Location Dam::getSpillSide() const
{
    return spillSide;
}

void Dam::setSpillSide(const Location &value)
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

float Dam::getRswSpillMax() const
{
    return rswSpillMax;
}

void Dam::setRswSpillMax(float value)
{
    rswSpillMax = value;
}

BoolPeriodList *Dam::getRswActive() const
{
    return rswActive;
}

void Dam::setRswActive(BoolPeriodList *value)
{
    rswActive = value;
}

float Dam::getFlowProjectMin() const
{
    return flowProjectMin;
}

void Dam::setFlowProjectMin(float value)
{
    flowProjectMin = value;
}

float Dam::getFlowRiverMin() const
{
    return flowRiverMin;
}

void Dam::setFlowRiverMin(float value)
{
    flowRiverMin = value;
}

Transport *Dam::getTransport() const
{
    return transport;
}

void Dam::setTransport(Transport *value)
{
    transport = value;
}

QList<float> Dam::getDepthForebayDay() const
{
    return depthForebayDay;
}

void Dam::setDepthForebayDay(const QList<float> &value)
{
    depthForebayDay = value;
}

QList<float> Dam::getDepthTailraceDay() const
{
    return depthTailraceDay;
}

void Dam::setDepthTailraceDay(const QList<float> &value)
{
    depthTailraceDay = value;
}

QList<float> Dam::getDropRatioDay() const
{
    return dropRatioDay;
}

void Dam::setDropRatioDay(const QList<float> &value)
{
    dropRatioDay = value;
}

QList<float> Dam::getDropRatioDayTR() const
{
    return dropRatioDayTR;
}

void Dam::setDropRatioDayTR(const QList<float> &value)
{
    dropRatioDayTR = value;
}

QList<float> Dam::getDaylightProportion() const
{
    return daylightProportion;
}

void Dam::setDaylightProportion(const QList<float> &value)
{
    daylightProportion = value;
}

int Dam::getNumFishways()
{
    int num = 0;
    if (!fishways.isEmpty())
        num = fishways.count();
    return num;
}

void Dam::setNumFishways(int num)
{
    Fishway *fsh;
    while (fishways.count() < num)
    {
        fsh = new Fishway();
        fishways.append(fsh);
    }

    while (fishways.count() > num)
    {
        fsh = fishways.takeLast();
        delete fsh;
    }
}

Fishway *Dam::getFishway(int index) const
{
    return fishways[index];
}

void Dam::setFishway(int index, Fishway *value)
{
    fishways[index] = value;
}
