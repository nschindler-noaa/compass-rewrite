#include "cmpdam.h"

#include "cmpriver.h"

cmpDam::cmpDam(cmpRiver *parent) : cmpRiverSegment(parent)
{
    name = QString ();
    type = cmpRiverSegment::Dam;
    typeStr = "dam";
    setup ();
}

cmpDam::cmpDam(QString dname, cmpRiver *parent) :
    cmpRiverSegment (parent)
{
    name = QString (dname);
    type = cmpRiverSegment::Dam;
    typeStr = "dam";
    setup ();
}

cmpDam::~cmpDam ()
{
    clear();
}

void cmpDam::setup()
{
    basin = nullptr;
    if (!species.isEmpty())
        species.clear();
    phouseSide =  Right;
    lengthTailrace = 0.0;
    elevBase = 0.0;
    elevForebay = 0.0;
    elevTailrace = 0.0;
    fullHead = 0.0;
    depthForebay = 0.0;
    elevBypass = 0.0;
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

    nsatEqn = nullptr;
    nsatNightEqn = nullptr;
    nsatBackupEqn = nullptr;

    flowMax = 0.0;
    flowProjectMin = 0.0;
    flowRiverMin = 0.0;

    allocateDays(366, 2);

    spillWeir = nullptr;
    transport = nullptr;
    fishway = nullptr;
}

void cmpDam::resetData()
{
    allocateDays(366, 2);
}

void cmpDam::clear()
{
    if (basin != nullptr)
        delete basin;
    basin = nullptr;
    while (!species.isEmpty())
        delete species.takeLast();

    delete spillway;
    spillway = nullptr;

    if (spillPlannedDay != nullptr)
        delete spillPlannedDay;
    spillPlannedDay = nullptr;
    if (spillPlannedNight != nullptr)
        delete spillPlannedNight;
    spillPlannedNight = nullptr;
    if (spillLegacyPlanned != nullptr)
        delete spillLegacyPlanned;
    spillLegacyPlanned = nullptr;
    if (spillLegacyFish != nullptr)
        delete spillLegacyFish;
    spillLegacyFish = nullptr;

    if (transport != nullptr)
        delete transport;
    transport = nullptr;
    if (fishway != nullptr)
        delete fishway;
    fishway = nullptr;
}

void cmpDam::allocateDays(int days, int slices)
{
    int dayslices = days * slices;
    if (!depthForebayDay.isEmpty())
        depthForebayDay.clear();
    if (!depthTailraceDay.isEmpty())
        depthTailraceDay.clear();
    if (!dropRatioDay.isEmpty())
        dropRatioDay.clear();
    if (!dropRatioDayTR.isEmpty())
        dropRatioDayTR.clear();
    for (int i = 0; i < days; i++)
    {
        depthForebayDay.append(0);
        depthTailraceDay.append(0);
        dropRatioDay.append(0);
        dropRatioDayTR.append(0);
    }

    if (!spill.isEmpty())
        spill.clear();
    if (!spillPlanned.isEmpty())
        spill.clear();
    if (!daylightProportion.isEmpty())
        daylightProportion.clear();
    for (int i = 0; i < dayslices; i++)
    {
        spill.append(0);
        spillPlanned.append(0);
        daylightProportion.append(0);
    }
    cmpRiverSegment::allocateDays(days);
}

void cmpDam::setSpeciesNames(QStringList &spNames)
{
    int total = powerhouses.count();
    for (int i = 0; i < total; i++)
    {
        powerhouses.at(i)->setSpeciesNames(spNames);
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

bool cmpDam::parseData (cmpFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");
    currentPHouse = nullptr;
    if (!powerhouses.isEmpty())
        currentPHouse = powerhouses[0];

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
            okay = cfile->checkEnd("dam", name);
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
    int tmpInt;
    float tmpFloat;
    QString na (""), tmpstr;

    if (token.compare ("tailrace_length", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, lengthTailrace);
    }
    else if (token.compare("nsat_day_equation", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readString(tmpstr);
        if (nsatEqn != nullptr)
            nsatEqn->setName(tmpstr);
        else
            nsatEqn = new cmpEquation(tmpstr);
        nsatEqn->parseData(cfile, "nsat_day_equation");
    }
    else if (token.compare("nsat_backup_equation", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readString(tmpstr);
        if (nsatBackupEqn != nullptr)
            nsatBackupEqn->setName(tmpstr);
        else
            nsatBackupEqn = new cmpEquation(tmpstr);
        nsatBackupEqn->parseData(cfile, "nsat_backup_equation");
    }
    else if (token.compare("nsat_night_equation", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readString(tmpstr);
        if (nsatNightEqn != nullptr)
            nsatNightEqn->setName(tmpstr);
        else
            nsatNightEqn = new cmpEquation(tmpstr);
        nsatNightEqn->parseData(cfile, "nsat_night_equation");
    }
    else if (token.compare("k_entrain", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, entrainK);
    }
    else if (token.compare("entrain_factor", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, entrainFactor);
    }
    else if (token.compare("powerhouse_priority", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, tmpFloat);
        currentPHouse->setPriority(tmpFloat);
    }
    else if (token.compare("powerhouse_capacity", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, tmpFloat);
        currentPHouse->setCapacity(tmpFloat);
    }
    else if (token.compare("flow_min", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, tmpFloat);
        currentPHouse->setThreshold(tmpFloat);
    }
    else if (token.compare("rsw_spill_cap", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        currentPHouse->setSpeciesRswCap(tmpstr, tmpFloat);
    }
    else if (token.compare("powerhouse_schedule", Qt::CaseInsensitive) == 0)
    {
//        okay = cfile->readFloatArray(na, currentPHouse->getSchedule());
        cfile->skipAllNumbers();
    }

//    outfile->writeStringNR(indent, "powerhouse_schedule ");
    else
    {
        okay = cmpRiverSegment::parseToken (token, cfile);
    }

    return okay;
}

void cmpDam::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    float dval = 0;
    cmpEquation *eqn = nullptr;
    int total = 0;
    int indent2 = indent + 1;

    outfile->writeString(indent, "dam", name);

    if (outputAll)
    {
        writeAllData(outfile, indent2);
    }
    else
    {
        outfile->writeValue(1, "output_settings", outputSettings, 0);
        outfile->writeValue(1, "tailrace_length", lengthTailrace, dval);
        outfile->writeValue(1, "spill_cap", spillMax);
        outfile->writeFloatArray(1, "actual_spill", QString(), spill, Data::None, stepsPerDay, Data::Float, dval);
        outfile->writeValue(1, "gas_theta", gasTheta, dval);
        outfile->writeValue(1, "k_entrain", entrainK, dval);
        outfile->writeValue(1, "entrain_factor", entrainFactor, dval);
        if (readGas)
        {
            outfile->writeString(1, "output_gas", "On");
            // output the gas distribution
        }
        else
        {
            outfile->writeString(1, "output_gas", "Off");
        }
        if (readTurbidity)
        {
            outfile->writeString(1, "input_turbidity", "On");
            // output the input turbidity
        }
        else
        {
            outfile->writeString(1, "input_turbidity", "Off");
        }
        if (basin != nullptr)
        {
            outfile->writeFloatArray(1, "storage_volume", QString(), basin->getVolume(), Data::None, stepsPerDay, Data::Float, dval);
        }
        outfile->writeNewline();
        eqn = getNsatEqn();
        if (eqn != nullptr && eqn != (new cmpEquation(eqn->getId())))
        {
            outfile->writeValue(1, "nsat_day_equation", eqn->getId());
            eqn->writeParameters(outfile, 2, false);
            outfile->writeEnd(1, "nsat_day_equation");
        }
        eqn = getNsatNightEqn();
        if (eqn != nullptr && eqn != (new cmpEquation(eqn->getId())))
        {
            outfile->writeValue(1, "nsat_night_equation", eqn->getId());
            eqn->writeParameters(outfile, 2, false);
            outfile->writeEnd(1, "nsat_night_equation");
        }
        eqn = getNsatBackupEqn();
        if (eqn != nullptr && eqn != (new cmpEquation(eqn->getId())))
        {
            outfile->writeValue(1, "nsat_backup_equation", eqn->getId());
            eqn->writeParameters(outfile, 2, false);
            outfile->writeEnd(1, "nsat_backup_equation");
        }

        powerhouses.at(0)->writeData(outfile, 1, outputAll);
        total = powerhouses.count();
        for (int i = 1; i < total; i++)
        {
            QString name2(QString("%1_%2").arg(name).arg(i+1));
            outfile->writeString(1, "additional_powerhouse", name2);// Bonneville_Dam_2
            powerhouses.at(i)->writeSecondData(outfile, 1, outputAll);
            outfile->writeEnd(1, "additional_powerhouse", name2);
        }
    }
}

void cmpDam::writeAllData(cmpFile *outfile, int indent)
{
    int total = 0;
    cmpEquation *eqn;
    outfile->writeValue(1, "output_settings", outputSettings);
    outfile->writeValue(1, "tailrace_length", lengthTailrace);
    outfile->writeValue(1, "spill_cap", spillMax);
    outfile->writeFloatArray(1, "actual_spill", QString(), spill, Data::None, stepsPerDay, Data::Float, 100000.0);
    outfile->writeValue(1, "gas_theta", gasTheta);
    outfile->writeValue(1, "k_entrain", entrainK);
    outfile->writeValue(1, "entrain_factor", entrainFactor);
    if (readGas)
    {
        outfile->writeString(1, "output_gas", "On");
        // output the gas distribution
    }
    else
    {
        outfile->writeString(1, "output_gas", "Off");
    }
    if (readTurbidity)
    {
        outfile->writeString(1, "input_turbidity", "On");
        // output the input turbidity
    }
    else
    {
        outfile->writeString(1, "input_turbidity", "Off");
    }
    if (basin != nullptr)
    {
        outfile->writeFloatArray(1, "storage_volume", QString(), basin->getVolume(), Data::None, stepsPerDay, Data::Float, 100000.0);
    }
    outfile->writeNewline();
    eqn = getNsatEqn();
    if (eqn != nullptr)
    {
        outfile->writeValue(1, "nsat_day_equation", eqn->getId());
        eqn->writeParameters(outfile, 2, true);
        outfile->writeEnd(1, "nsat_day_equation");
        outfile->writeNewline();
    }
    eqn = getNsatNightEqn();
    if (eqn != nullptr)
    {
        outfile->writeValue(1, "nsat_night_equation", eqn->getId());
        eqn->writeParameters(outfile, 2, true);
        outfile->writeEnd(1, "nsat_night_equation");
        outfile->writeNewline();
    }
    eqn = getNsatBackupEqn();
    if (eqn != nullptr)
    {
        outfile->writeValue(1, "nsat_backup_equation", eqn->getId());
        eqn->writeParameters(outfile, 2, true);
        outfile->writeEnd(1, "nsat_backup_equation");
        outfile->writeNewline();
    }

    powerhouses.at(0)->writeData(outfile, 1, true);
    total = powerhouses.count();
    for (int i = 1; i < total; i++)
    {
        QString name2(QString("%1_%2").arg(name).arg(i+1));
        outfile->writeString(1, "additional_powerhouse", name2);// Bonneville_Dam_2
        powerhouses.at(i)->writeSecondData(outfile, 2, true);
        outfile->writeEnd(1, "additional_powerhouse", name2);
    }
}

bool cmpDam::parseDesc (cmpFile *descfile)
{
    bool okay = true, end = false;
    int tempInt = 0;
    float tempFloat = 0.0;
    QString token;
    QString na;

    while (okay && !end)
    {
        token = descfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            descfile->printEOF("Dam description");
            okay = false;
        }
        else if (token.compare("floor_elevation", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, elevBase);
        }
        else if (token.compare("forebay_elevation", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, elevForebay);
        }
        else if (token.compare("tailrace_elevation", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, elevTailrace);
        }
        else if (token.compare("bypass_elevation", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, elevBypass);
        }
        else if (token.compare("spill_side", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readString(token);
            setSpillSideText(token);
        }
        else if (token.compare("spillway_width", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, tempFloat);
            spillway->setWidth(tempFloat);
        }
        else if (token.compare("ngates", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readInt(tempInt);
            spillway->setNumGates(tempInt);
        }
                else if (token.compare("gate_width", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, tempFloat);
            spillway->setGateWidth(tempFloat);
        }
        else if (token.compare("pergate", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, tempFloat);
            spillway->setPerGate(tempFloat);
        }
        else if (token.compare("basin_length", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, lengthBasin);
        }
        else if (token.compare("sgr", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, specGrav);
        }
        else if (token.compare("powerhouse_capacity", Qt::CaseInsensitive) == 0)
        {
            if (powerhouses.isEmpty())
                powerhouses.append(new cmpPowerhouse());
            okay = descfile->readFloatOrNa(na, tempFloat);
            if (okay && powerhouses.count() > 0)
                powerhouses.at(0)->setCapacity(tempFloat);
        }
        else if (token.compare("powerhouse_2_capacity", Qt::CaseInsensitive) == 0)
        {
            while (powerhouses.count() < 2)
                powerhouses.append(new cmpPowerhouse());
            okay = descfile->readFloatOrNa(na, tempFloat);
            if (okay && powerhouses.count() > 1)
                powerhouses.at(1)->setCapacity(tempFloat);
        }
        else if (token.compare("storage_basin", Qt::CaseInsensitive) == 0)
        {
            basin = new cmpBasin();
            descfile->readString(token);
            okay = basin->parseDesc(token);
        }
        else if (token.compare("fishway", Qt::CaseInsensitive) == 0)
        {
            fishway = new cmpFishway();
            fishway->parseDesc(descfile);
        }

        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            descfile->checkEnd("dam", name);
            end = true;
        }
        else
        {
            cmpRiverSegment::parseDescToken(token, descfile);
        }
    }
    return okay;
}

void cmpDam::outputDesc(cmpFile *outfile)
{
    float tmpfloat = 0;
    int tmpint = 0;
    int indent1 = 1, indent2 = 2;
    if (!outfile->isOpen())
        outfile->open(QIODevice::WriteOnly);

    if (outfile->isOpen())
    {
        QString namestr = name;
        int num = getNumPowerhouses();
        outfile->writeString(indent1, "dam", namestr.replace('_', ' '));
        outfile->writeString(indent2, "abbrev", getAbbrev());
        outfile->writeString(indent2+1, QString("latlon"), getCourse().at(0)->getLatLon());
        if (basin != nullptr)
            outfile->writeString(indent2, "storage_basin", basin->getText());
        outfile->writeValue(indent2, "floor_elevation", getElevBase());
        outfile->writeValue(indent2, "forebay_elevation", getElevForebay());
        outfile->writeValue(indent2, "tailrace_elevation", getElevTailrace());
        tmpfloat = getHeightBypass();
        if (tmpfloat > 0.1)
            outfile->writeValue(indent2, "bypass_elevation", tmpfloat);
        outfile->writeValue(indent2, "spillway_width", getSpillway()->getWidth());
        outfile->writeString(indent2, "spill_side", getSpillSideText());
        tmpint = getSpillway()->getNumGates();
        if (tmpint > 0)
        {
            outfile->writeValue(indent2, "ngates", tmpint);
            outfile->writeValue(indent2, "gate_width", getSpillway()->getGateWidth());
            outfile->writeValue(indent2, "pergate", getSpillway()->getPerGate());
        }
        outfile->writeValue(indent2, "basin_length", getLengthBasin());
        outfile->writeValue(indent2, "sgr", specGrav);
        for (int i = 0; i < num; i++)
        {
            if (i == 0)
                outfile->writeValue(indent2, "powerhouse_capacity", powerhouses.at(i)->getCapacity());

            else
                outfile->writeValue(indent2, QString("powerhouse_%1_capacity").arg(QString::number(i+1)),
                                    powerhouses.at(i)->getCapacity());
        }

        if (getFishway() != nullptr) {
            outfile->writeString(indent2, "fishway");
            outfile->writeString(3, "type", getFishway()->getTypeString());
            outfile->writeValue(3, "length", getFishway()->getLength());
            outfile->writeValue(3, "capacity", getFishway()->getCapacity());
            outfile->writeValue(3, "velocity", getFishway()->getVelocity());
            outfile->writeString(indent2, "end", "fishway");
        }
        outfile->writeEnd(indent1, QString("dam"), namestr);
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

cmpDamSpecies *cmpDam::getSpecies(int index) const
{
    return species.at(index);
}

void cmpDam::setSpecies(int index, cmpDamSpecies *spec)
{
    while (species.count() < index)
        species.append(new cmpDamSpecies());
    species[index] = spec;
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
    return elevBypass;
}

void cmpDam::setHeightBypass(float value)
{
    elevBypass = value;
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

cmpEquation *cmpDam::getNsatEqn() const
{
    return nsatEqn;
}

void cmpDam::setNsatEqn(cmpEquation *newNsatEqn)
{
    nsatEqn = newNsatEqn;
}

cmpEquation *cmpDam::getNsatNightEqn() const
{
    return nsatNightEqn;
}

void cmpDam::setNsatNightEqn(cmpEquation *newNsatNightEqn)
{
    nsatNightEqn = newNsatNightEqn;
}

cmpEquation *cmpDam::getNsatBackupEqn() const
{
    return nsatBackupEqn;
}

void cmpDam::setNsatBackupEqn(cmpEquation *newNsatBackupEqn)
{
    nsatBackupEqn = newNsatBackupEqn;
}

float cmpDam::getEntrainFactor() const
{
    return entrainFactor;
}

void cmpDam::setEntrainFactor(float newEntrainFactor)
{
    entrainFactor = newEntrainFactor;
}
