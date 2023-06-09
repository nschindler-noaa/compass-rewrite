#include "cmpriversystem.h"


cmpRiverSystem::cmpRiverSystem(QObject *parent) :
    QObject(parent)
{
    setup ();
}

cmpRiverSystem::cmpRiverSystem(QString riverFile, QObject *parent) :
    QObject(parent)
{
    cmpFile cf(riverFile);
    setup();
    parseDesc (&cf);
}


void cmpRiverSystem::setup ()
{
    species.append(new cmpSpecies());
    stocks.append(new cmpStock());
    rivers.append(new cmpRiver(this));
    releaseSites.append(new cmpReleaseSite());
    speciesNames.append(QString());
    stockNames.append(QString());
    reachClassNames.append(QString());
    powerhouses.append(QString());
    dams.append(QString());
    reaches.append(QString());
    headwaters.append(QString());
    basins.append(QString());
    deleteAll();
}

cmpRiverSystem::~cmpRiverSystem ()
{
    deleteAll ();
}

void cmpRiverSystem::deleteAll()
{
    while (species.count())
        delete species.takeLast();
    while (stocks.count())
        delete stocks.takeLast();

    while (rivers.count())
        delete rivers.takeLast();

    while (releaseSites.count())
        delete (releaseSites.takeLast());

//    releases;

    speciesNames.clear();
    stockNames.clear();
    powerhouses.clear();
    dams.clear();
    reaches.clear();
    headwaters.clear();
    basins.clear();
}

const QStringList &cmpRiverSystem::getReachClassNames() const
{
    return reachClassNames;
}

void cmpRiverSystem::setReachClassName(int index, const QString &newReachClassName)
{
    while (reachClassNames.count() < index)
        reachClassNames.append(QString());
    reachClassNames[index] = newReachClassName;
}

void cmpRiverSystem::resetData()
{
    int i = 0;
    int totalspecies = species.count();
    int totalstocks = stocks.count();
    int totaldams = dams.count();
    int totalreaches = reaches.count();
    int totalheadwtrs = headwaters.count();
    for (i = 0; i < totalspecies; i++)
        species[i]->setDefaults();
    for (i = 0; i < totalstocks; i++)
        stocks[i]->setDefaults();
    for (i = 0; i < totaldams; i++)
    {
        cmpDam *dam = static_cast<cmpDam *>(findSegment(dams.at(i)));
        dam->resetData();
    }
    for (i = 0; i < totalreaches; i++)
    {
        cmpReach *reach = static_cast<cmpReach *>(findSegment(reaches.at(i)));
        reach->resetData();
    }
    for (i = 0; i < totalheadwtrs; i++)
    {
        cmpHeadwater *headwtr = static_cast<cmpHeadwater *>(findSegment(headwaters.at(i)));
        headwtr->resetData();
    }
    deleteReleases();
}

cmpSettings *cmpRiverSystem::getSettings() const
{
    return cSettings;
}

void cmpRiverSystem::setSettings(cmpSettings *newSettings)
{
    cSettings = newSettings;
}

bool cmpRiverSystem::parseDesc(cmpFile *descfile)
{
    bool okay = true, end = false;
    QString token (""), val ("");
    QString name;
    cmpReleaseSite *newrelsite = nullptr;
    cmpRiver *river = nullptr;

    while (okay && !end)
    {
        token = descfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            descfile->printMessage("EOF in Compass file.");
            end = true;
        }
        else if (token.compare ("species") == 0)
        {
            okay = descfile->readString(name);
            name = name.simplified().replace(' ', '_');
            speciesNames.append(name);
        }
        else if (token.compare ("stock") == 0)
        {
            okay = descfile->readString(name);
            name = name.simplified().replace(' ', '_');
            stockNames.append(name);
        }
        else if (token.compare ("release_site") == 0)
        {
            okay = descfile->readString(name);
            name = name.simplified().replace(' ', '_');
            newrelsite = new cmpReleaseSite(name);
            newrelsite->parseDesc(descfile);
            releaseSites.append(newrelsite);
        }
        else if (token.compare ("river") == 0)
        {
            river = new cmpRiver(this);
            okay = descfile->readString(name);
            name = name.simplified().replace(' ', '_');
            river->setName(name);
            okay = river->parseDesc(descfile);

            rivers.append(river);
        }
        else if (token.startsWith("#"))
        {
            descfile->skipLine();
        }
    }
    return okay;
}

bool cmpRiverSystem::parseData(cmpFile *cfile)
{
    bool okay = true, end = false;
    QString token, stringval;
    QString name;
    QStringList tokens;
    int index = 0;
    int tmpInt = 0;
    float tmpFloat = 0;
    cmpDataSettings *dSettings = cSettings->getDataSettings();

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            if (!okay)
                cfile->printError("Found EOF in Compass file.");
            end = true;
        }
        else if (token.compare("migration") == 0)
        {
            okay = cfile->readString(name);
            if (okay) dSettings->setMigration(name);
        }
        else if (token.compare("days_in_season") == 0)
        {
            okay = cfile->readInt(tmpInt);
            if(okay)
            {
                dSettings->setNumDaysInSeason(tmpInt);
                setNumDays(tmpInt);
            }
        }
        else if (token.compare("time_steps_per_day") == 0)
        {
            okay = cfile->readInt(tmpInt);
            if(okay)
            {
                dSettings->setTimeStepsPerDay(tmpInt);
                setNumSteps(tmpInt);
            }
        }
        else if (token.compare("dam_slices_per_day") == 0)
        {
            okay = cfile->readInt(tmpInt);
            if(okay)
            {
                dSettings->setDamSlicesPerDay(tmpInt);
                setNumSlices(tmpInt);
            }
        }
        else if (token.compare("gas_steps_per_day") == 0)
        {
            okay = cfile->readInt(tmpInt);
            if(okay)
            {
                dSettings->setGasStepsPerDay(tmpInt);
                setNumGasSteps(tmpInt);
            }
        }
        else if (token.compare("day_start") == 0)
        {
            okay = cfile->readInt(tmpInt);
            if(okay) dSettings->setDayStart(tmpInt);
        }
        else if (token.compare("night_start") == 0)
        {
            okay = cfile->readInt(tmpInt);
            if (okay) dSettings->setNightStart(tmpInt);
        }
        else if (token.compare("compute_gas") == 0)
        {
            okay = cfile->readString(stringval);
            if (stringval.compare("on", Qt::CaseInsensitive) == 0)
                dSettings->setCalcGas(true);
            else
                dSettings->setCalcGas(false);
        }
        else if (token.compare("compute_growth") == 0)
        {
            okay = cfile->readString(stringval);
            if (stringval.compare("on", Qt::CaseInsensitive) == 0)
                dSettings->setCalcGrowth(true);
            else
                dSettings->setCalcGrowth(false);
        }
        else if (token.compare("compute_turbidity") == 0)
        {
            okay = cfile->readString(stringval);
            if (stringval.compare("on", Qt::CaseInsensitive) == 0)
                dSettings->setCalcTurbidity(true);
            else
                dSettings->setCalcTurbidity(false);
        }
        else if (token.compare("num_reach_classes") == 0)
        {
            okay = cfile->readInt(tmpInt);
            if (okay)
            {
                for (int i = 0; i < tmpInt; i++)
                {
                    token = cfile->popToken();
                    if (token.compare("reach_class_name") == 0)
                    {
                        okay = cfile->readInt(tmpInt);
                        if (okay) okay = cfile->readString(name);
                        if (okay) setReachClassName(tmpInt, name);
                    }
                    else
                    {
                        cfile->pushToken(token);
                        i = tmpInt;
                    }
                }
            }
        }
        else if (token.compare ("species") == 0)
        {
            okay = cfile->readString(name);
            if (speciesNames.contains(name))
            {
                index = speciesNames.indexOf(name);
                species[index]->setDefaults();
                species[index]->setReachClassNames(reachClassNames);
                species[index]->parseData(cfile);
            }
            else
            {
                cfile->printError(QString("Species name %1 not found in species list.").arg(name));
                cfile->skipToEnd();
            }
        }
        else if (token.compare ("stock") == 0)
        {
            okay = cfile->readString(name);
            if (stockNames.contains(name))
            {
                QString stName = name;
                int stIndex = 0;
                stIndex = stockNames.indexOf(stName);
                token = cfile->popToken();
                if (token.compare("species", Qt::CaseInsensitive) == 0)
                {
                    okay = cfile->readString(name);
                    if (okay)
                    {
                        index = speciesNames.indexOf(name);
                        if (index < 0)
                        {
                            cfile->printError(QString("Species name %1 not found in species list.").arg(name));
                            cfile->skipToEnd();
                        }
                        else
                        {
                            stocks[stIndex]->copy(*static_cast<cmpStock *>(species.at(index)));
                            stocks[stIndex]->setName(stName);
                            stocks[stIndex]->setSpeciesName(name);
//                            stocks[stIndex]->setReachClassNames(reachClassNames);
                            stocks[stIndex]->parseData(cfile);
                        }
                    }
                    else
                    {
                        cfile->skipToEnd();
                    }
                }
                else
                {
                    cfile->printError(QString("'species' token not found. It must be the first item in stock definition."));
                    cfile->skipToEnd();
                }
            }
            else
            {
                cfile->printError(QString("Stock name %1 not found in stock name list.").arg(name));
                cfile->skipToEnd();
            }
        }
        else if (token.compare ("post_bonneville_hypothesis", Qt::CaseInsensitive) == 0 ||
                 token.compare ("fish_return_hypothesis", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(name);
            if (okay)
                dSettings->setFishReturnHyp(name);
        }
        else if (token.compare ("reach", Qt::CaseInsensitive) == 0)
        {
            QString reachName;
            okay = cfile->readString (reachName);
            if (okay)
            {
                if (reaches.contains(reachName))
                {
                    cmpReach *reach = static_cast<cmpReach *>(findSegment(reachName));
                   // reach->allocateDays(numDays, timeSteps);
                    okay = reach->parseData (cfile);
                }
                else
                {
                    cfile->printError(QString("Reach name %1 not found.").arg(name));
                    cfile->skipToEnd();
                }
            }
        }
        else if (token.compare ("dam") == 0)
        {
            QString damName;
            okay = cfile->readString (damName);
            if (okay)
            {
                if (dams.contains(damName))
                {
                    cmpDam *dam = static_cast<cmpDam *>(findSegment(damName));
                   // dam->allocateDays(numDays, damSlices);
                    dam->setSpeciesNames(speciesNames);
                    okay = dam->parseData (cfile);
                }
                else
                {
                    cfile->printError(QString("Dam name %1 not found.").arg(name));
                    cfile->skipToEnd();
                }
            }
        }
        else if (token.compare ("headwater") == 0)
        {
            QString hwName;
            okay = cfile->readString (hwName);
            if (okay)
            {
                if (headwaters.contains(hwName))
                {
                    cmpHeadwater *hwtr = static_cast<cmpHeadwater *>(findSegment(hwName));
                   // hwtr->allocateDays(numDays);
                    okay = hwtr->parseData (cfile);
                }
                else
                {
                    cfile->printError(QString("Headwater name %1 not found.").arg(name));
                    cfile->skipToEnd();
                }
            }
        }
        else if (token.compare ("release") == 0)
        {
            QString relName;
            okay = cfile->readString (relName);
            if (okay)
            {
                tokens = relName.split(' ', QString::SkipEmptyParts);
                if (tokens.count() < 3)
                    okay = false;
                else
                {
                    cmpRelease *newrel = new cmpRelease();
                    cmpSpecies *spec = findSpecies(tokens.at(0));
                    cmpReleaseSite *site = findReleaseSite(tokens.at(1));
                    token = tokens.at(2);
                    tmpInt = token.toInt(&okay);
                    if (okay)
                    {
                        releases.append(newrel);
                        newrel->setSpecies(spec);
                        newrel->setSite(site);
                        newrel->setStartDay(tmpInt);
                        newrel->setName(relName.replace(' ', '_'));
                        newrel->parseData(cfile);
                        cmpStock *stk = findStock(newrel->getStockName());
                        newrel->setStock(stk);
                        site = findReleaseSite(newrel->getDestSiteName());
                        newrel->setDestination(site);
                    }
                }
            }
        }
        else if (token.compare ("gas_dissp_exp") == 0)
        {
            okay = cfile->readFloatOrNa(stringval, tmpFloat);
            if (okay) dSettings->setGasDisspExp(tmpFloat);
        }
        else if (token.compare ("hw_flow_prop") == 0)
        {
            okay = cfile->readFloatOrNa(stringval, tmpFloat);
            if (okay) dSettings->setHwFlowProp(tmpFloat);
        }
        else if (token.compare ("ufree_eqn") == 0)
        {
            okay = cfile->readInt(tmpInt);
            if (okay)
            {
                cmpEquation *fflow = new cmpEquation(tmpInt);
                fflow->parseData(cfile, "ufree_eqn");
                dSettings->setFreeFlowEqn(fflow);
            }
        }
        else if (token.compare("ufree_max", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readFloatOrNa(stringval, tmpFloat);
            if (okay) dSettings->setFreeFlowMax(tmpFloat);
        }
        else if (token.compare("prey_energy_density", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readFloatOrNa(stringval, tmpFloat);
            if (okay) dSettings->setPreyEnergyDensity(tmpFloat);
        }
        else if (token.compare("length_weight_b0", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readFloatOrNa(stringval, tmpFloat);
            if (okay) dSettings->setLengthWeightB0(tmpFloat);
        }
        else if (token.compare("length_weight_b1", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readFloatOrNa(stringval, tmpFloat);
            if (okay) dSettings->setLengthWeightB1(tmpFloat);
        }
        else if (token.compare("fork_threshold", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readFloatOrNa(stringval, tmpFloat);
            if (okay) dSettings->setForkThreshold(tmpFloat);
        }
        else if (token.compare("water_travel_upper_segment", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(stringval);
            if (okay) dSettings->setWaterTravelUpperSegment(stringval);
        }
        else if (token.compare("water_travel_lower_segment", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(stringval);
            if (okay) dSettings->setWaterTravelLowerSegment(stringval);
        }
        else if (token.compare("water_travel_first_day", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readInt(tmpInt);
            if (okay) dSettings->setWaterTravelFirstDay(tmpInt);
        }
        else if (token.compare("water_travel_last_day", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readInt(tmpInt);
            if (okay) dSettings->setWaterTravelLastDay(tmpInt);
        }
        else if (token.compare("min_migration_rate", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readFloatOrNa(stringval, tmpFloat);
            if (okay) dSettings->setMigrationRateMin(tmpFloat);
        }
        else if (token.compare("suppress_variation", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(stringval);
            if (okay)
            {
                if (stringval.compare("on", Qt::CaseInsensitive) == 0)
                    dSettings->setSuppressVariation(true);
                else
                    dSettings->setSuppressVariation(false);
            }
        }
        else if (token.compare("pred_vol_interaction", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(stringval);
            if (okay)
            {
                if (stringval.compare("on", Qt::CaseInsensitive) == 0)
                    dSettings->setPredVolInteraction(true);
                else
                    dSettings->setPredVolInteraction(false);
            }
        }
        else if (token.compare("age_dependent_fge", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(stringval);
            if (okay)
            {
                if (stringval.compare("on", Qt::CaseInsensitive) == 0)
                    dSettings->setAgeDependentFge(true);
                else
                    dSettings->setAgeDependentFge(false);
            }
        }
        else if (token.compare("truncate_travel_vect", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(stringval);
            if (okay)
            {
                if (stringval.compare("on", Qt::CaseInsensitive) == 0)
                    dSettings->setTruncateTravelVect(true);
                else
                    dSettings->setTruncateTravelVect(false);
            }
        }
        else if (token.compare("truncate_res_survival", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(stringval);
            if (okay)
            {
                if (stringval.compare("on", Qt::CaseInsensitive) == 0)
                    dSettings->setTruncateResSurvival(true);
                else
                    dSettings->setTruncateResSurvival(false);
            }
        }
        else if (token.compare("compute_mu_method", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readInt(tmpInt);
            if (okay)
                dSettings->setComputeMuMethod(tmpInt);
        }
        else if (token.compare("year_traveltime_indicator", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readInt(tmpInt);
            if (okay)
                dSettings->setYearTraveltimeIndicator(tmpInt);
        }
        else if (token.compare("mortality_class", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(stringval);
            if (okay)
                dSettings->setMortClass(stringval);
        }
        else if (token.contains ("end"))
        {
            cfile->skipLine ();
            end = true;
        }
        else
        {
            cfile->unknownToken(token);
            cfile->skipLine ();
        }
    }
    return okay;
}

bool cmpRiverSystem::parseReleaseSite(cmpFile *cfile, cmpReleaseSite *relsite)
{
    bool okay = true;
    QString line;
    cmpRiverPoint *rivpt = new cmpRiverPoint();
    okay = cfile->readString(line);
    if (okay)
    {
        rivpt->parse(line);
        relsite->setLatlon(rivpt);
        releaseSites.append(relsite);
        okay = cfile->checkEnd ("release_site", relsite->getName());
    }
    else
    {
        cfile->printError(QString("problem reading release site %1").arg(relsite->getName()));
    }
    return okay;
}

bool cmpRiverSystem::outputDesc(cmpFile *descfile)
{
    bool okay = true;
    QString name, val;

    if (!descfile->isOpen())
        okay = descfile->open(QIODevice::WriteOnly);

    if (okay)
    {
        // output river system values
        // species
        for (int i = 0; i < speciesNames.count(); i++)
        {
            name = speciesNames.at(i);
            descfile->writeString(0, "species", name.replace('_', ' '));
        }
        descfile->writeNewline();
        // stocks
        for (int i = 0; i < stockNames.count(); i++)
        {
            name = stockNames.at(i);
            descfile->writeString(0, "stock", name.replace('_', ' '));
        }
        descfile->writeNewline();
        // release sites
        for (int i = 0, total = releaseSites.count(); i < total; i++)
        {
            releaseSites.at(i)->outputDesc(descfile);
            descfile->writeNewline();
        }

        // output rivers
        for (int i = 0, total = rivers.count(); i < total; i++)
        {
            rivers.at(i)->outputDesc(descfile);
            descfile->writeNewline();
        }
    }
    else
    {
        okay = false;
    }
    return okay;
}

bool cmpRiverSystem::outputData(cmpFile *cfile, bool outputAll)
{
    bool okay = true;
    if (cfile->isOpen())
    {
        for (int i = 0; i < rivers.count(); i++)
        {
            rivers.at(i)->outputData(cfile, outputAll);
        }
    }
    else {
        okay = false;
    }
    return okay;
}

bool cmpRiverSystem::outputAllSpecies(cmpFile *outfile, bool outputAll)
{
    bool okay = true;
    int total = species.count();
    for (int i = 0; i < total; i++)
    {
        species.at(i)->writeData(outfile, 0, outputAll);
        outfile->writeNewline();
    }
    return okay;
}

bool cmpRiverSystem::outputAllStocks(cmpFile *outfile, bool outputAll)
{
    bool okay = true;
    int total = stocks.count();
    for (int i = 0; i < total; i++)
    {
        stocks.at(i)->writeData(outfile, 0, outputAll);
        outfile->writeNewline();
    }
    return okay;
}

bool cmpRiverSystem::outputPostRiverData(cmpFile *outfile, bool outputAll)
{
    bool okay = true;
    int total = species.count();
    QString name(cSettings->getDataSettings()->getFishReturnHypStr());
    outputPostRiverHypothesis (outfile, outputAll);
    outfile->writeNewline();
    // species return eqns
    for (int i = 0; i < total; i++)
    {
        name = species.at(i)->getName();
        outfile->writeString(0, "species", name);
        outfile->writeNewline();
        species[i]->writeFishReturnEqns(outfile, 1, outputAll);
        outfile->writeNewline();
        outfile->writeEnd(0, "species", name);
        outfile->writeNewline();
    }
    return okay;
}

bool cmpRiverSystem::outputPostRiverHypothesis(cmpFile *outfile, bool outputAll)
{
    bool okay = true;
    QString title = QString("fish_return_hypothesis");
    QString name(cSettings->getDataSettings()->getFishReturnHypStr());

    if (outfile->getDataVersion() < 20)
    {
        title = QString("post_bonneville_hypothesis");
    }
    if (outputAll || name.compare("sar_vs_date") != 0)
        outfile->writeString(0, title, name);
    return okay;
}
bool cmpRiverSystem::outputCalibData(cmpFile *outfile, bool outputAll)
{
    bool okay = true;
    int total = reachClassNames.count();
    outfile->writeValue(0, "num_reach_classes", total);
    for (int i = 0; i < total; i++)
    {
        outfile->writeString(1, "reach_class_name", QString::number(i), reachClassNames.at(i));
    }
    outfile->writeNewline();
    okay = outputAllSpecies(outfile, outputAll);
    outfile->writeNewline();
    if (okay)
        okay = outputAllStocks(outfile, outputAll);
    outfile->writeNewline();
    if (okay)
        okay = outputConfigData(outfile, outputAll);
    outfile->writeNewline();
    return okay;
}

bool cmpRiverSystem::outputConfigData(cmpFile *outfile, bool outputAll)
{
    int total = segments.count();
    cmpRiverSegment::SegmentType stype;
    QString type, name, rClass = QString();
    for (int i = 0; i < total; i++)
    {
        name = segments.at(i)->getName();
        stype = segments.at(i)->getType();
        type = segments.at(i)->getTypeStr();
        outfile->writeString(1, type, name);
        switch (stype)
        {
        case cmpRiverSegment::Reach:
            static_cast<cmpReach *>(segments.at(i))->writeConfigData(outfile, 2, outputAll);
            break;
        default:
            segments.at(i)->writeConfigData(outfile, 2, outputAll);
        }
        outfile->writeEnd(1, type, name);
        outfile->writeNewline();

        rClass.clear();
    }
    return true;
}

bool cmpRiverSystem::outputRiverYrData(cmpFile *outfile, bool outputAll)
{
    bool okay = true;
    int total = segments.count();
    cmpDam *dam;
    cmpReach *reach;
    cmpHeadwater *hwater;
    for (int i = 0; i < total; i++)
    {
        switch (segments.at(i)->getType())
        {
        case cmpRiverSegment::Dam:
            dam = static_cast<cmpDam *>(segments.at(i));
            outfile->writeString(1, "dam", dam->getName());
            dam->writeRivData(outfile, 2, outputAll);
            outfile->writeEnd(1, "dam", dam->getName());
            break;
        case cmpRiverSegment::Reach:
            reach = static_cast<cmpReach *>(segments[i]);
            outfile->writeString(1, "reach", reach->getName());
            reach->writeRivData(outfile, 2, outputAll);
            outfile->writeEnd(1, "reach", reach->getName());
            outfile->writeNewline();
            break;
        case cmpRiverSegment::Headwater:
            hwater = static_cast<cmpHeadwater *>(segments.at(i));
            outfile->writeString(1, "headwater", hwater->getName());
            hwater->writeRivData(outfile, 2, outputAll);
            outfile->writeEnd(1, "headwater", hwater->getName());
            break;
        }
    }
    return okay;
}

bool cmpRiverSystem::outputDamOpsData(cmpFile *outfile, bool outputAll)
{
    bool okay = true;
    int total = segments.count();
    cmpDam *dam;
    for (int i = 0; i < total; i++)
    {
        if (segments.at(i)->getType() == cmpRiverSegment::Dam)
        {
            dam = static_cast<cmpDam *>(segments.at(i));
            outfile->writeString(1, "dam", dam->getName());
            dam->writeOpsData(outfile, 2, outputAll);
            outfile->writeEnd(1, "dam", dam->getName());
            outfile->writeNewline();
        }
    }
    return okay;
}

bool cmpRiverSystem::outputReleaseData(cmpFile *outfile, bool outputAll)
{
    bool okay = true;
    int total = releases.count();
    cmpRelease *rels;

    for (int i = 0; i < total; i++)
    {
        releases.at(i)->writeData(outfile, outputAll);
        outfile->writeNewline();
    }
    return okay;
}


bool cmpRiverSystem::construct()
{
    bool okay = true;
    cmpRiver *riv = nullptr;
    cmpRiverSegment *cur = nullptr;
    cmpRiverSegment *prev = nullptr;
    QString name;
    QString curRiver ("");

    if (segments.count() < 2)
        okay = false;

    if (okay) {
        // create species
        for (int i = 0, total = speciesNames.count(); i < total; i++)
        {
            name = speciesNames.at(i);
            species.append(new cmpSpecies());
            species[i]->setName(name);
        }
        // create stocks
        for (int i = 0, total = stockNames.count(); i < total; i++)
        {
            name = stockNames.at(i);
            stocks.append(new cmpStock());
            stocks[i]->setName(name);
        }
        // connect segments
        prev = static_cast<cmpRiverSegment *> (segments.at (0));
        curRiver = QString (prev->getRiverName());
        for (int i = 1; okay && i < segments.count(); i++)
        {
            cur = static_cast<cmpRiverSegment *> (segments.at(i));
            if (curRiver.compare(cur->getRiverName()) != 0)
            {
                // create headwater of previous river, if it doesn't exist
                if (prev->getType() != cmpRiverSegment::Headwater)
                {
                    QString hname (curRiver);
                    hname.append(" Headwater");
                    prev->setUpperSegment(new cmpHeadwater (hname, prev->getRiver()));
                    prev->getUpperSegment()->setLowerSegment(prev);
                    segments.insert (i, prev->getUpperSegment());
                    i++;
                    headwaters.append(hname);
//                    cmpLog::outlog->add(cmpLog::Debug, QString (
//                              QString("adding headwater %1").arg(hname)));
                }
                // change rivers, if exist
                curRiver = QString (cur->getRiverName());
                riv = findRiver(curRiver);
                if (riv == nullptr)
                {
//                    cmpLog::outlog->add (cmpLog::Error, QString (
//                               QString("cmpRiver name %1 not found when constructing system").arg (
//                                   curRiver)));
                    okay = false;
                }
                else
                {
                    // find previous segment by top latlon
                    prev = findSegment (cur->getBottomPoint());
                    if (prev == nullptr)
                    {
                        okay = false;
//                        cmpLog::outlog->add(cmpLog::Error, QString ("faulty cmpRiver construction."));
                    }
                    else
                    {
                        prev->setForkSegment(cur);
                        cur->setLowerSegment(prev);
                    }
                }
            }
            else
            {
                prev->setUpperSegment(cur);
                cur->setLowerSegment(prev);
            }
            prev = cur;
        }
    }
    return okay;
}

bool cmpRiverSystem::initialize()
{
    bool okay = true;
    cmpDam *dm;
    cmpReach *rc;
    cmpHeadwater *hw;
    for (int i = 0, total = segments.count(); i < total; i++)
    {
        switch (segments.at(i)->getType())
        {
        case cmpRiverSegment::Headwater:
            hw = static_cast<cmpHeadwater*>(segments[i]);
            hw->resetData();
            break;
        case cmpRiverSegment::Reach:
            rc = static_cast<cmpReach *>(segments[i]);
            rc->setType(cmpRiverSegment::Reach);
            rc->setup();
            break;
        case cmpRiverSegment::Dam:
            dm = static_cast<cmpDam *>(segments[i]);
            dm->setup();
            break;
        }
    }

    return okay;
}

cmpRiver * cmpRiverSystem::findRiver(QString name)
{
    cmpRiver *riv = nullptr;
    for (int i = 0; i < rivers.count(); i++)
    {
        riv = rivers.at (i);
        if (riv->getName().compare(name) == 0)
            break;
    }
    return riv;
}

cmpRiverSegment * cmpRiverSystem::findSegment(QString name)
{
    cmpRiverSegment *seg = nullptr;
    for (int i = 0; i < segments.count(); i++)
    {
        seg = segments.at (i);
        if (seg->getName().compare(name) == 0)
            break;
    }
    return seg;
}

cmpRiverSegment * cmpRiverSystem::findSegment(cmpRiverPoint *pt)
{
    cmpRiverSegment *seg = nullptr;
    bool found = false;
    for (int i = 0; !found && i < segments.count(); i++)
    {
        seg = segments.at (i);
        for (int j = 0; !found && j < seg->getCourse().count(); j++)
            if (seg->getCourse().at (j)->equals(*pt))
                found = true;
    }
    return seg;
}

cmpSpecies * cmpRiverSystem::findSpecies(QString name)
{
    cmpSpecies *spec = nullptr;
    for (int i = 0; i < species.count(); i++)
    {
        spec = species.at (i);
        if (spec->getName().compare(name) == 0)
            break;
    }
    return spec;
}

cmpStock * cmpRiverSystem::findStock(QString name)
{
    cmpStock *st = nullptr;
    for (int i = 0; i < stocks.count(); i++)
    {
        st = stocks.at (i);
        if (st->getName().compare(name) == 0)
            break;
    }
    return st;
}

cmpTransport * cmpRiverSystem::findTransport(QString name)
{
    cmpTransport *trans = nullptr;
    cmpDam *dm = static_cast<cmpDam *> (findSegment(name));
    if (!(dm == nullptr))
        trans = dm->getTransport();
    return trans;
}

cmpRelease * cmpRiverSystem::findRelease(QString name)
{
    cmpRelease *rel = nullptr;
    for (int i = 0; i < releases.count(); i++)
    {
        rel = releases.at (i);
        if (rel->getName().compare(name) == 0)
            break;
    }
    return rel;
}

cmpReleaseSite * cmpRiverSystem::findReleaseSite(QString name)
{
    cmpReleaseSite *site = nullptr;
    for (int i = 0, total = releaseSites.count(); i < total; ++i)
    {
        site = releaseSites.at (i);
        if (site->getName().compare(name) == 0)
            break;
    }
    return site;
}

void cmpRiverSystem::computeFlows ()
{
    markRegulationPts ();
    fillHeadwaters ();
    computeSegFlow (getMouth ());
}
void cmpRiverSystem::markRegulationPts()
{
    for (int i = 0, total = dams.count(); i < total; ++i)
    {
        QString damname = dams.at (i);
        cmpDam *dam = static_cast <cmpDam *> (findSegment(damname));
        dam->setIsRegPoint(true);
    }
}

/** Upstream flow propogation (to headwaters) */

void cmpRiverSystem::fillHeadwaters ()
{
    QString hwtrname ("");
    cmpHeadwater *hwtr;

    for (int i = 0, total = headwaters.count(); i < total; ++i)
    {
        hwtrname = headwaters.at (i);
        hwtr = static_cast <cmpHeadwater *> (findSegment (hwtrname));
        hwtr->fillRegulated();
    }
    for (int i = 0, total = headwaters.count(); i < total; ++i)
    {
        hwtrname = headwaters.at (i);
        hwtr = static_cast <cmpHeadwater *> (findSegment (hwtrname));
        hwtr->fillUnRegulated();
    }
}

/** Downstream flow propogation, done recursively.
 *  Headwaters are always done first, then
 *  the downstream segments.  */

void cmpRiverSystem::computeSegFlow (cmpRiverSegment *seg)
{
    if (seg->getUpperSegment() != nullptr)
    {
            computeSegFlow(seg->getUpperSegment());
        if (seg->getForkSegment() != nullptr)
            computeSegFlow(seg->getForkSegment());

        if (seg->getType() == cmpRiverSegment::Dam)
            static_cast <cmpDam *> (seg)->calculateFlow();
        else if (seg->getType() == cmpRiverSegment::Reach)
            static_cast <cmpReach *> (seg)->calculateFlow();
    }
    else if (seg->getType() == cmpRiverSegment::Headwater)
    {
        static_cast <cmpHeadwater *> (seg)->calculateFlow();
    }
    else
    {
        QString msg (QString ("Segment %1 is not headwater and has no upstream segment.").arg (seg->getName()));
//        cmpLog::outlog->add(cmpLog::Error, msg);
        return;
    }
}

int cmpRiverSystem::computeAllFlows()
{
    int retval = 0;

    if (segments.count() == 0)
    {
        std::cout << "No river segments - execution stopped. " << std::endl;
        retval = -1;
    }
    else if (releases.count() == 0)
    {
        std::cout << "No releases - execution stopped." << std::endl;
        retval = -2;
    }
    retval = computeAllFlows(segments[0]);

    return retval;
}

int cmpRiverSystem::computeAllFlows(cmpRiverSegment *seg)
{
    int retval = 0;
    cmpDam *dam;
    cmpReach *reach;
    cmpHeadwater *headwtr;
    QString msg;

    if (seg == nullptr)
    {
        retval = -1;
    }
    else
    {
        if (seg->getUpperSegment() != nullptr)
            retval += computeAllFlows(seg->getUpperSegment());
        if (seg->getForkSegment() != nullptr)
            retval += computeAllFlows(seg->getForkSegment());

        if (cSettings->getDataSettings()->getInterrupt())
        {
            std::cout << "Execution interrupted. " << std::endl;
            retval = 1;
        }
        else
        {
            msg = QString(QString("Calculating flows for segment %1\n").arg(seg->getName()));
            std::cout << msg.toStdString() << std::endl;
    //        log_msg(L_DBGMSG, msg);

            switch (seg->getType())
            {
            case cmpRiverSegment::Dam:
            {
                dam = static_cast<cmpDam *>(seg);
                retval += dam->calculateFlow();// compute_dam_flow (seg);
                retval += dam->calculateTemp();//compute_dam_flow_temp (seg);
                if (cSettings->getDataSettings()->getCalcTurbidity())
                    retval += dam->calculateTurb();//compute_dam_flow_turbidity (seg);
                if (cSettings->getDataSettings()->getCalcGas())
                    retval += dam->calculateGas();//compute_dam_flow_gas (seg);
        //            dam->calculateRouteProbs(releases.at(0)->getSpecies());//calc_route_prob(seg, current->release_list.at(0)->species->index);
                break;
            }
            case cmpRiverSegment::Reach:
            {
                reach = static_cast<cmpReach *>(seg);
                retval += reach->calculateFlow();//compute_reach_flow (seg);
                retval += reach->calculateTemp();//compute_reach_flow_temp (seg);
                if (cSettings->getDataSettings()->getCalcTurbidity())
                    retval += reach->calculateTurb();//compute_reach_flow_turbidity (seg);
                if (cSettings->getDataSettings()->getCalcGas())
                    retval += reach->calculateGas();//compute_reach_flow_gas (seg);
                /* print some stats for day 1 */
        //            log_day_stats (seg, 1);
                break;
            }
            case cmpRiverSegment::Headwater:
            {
                headwtr = static_cast<cmpHeadwater *>(seg);
                retval += headwtr->calculateFlow();// compute_headwater_flow (seg);
                retval += headwtr->calculateTemp();// compute_headwater_flow_temp (seg);
                if (cSettings->getDataSettings()->getCalcTurbidity())
                    retval += headwtr->calculateTurb();// += compute_headwater_flow_turbidity (seg);
                if (cSettings->getDataSettings()->getCalcGas())
                    retval += headwtr->calculateGas();// compute_headwater_flow_gas (seg);
                break;
            }
            }
        }
    }
    return retval;
}

/** Downstream temperature propogation. Headwaters are always done first, then
 *  the downstream segments. */

void cmpRiverSystem::computeTemps ()
{
    getMouth()->calculateTemps();
}

void cmpRiverSystem::computeSegTemp (cmpRiverSegment *seg)
{
    if (seg->getUpperSegment() != nullptr)
    {
        computeSegTemp(seg->getUpperSegment());
        if (seg->getForkSegment() != nullptr)
            computeSegTemp(seg->getForkSegment());

        if (!seg->getReadTemps())
        {
            if (seg->getType() == cmpRiverSegment::Dam)
                static_cast <cmpDam *> (seg)->calculateTemp ();
            else if (seg->getType() == cmpRiverSegment::Reach)
                static_cast <cmpReach *> (seg)->calculateTemp ();
        }
    }
    else if (seg->getType() == cmpRiverSegment::Headwater)
    {
        if (!seg->getReadTemps())
            static_cast<cmpHeadwater *>(seg)->calculateTemp ();
    }
    else
    {
        QString msg (QString ("Segment %1 is not headwater and has no upstream segment.").arg (seg->getName()));
//        cmpLog::outlog->add(cmpLog::Error, msg);
    }
}

void cmpRiverSystem::computeSpill ()
{
//    computeSegSpill (getMouth());
    for (int i = 0, total = segments.count(); i < total; ++i)
    if (segments.at (i)->getType() == cmpRiverSegment::Dam)
    {
        static_cast <cmpDam *> (segments.at (i))->calculateSpill ();
    }
}

void cmpRiverSystem::computeSegSpill(cmpRiverSegment *seg)
{
    if (seg->getType() == cmpRiverSegment::Dam)
    {
        static_cast <cmpDam *> (seg)->calculateSpill ();
    }
}

void cmpRiverSystem::deleteReleases ()
{
    int i = 0, totalreleases = (releases.isEmpty()? 0: releases.count());
    for (; i < totalreleases; i++)
        delete releases.at(i);
    releases.clear();
}

void cmpRiverSystem::deleteSpill ()
{
    for (int i = 0, total = segments.count(); i < total; ++i)
    if (segments.at (i)->getType() == cmpRiverSegment::Dam)
    {
        static_cast <cmpDam *> (segments.at (i))->deleteSpill ();
    }
 }

void cmpRiverSystem::allocate(int numDays, int timeSteps, int damSlices, int gasSteps)
{
    int totalSegs = segments.count();
    cmpRiverSegment::SegmentType type;
    cmpReach *reach = nullptr;
    cmpDam *dam = nullptr;
    cmpHeadwater *hwater = nullptr;
    for (int i = 0; i < totalSegs; i ++)
    {
        type = segments.at(i)->getType();
        switch (type)
        {
        case cmpRiverSegment::Reach:
            reach = static_cast<cmpReach*>(segments.at(i));
            reach->allocate(numDays, timeSteps, gasSteps);
            break;
        case cmpRiverSegment::Dam:
            dam = static_cast<cmpDam*>(segments.at(i));
            dam->allocate(numDays, damSlices, gasSteps);
            dam->setSpeciesNames(speciesNames);
            break;
        case cmpRiverSegment::Headwater:
            hwater = static_cast<cmpHeadwater*>(segments.at(i));
            hwater->allocate(numDays, timeSteps, gasSteps);
            break;
        }
    }
}

void cmpRiverSystem::setNumDays(int newNumDays)
{
    int totalSegs = segments.count();
    cmpRiverSegment::SegmentType type;
    cmpReach *reach = nullptr;
    cmpDam *dam = nullptr;
    cmpHeadwater *hwater = nullptr;
    for (int i = 0; i < totalSegs; i ++)
    {
        type = segments.at(i)->getType();
        switch (type)
        {
        case cmpRiverSegment::Reach:
            reach = static_cast<cmpReach*>(segments.at(i));
            reach->setDaysPerSeason(newNumDays);
            break;
        case cmpRiverSegment::Dam:
            dam = static_cast<cmpDam*>(segments.at(i));
            dam->setDaysPerSeason(newNumDays);
            break;
        case cmpRiverSegment::Headwater:
            hwater = static_cast<cmpHeadwater*>(segments.at(i));
            hwater->setDaysPerSeason(newNumDays);
            break;
        }
    }
}

void cmpRiverSystem::setNumSteps(int newNumSteps)
{
    int totalSegs = segments.count();
    cmpRiverSegment::SegmentType type;
    cmpReach *reach = nullptr;
    cmpDam *dam = nullptr;
    cmpHeadwater *hwater = nullptr;
    for (int i = 0; i < totalSegs; i ++)
    {
        type = segments.at(i)->getType();
        switch (type)
        {
        case cmpRiverSegment::Reach:
            reach = static_cast<cmpReach*>(segments.at(i));
            reach->setStepsPerDay(newNumSteps);
            break;
        case cmpRiverSegment::Dam:
            dam = static_cast<cmpDam*>(segments.at(i));
            dam->setStepsPerDay(newNumSteps);
            break;
        case cmpRiverSegment::Headwater:
            hwater = static_cast<cmpHeadwater*>(segments.at(i));
            hwater->setStepsPerDay(newNumSteps);
            break;
        }
    }
}

void cmpRiverSystem::setNumSlices(int newNumSlices)
{
    int totalSegs = segments.count();
    cmpRiverSegment::SegmentType type;
    cmpReach *reach = nullptr;
    cmpDam *dam = nullptr;
    cmpHeadwater *hwater = nullptr;
    for (int i = 0; i < totalSegs; i ++)
    {
        type = segments.at(i)->getType();
        if (type == cmpRiverSegment::Dam)
        {
            dam = static_cast<cmpDam*>(segments.at(i));
            dam->setSlicesPerDay(newNumSlices);
            break;
        }
    }
}

void cmpRiverSystem::setNumGasSteps(int newNumGasSteps)
{
    int totalSegs = segments.count();
    cmpRiverSegment::SegmentType type;
    cmpReach *reach = nullptr;
    cmpDam *dam = nullptr;
    cmpHeadwater *hwater = nullptr;
    for (int i = 0; i < totalSegs; i ++)
    {
        type = segments.at(i)->getType();
        switch (type)
        {
        case cmpRiverSegment::Reach:
            reach = static_cast<cmpReach*>(segments.at(i));
            reach->setGasStepsPerDay(newNumGasSteps);
            break;
        case cmpRiverSegment::Dam:
            dam = static_cast<cmpDam*>(segments.at(i));
            dam->setGasStepsPerDay(newNumGasSteps);
            break;
        case cmpRiverSegment::Headwater:
            hwater = static_cast<cmpHeadwater*>(segments.at(i));
            hwater->setGasStepsPerDay(newNumGasSteps);
            break;
        }
    }
}



