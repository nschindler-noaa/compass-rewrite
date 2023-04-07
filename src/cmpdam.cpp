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
    tailraceLength = 0.0;
    elevBase = 0.0;
    elevForebay = 0.0;
    elevTailrace = 0.0;
    fullHead = 0.0;
    depthForebay = 0.0;
    elevBypass = 0.0;
    collector = 0;

    basinLength = 0.0;
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

    allocate(366, 2, 2);

    spillWeir = nullptr;
    transport = nullptr;
    fishway = nullptr;
}

void cmpDam::resetData()
{
    allocate(366, 2, 2);
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

void cmpDam::setSlicesPerDay(int newStepsPerDay)
{
    allocateSlices(stepsPerDay);
}

void cmpDam::allocateDays(int newDaysPerSeason)
{
    cmpRiverSegment::allocateDays(newDaysPerSeason);
    if (!depthForebayDay.isEmpty())
        depthForebayDay.clear();
    if (!depthTailraceDay.isEmpty())
        depthTailraceDay.clear();
    if (!dropRatioDay.isEmpty())
        dropRatioDay.clear();
    if (!dropRatioDayTR.isEmpty())
        dropRatioDayTR.clear();
    for (int i = 0; i < daysPerSeason; i++)
    {
        depthForebayDay.append(0);
        depthTailraceDay.append(0);
        dropRatioDay.append(0);
        dropRatioDayTR.append(0);
    }
    for (int i = 0; i < powerhouses.count(); i++)
        powerhouses[i]->setNumDays(daysPerSeason);
}

void cmpDam::allocateSlices(int newStepsPerDay)
{
    cmpRiverSegment::allocateSteps(newStepsPerDay);
    if (!spill.isEmpty())
        spill.clear();
    if (!spillPlanned.isEmpty())
        spill.clear();
    if (!daylightProportion.isEmpty())
        daylightProportion.clear();
    for (int i = 0; i < stepsPerSeason; i++)
    {
        spill.append(0);
        spillPlanned.append(0);
        daylightProportion.append(0);
    }
    for (int i = 0; i < powerhouses.count(); i++)
        powerhouses[i]->setNumSlices(stepsPerSeason);
}

void cmpDam::allocate(int days, int steps, int gasSteps)
{
    allocateDays(days);
    setSlicesPerDay(steps);
    allocateGasSteps(gasSteps);
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
            int number = currentPHouse->getNumber();
            QString phname(QString("%1_%2").arg(name).arg(number));
            if (number > 1)
            {
                okay = cfile->checkEnd("additional_powerhouse", phname);
                if (okay)
                    setCurrentPHouse(1);
            }
            else
            {
                okay = cfile->checkEnd("dam", name);
                if (okay)
                    end = true;
            }
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
        okay = cfile->readFloatOrNa(na, tailraceLength);
    }
    else if (token.compare("spill_cap", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, spillMax);
    }
    else if (token.compare("actual_spill", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatArray(spill, stepsPerSeason, Data::None, stepsPerDay, "actual_spill");
    }
    else if (token.compare("storage_volume", Qt::CaseInsensitive) == 0)
    {
        QList<float> templist;
        for (int i = 0; i < daysPerSeason; i++)
            templist.append(0.0);
        okay = cfile->readFloatArray(templist, daysPerSeason, Data::None, 1, "storage_volume");
        basin->setVolume(templist);
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
    else if (token.compare("additional_powerhouse", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readString(tmpstr);
        setCurrentPowerhouse (tmpstr);
    }
    else if (token.compare("powerhouse_priority", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readInt(tmpInt);
        currentPHouse->setPriority(tmpInt);
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
    else if (token.compare("powerhouse_schedule", Qt::CaseInsensitive) == 0)
    {
        getCurrentPHouse()->getSchedule().parseData(cfile, "powerhouse_schedule");
    }
    else if (token.compare("rsw_spill_cap", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, tmpFloat);
        currentPHouse->setRswCapacity(tmpFloat);
    }
    else if (token.compare("mean_forebay_transit_time", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setMeanForebayTransitTime(tmpFloat);
    }
    else if (token.compare("sluiceway_prop", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setSluicewayProp(tmpFloat);
    }
    else if (token.compare("separation_prob", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setSeparationProb(tmpFloat);
    }
    else if (token.compare("sluiceway_mort", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setSluicewayMort(tmpFloat);
    }
    else if (token.compare("bypass_mort", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setBypassMort(tmpFloat);
    }
    else if (token.compare("turbine_mort", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setTurbineMort(tmpFloat);
    }
    else if (token.compare("transport_mort", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setTransportMort(tmpFloat);
    }
    else if (token.compare("spill_mort", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setSpillMort(tmpFloat);
    }
    else if (token.compare("rsw_mort", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setRswMort(tmpFloat);
    }
    else if (token.compare("spill_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setSpillDelay(tmpFloat);
    }
    else if (token.compare("bypass_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setBypassDelay(tmpFloat);
    }
    else if (token.compare("sluiceway_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setSluicewayDelay(tmpFloat);
    }
    else if (token.compare("turbine_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setTurbineDelay(tmpFloat);
    }
    else if (token.compare("rsw_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setRswDelay(tmpFloat);
    }
    else if (token.compare("spill_day_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setSpillDayDelay(tmpFloat);
    }
    else if (token.compare("bypass_day_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setBypassDayDelay(tmpFloat);
    }
    else if (token.compare("sluiceway_day_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setSluicewayDayDelay(tmpFloat);
    }
    else if (token.compare("turbine_day_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setTurbineDayDelay(tmpFloat);
    }
    else if (token.compare("rsw_day_delay", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setRswDayDelay(tmpFloat);
    }
    else if (token.compare("fishway_mortality", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readTitledValue(tmpstr, tmpFloat);
        cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
        if (spec != nullptr)
            spec->setFishwayMort(tmpFloat);
    }
    else if (token.compare("spill_equation", Qt::CaseInsensitive) == 0)
    {
        tmpstr = cfile->popToken();
        okay = cfile->readInt(tmpInt);
        if (okay)
        {
            cmpEquation eqn(tmpInt);
            okay = eqn.parseData(cfile, "spill_equation");
            if (okay)
            {
                cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
                if (spec != nullptr)
                    spec->setSpillEqn(eqn);
            }
        }
    }
    else if (token.compare("fge_equation", Qt::CaseInsensitive) == 0)
    {
        tmpstr = cfile->popToken();
        okay = cfile->readInt(tmpInt);
        if (okay)
        {
            cmpEquation eqn(tmpInt);
            okay = eqn.parseData(cfile, "fge_equation");
            if (okay)
            {
                cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
                if (spec != nullptr)
                    spec->setFgeEqn(eqn);
            }
        }
    }
    else if (token.compare("trans_mort_equation", Qt::CaseInsensitive) == 0)
    {
        tmpstr = cfile->popToken();
        okay = cfile->readInt(tmpInt);
        if (okay)
        {
            cmpEquation eqn(tmpInt);
            okay = eqn.parseData(cfile, "trans_mort_equation");
            if (okay)
            {
                cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
                if (spec != nullptr)
                    spec->setTransEqn(eqn);
            }
        }
    }
    else if (token.compare("delay_equation", Qt::CaseInsensitive) == 0)
    {
        tmpstr = cfile->popToken();
        okay = cfile->readInt(tmpInt);
        if (okay)
        {
            cmpEquation eqn(tmpInt);
            okay = eqn.parseData(cfile, "delay_equation");
            if (okay)
            {
                cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
                if (spec != nullptr)
                    spec->setDelayEqn(eqn);
            }
        }
    }
    else if (token.compare("rsw_equation", Qt::CaseInsensitive) == 0)
    {
        tmpstr = cfile->popToken();
        okay = cfile->readInt(tmpInt);
        if (okay)
        {
            cmpEquation eqn(tmpInt);
            okay = eqn.parseData(cfile, "rsw_equation");
            if (okay)
            {
                cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
                if (spec != nullptr)
                    spec->setRswEqn(eqn);
            }
        }
    }
    else if (token.compare("fishway_migr_equation", Qt::CaseInsensitive) == 0)
    {
        tmpstr = cfile->popToken();
        okay = cfile->readInt(tmpInt);
        if (okay)
        {
            cmpEquation eqn(tmpInt);
            okay = eqn.parseData(cfile, "fishway_migr_equation");
            if (okay)
            {
                cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
                if (spec != nullptr)
                    spec->setFishwayMigrEqn(eqn);
            }
        }
    }
    else if (token.compare("fishway_surv_equation", Qt::CaseInsensitive) == 0)
    {
        tmpstr = cfile->popToken();
        okay = cfile->readInt(tmpInt);
        if (okay)
        {
            cmpEquation eqn(tmpInt);
            okay = eqn.parseData(cfile, "fishway_surv_equation");
            if (okay)
            {
                cmpDamSpecies *spec = currentPHouse->getSpecies(tmpstr);
                if (spec != nullptr)
                    spec->setFishwaySurvEqn(eqn);
            }
        }
    }


    else
    {
        okay = cmpRiverSegment::parseToken (token, cfile);
    }

    return okay;
}

void cmpDam::writeRivData (cmpFile *outfile, int indent, bool outputAll)
{
    float fdefault = outputAll? 1000000: 0.0;
    if (basin != nullptr)
    {
        outfile->writeFloatArray(indent, "storage_volume", QString(), basin->getVolume(), Data::None, stepsPerDay, Data::Float, fdefault);// [*] 2015.80
    }
    writeGasData(outfile, indent, outputAll);
    writeTurbidData(outfile, indent, outputAll);
}

void cmpDam::writeOpsData (cmpFile *outfile, int indent, bool outputAll)
{
    float fdefault = outputAll? 1000000: 0.0;
    int idefault = outputAll? 100000: 0;
    cmpEquation *eqn = nullptr;
    int total = 0;
    int indent2 = indent + 1;

    outfile->writeValue(indent2, "output_settings", outputSettings, idefault);
    outfile->writeValue(indent2, "tailrace_length", tailraceLength, Data::Fixed, fdefault);
    outfile->writeValue(indent2, "spill_cap", spillMax, Data::Fixed, fdefault);
    outfile->writeFloatArray(indent2, "actual_spill", QString(), spill, Data::None, stepsPerDay, Data::Float, fdefault);
    outfile->writeValue(indent2, "gas_theta", gasTheta, Data::Scientific, fdefault);
    outfile->writeValue(indent2, "k_entrain", entrainK, Data::Scientific, fdefault);
    outfile->writeValue(indent2, "entrain_factor", entrainFactor, Data::Scientific, fdefault);
    writeGasData(outfile, indent2, outputAll);
    writeTurbidData(outfile, indent2, outputAll);
    if (basin != nullptr)
    {
        outfile->writeFloatArray(indent2, "storage_volume", QString(), basin->getVolume(), Data::None, stepsPerDay, Data::Float, fdefault);
    }
    outfile->writeNewline();
    eqn = getNsatEqn();
    if (eqn != nullptr && eqn != (new cmpEquation(eqn->getId())))
    {
        outfile->writeValue(indent2, "nsat_day_equation", eqn->getId());
        eqn->writeParameters(outfile, indent2+1, false);
        outfile->writeEnd(indent2, "nsat_day_equation");
    }
    eqn = getNsatNightEqn();
    if (eqn != nullptr && eqn != (new cmpEquation(eqn->getId())))
    {
        outfile->writeValue(indent2, "nsat_night_equation", eqn->getId());
        eqn->writeParameters(outfile, indent2+1, false);
        outfile->writeEnd(indent2, "nsat_night_equation");
    }
    eqn = getNsatBackupEqn();
    if (eqn != nullptr && eqn != (new cmpEquation(eqn->getId())))
    {
        outfile->writeValue(indent2, "nsat_backup_equation", eqn->getId());
        eqn->writeParameters(outfile, indent2+1, false);
        outfile->writeEnd(indent2, "nsat_backup_equation");
    }

    if (!powerhouses.isEmpty())
    {
        total = powerhouses.count();
        bool single = (total == 1);
        powerhouses.at(0)->writeData(outfile, indent2, outputAll, single);
        for (int i = 1; i < total; i++)
        {
            QString name2(QString("%1_%2").arg(name).arg(i+1));
            outfile->writeString(indent2, "additional_powerhouse", name2);// Bonneville_Dam_2
            powerhouses.at(i)->writeSecondData(outfile, indent2+1, outputAll);
            outfile->writeEnd(indent2, "additional_powerhouse", name2);
        }
    }
    outfile->writeEnd(indent, "dam", name);
}


void cmpDam::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    float fdefault = outputAll? 1000000: 0.0;
    int idefault = outputAll? 100000: 0;
    cmpEquation *eqn = nullptr;
    int total = 0;
    int indent2 = indent + 1;
    int indent3 = indent + 2;

    outfile->writeString(indent, "dam", name);

    outfile->writeValue(indent2, "output_settings", outputSettings, idefault);
    outfile->writeValue(indent2, "tailrace_length", tailraceLength, Data::Float, fdefault);
    outfile->writeValue(indent2, "spill_cap", spillMax, Data::Float, fdefault);
    outfile->writeFloatArray(indent2, "actual_spill", QString(), spill, Data::None, stepsPerDay, Data::Float, fdefault);
    outfile->writeValue(indent2, "gas_theta", gasTheta, Data::Scientific, fdefault);
    outfile->writeValue(indent2, "k_entrain", entrainK, Data::Scientific, fdefault);
    outfile->writeValue(indent2, "entrain_factor", entrainFactor, Data::Scientific, fdefault);
    writeGasData(outfile, indent2, outputAll);
    eqn = getNsatEqn();
    if (eqn != nullptr && eqn != (new cmpEquation(eqn->getId())))
    {
        outfile->writeValue(indent2, "nsat_day_equation", eqn->getId());
        eqn->writeParameters(outfile, indent3, outputAll);
        outfile->writeEnd(indent2, "nsat_day_equation");
    }
    outfile->writeNewline();
    eqn = getNsatNightEqn();
    if (eqn != nullptr && eqn != (new cmpEquation(eqn->getId())))
    {
        outfile->writeValue(indent2, "nsat_night_equation", eqn->getId());
        eqn->writeParameters(outfile, indent3, outputAll);
        outfile->writeEnd(indent2, "nsat_night_equation");
    }
    outfile->writeNewline();
    eqn = getNsatBackupEqn();
    if (eqn != nullptr && eqn != (new cmpEquation(eqn->getId())))
    {
        outfile->writeValue(indent2, "nsat_backup_equation", eqn->getId());
        eqn->writeParameters(outfile, indent3, outputAll);
        outfile->writeEnd(indent2, "nsat_backup_equation");
    }
    outfile->writeNewline();
    writeTurbidData(outfile, indent2, outputAll);
    if (basin != nullptr)
    {
        outfile->writeFloatArray(indent2, "storage_volume", QString(), basin->getVolume(), Data::None, stepsPerDay, Data::Float, fdefault);
    }
    outfile->writeNewline();

    if (!powerhouses.isEmpty())
    {
        total = powerhouses.count();
        bool single = (total == 1);
        powerhouses.at(0)->writeData(outfile, indent2, outputAll, single);
        for (int i = 1; i < total; i++)
        {
            QString name2(QString("%1_%2").arg(name).arg(i+1));
            outfile->writeString(indent2, "additional_powerhouse", name2);// Bonneville_Dam_2
            powerhouses.at(i)->writeSecondData(outfile, indent2+1, outputAll);
            outfile->writeEnd(indent2, "additional_powerhouse", name2);
        }
    }
    outfile->writeEnd(indent, "dam", name);
}

void cmpDam::writeAllData(cmpFile *outfile, int indent)
{
    int total = 0;
    int indent2 = indent + 1;
    cmpEquation *eqn;
    outfile->writeValue(indent, "output_settings", outputSettings);
    outfile->writeValue(indent, "tailrace_length", tailraceLength, Data::Float);
    outfile->writeValue(indent, "spill_cap", spillMax, Data::Float);
    outfile->writeFloatArray(indent, "actual_spill", QString(), spill, Data::None, stepsPerDay, Data::Float, 100000.0);
    outfile->writeValue(indent, "gas_theta", gasTheta, Data::Float);
    outfile->writeValue(indent, "k_entrain", entrainK, Data::Float);
    outfile->writeValue(indent, "entrain_factor", entrainFactor, Data::Float);
    if (readGas)
    {
        outfile->writeString(indent, "output_gas", "On");
        // output the gas distribution
    }
    else
    {
        outfile->writeString(indent, "output_gas", "Off");
    }
    if (readTurbidity)
    {
        outfile->writeString(indent, "input_turbidity", "On");
        // output the input turbidity
    }
    else
    {
        outfile->writeString(indent, "input_turbidity", "Off");
    }
    if (basin != nullptr)
    {
        outfile->writeFloatArray(indent, "storage_volume", QString(), basin->getVolume(), Data::None, stepsPerDay, Data::Float, 100000.0);
    }
    outfile->writeNewline();
    eqn = getNsatEqn();
    if (eqn != nullptr)
    {
        outfile->writeValue(indent, "nsat_day_equation", eqn->getId());
        eqn->writeParameters(outfile, indent2, true);
        outfile->writeEnd(indent, "nsat_day_equation");
        outfile->writeNewline();
    }
    eqn = getNsatNightEqn();
    if (eqn != nullptr)
    {
        outfile->writeValue(indent, "nsat_night_equation", eqn->getId());
        eqn->writeParameters(outfile, indent2, true);
        outfile->writeEnd(indent, "nsat_night_equation");
        outfile->writeNewline();
    }
    eqn = getNsatBackupEqn();
    if (eqn != nullptr)
    {
        outfile->writeValue(indent, "nsat_backup_equation", eqn->getId());
        eqn->writeParameters(outfile, indent2, true);
        outfile->writeEnd(indent, "nsat_backup_equation");
        outfile->writeNewline();
    }

    total = powerhouses.count();
    powerhouses.at(0)->writeData(outfile, indent, true, (total == 1));
    for (int i = 1; i < total; i++)
    {
        QString name2(QString("%1_%2").arg(name).arg(i+1));
        outfile->writeString(indent, "additional_powerhouse", name2);// Bonneville_Dam_2
        powerhouses.at(i)->writeSecondData(outfile, indent2, true);
        outfile->writeEnd(indent, "additional_powerhouse", name2);
    }
}

bool cmpDam::parseDesc (cmpFile *descfile)
{
    bool okay = true, end = false;
    int tempInt = 0;
    float tempFloat = 0.0;
    QString token;
    QString na;

    std::cout << "       Parsing Dam description: " << name.toStdString() << std::endl;

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
            okay = descfile->readFloatOrNa(na, basinLength);
        }
        else if (token.compare("sgr", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, specGrav);
        }
        else if (token.compare("powerhouse_capacity", Qt::CaseInsensitive) == 0)
        {
            if (powerhouses.isEmpty())
                powerhouses.append(new cmpPowerhouse());
            powerhouses[0]->setNumber(1);
            okay = descfile->readFloatOrNa(na, tempFloat);
            if (okay && powerhouses.count() > 0)
                powerhouses.at(0)->setCapacity(tempFloat);
        }
        else if (token.compare("powerhouse_2_capacity", Qt::CaseInsensitive) == 0)
        {
            while (powerhouses.count() < 2)
                powerhouses.append(new cmpPowerhouse());
            powerhouses[1]->setNumber(2);
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
        outfile->writeValue(indent2, "floor_elevation", getElevBase(), Data::Float);
        outfile->writeValue(indent2, "forebay_elevation", getElevForebay(), Data::Float);
        outfile->writeValue(indent2, "tailrace_elevation", getElevTailrace(), Data::Float);
        tmpfloat = getHeightBypass();
        if (tmpfloat > 0.1)
            outfile->writeValue(indent2, "bypass_elevation", tmpfloat, Data::Float);
        outfile->writeValue(indent2, "spillway_width", getSpillway()->getWidth(), Data::Float);
        outfile->writeString(indent2, "spill_side", getSpillSideText());
        tmpint = getSpillway()->getNumGates();
        if (tmpint > 0)
        {
            outfile->writeValue(indent2, "ngates", tmpint);
            outfile->writeValue(indent2, "gate_width", getSpillway()->getGateWidth(), Data::Float);
            outfile->writeValue(indent2, "pergate", getSpillway()->getPerGate(), Data::Float);
        }
        outfile->writeValue(indent2, "basin_length", getLengthBasin(), Data::Float);
        outfile->writeValue(indent2, "sgr", specGrav, Data::Float);
        for (int i = 0; i < num; i++)
        {
            if (i == 0)
                outfile->writeValue(indent2, "powerhouse_capacity", powerhouses.at(i)->getCapacity(), Data::Float);

            else
                outfile->writeValue(indent2, QString("powerhouse_%1_capacity").arg(QString::number(i+1)),
                                    powerhouses.at(i)->getCapacity(), Data::Float);
        }

        if (getFishway() != nullptr) {
            outfile->writeString(indent2, "fishway");
            outfile->writeString(3, "type", getFishway()->getTypeString());
            outfile->writeValue(3, "length", getFishway()->getLength(), Data::Float);
            outfile->writeValue(3, "capacity", getFishway()->getCapacity(), Data::Float);
            outfile->writeValue(3, "velocity", getFishway()->getVelocity(), Data::Float);
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
    return tailraceWidth;
}

void cmpDam::setWidthTailrace(float value)
{
    tailraceWidth = value;
}

float cmpDam::getLengthTailrace() const
{
    return tailraceLength;
}

void cmpDam::setLengthTailrace(float value)
{
    tailraceLength = value;
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
    return basinLength;
}

void cmpDam::setLengthBasin(float value)
{
    basinLength = value;
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

cmpFloatPeriodList *cmpDam::getSpillPlannedDay() const
{
    return spillPlannedDay;
}

void cmpDam::setSpillPlannedDay(cmpFloatPeriodList *value)
{
    spillPlannedDay = value;
}

cmpFloatPeriodList *cmpDam::getSpillPlannedNight() const
{
    return spillPlannedNight;
}

void cmpDam::setSpillPlannedNight(cmpFloatPeriodList *value)
{
    spillPlannedNight = value;
}

cmpFloatPeriodList *cmpDam::getSpillLegacyPlanned() const
{
    return spillLegacyPlanned;
}

void cmpDam::setSpillLegacyPlanned(cmpFloatPeriodList *value)
{
    spillLegacyPlanned = value;
}

cmpFloatPeriodList *cmpDam::getSpillLegacyFish() const
{
    return spillLegacyFish;
}

void cmpDam::setSpillLegacyFish(cmpFloatPeriodList *value)
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

cmpPowerhouse *cmpDam::getCurrentPHouse() const
{
    return currentPHouse;
}

void cmpDam::setCurrentPHouse(int number)
{
    int total = powerhouses.count();
    for (int i = 0; i < total; i++)
    {
        if (powerhouses.at(i)->getNumber() == number)
            setCurrentPHouse(powerhouses.at(i));
    }
}

void cmpDam::setCurrentPHouse(cmpPowerhouse *newCurrentPHouse)
{
    currentPHouse = newCurrentPHouse;
}

bool cmpDam::setCurrentPowerhouse(QString tmpname)
{
    bool okay = false;
    cmpPowerhouse *phouse = nullptr;
    int number = 0;
    QString phname;
    for (int i = 0, total = powerhouses.count(); i < total; i++)
    {
        number = powerhouses.at(i)->getNumber();
        phname = QString("%1_%2").arg(name).arg(number);
        if (tmpname.compare(phname, Qt::CaseInsensitive) == 0)
        {
            phouse = powerhouses[i];
            break;
        }
    }
    if (phouse != nullptr)
    {
        setCurrentPHouse(phouse);
        okay = true;
    }
    return okay;
}
