#include "cmppowerhouse.h"
#include "cmpdatasettings.h"

cmpPowerhouse::cmpPowerhouse(int num)
{
    capacity = 0.0;
    setNumber(num);
    setPriority(num);
    allocate(366, 2);
}

cmpPowerhouse::~cmpPowerhouse()
{
    deleteSpecies();
    if (!schedule.isEmpty())
        schedule.clear();
    if (!flowFraction.isEmpty())
        flowFraction.clear();
}

void cmpPowerhouse::allocate(int days, int slicesPerDay)
{
    int damSlicesDay = slicesPerDay;
    int damSlicesSeason = days * damSlicesDay;
    if (!schedule.isEmpty())
        schedule.clear();
    if (!flowFraction.isEmpty())
        flowFraction.clear();

    for (int i = 0; i < days; i++)
    {
        schedule.append(BoolList());
        for (int j = 0; j < damSlicesDay; j++)
            schedule[i].append(false);
    }
    for (int i = 0; i < damSlicesSeason; i++)
        flowFraction.append(0.0);
}

void cmpPowerhouse::allocateSpecies (int num)
{
    cmpDamSpecies *spc = nullptr;
    while (species.count() < num)
        species.append(new cmpDamSpecies());
    while (species.count() > num)
    {
        spc = species.takeLast();
        delete spc;
    }
    for (int i = 0; i < num; i++)
        species[i]->reset();
}

void cmpPowerhouse::setSpeciesNames(QStringList &spNames)
{
    int total = spNames.count();
    deleteSpecies();
    for (int i = 0; i < total; i++)
        species.append(new cmpDamSpecies(spNames[i]));
}

void cmpPowerhouse::deleteSpecies()
{
    cmpDamSpecies *dspc;
    int total = species.count();
    for (int i = 0; i < total; i++)
    {
        dspc = species.takeFirst();
        delete dspc;
    }
}

void cmpPowerhouse::writeData (cmpFile *outfile, int indent, bool outputAll)
{
    float dval = outputAll? 100000: 0;
    int total = species.count();
    cmpEquation eqn;
    QString name;

    outfile->writeValue(indent, "powerhouse_priority", priority, 0);
    outfile->writeValue(indent, "powerhouse_capacity", capacity, dval);
    outfile->writeValue(indent, "flow_min", threshold, dval);
//    outfile->writeStringNR(indent, "powerhouse_schedule ");
//    outfile->writeBoolArray(0, schedule, false);
    for (int i = 0; i < total; i++)
    {
        name = species[i]->getName();
        outfile->writeTitledValue(indent, "mean_forebay_transit_time", name, species[i]->getMeanForebayTransitTime(), dval);
        outfile->writeTitledValue(indent, "separation_prob", name, species[i]->getSeparationProb(), dval);
        outfile->writeTitledValue(indent, "sluideway_proportion", name, species[i]->getSluicewayProp(), dval);
        outfile->writeTitledValue(indent, "rsw_spill_cap", name, species[i]->getRswCapacity(), dval);
        outfile->writeTitledValue(indent, "sluiceway_mort", name, species[i]->getSluicewayMort(), dval);
        outfile->writeTitledValue(indent, "bypass_mort", name, species[i]->getBypassMort(), dval);
        outfile->writeTitledValue(indent, "turbine_mort", name, species[i]->getTurbineMort(), dval);
        outfile->writeTitledValue(indent, "spill_mort", name, species[i]->getSpillMort(), dval);
        outfile->writeTitledValue(indent, "transport_mort", name, species[i]->getTransportMort(), dval);
        outfile->writeTitledValue(indent, "rsw_mort", name, species[i]->getRswMort(), dval);
        outfile->writeNewline();
        outfile->writeTitledValue(indent, "sluiceway_delay", name, species[i]->getSluicewayDelay(), dval);
        outfile->writeTitledValue(indent, "bypass_delay", name, species[i]->getBypassDelay(), dval);
        outfile->writeTitledValue(indent, "turbine_delay", name, species[i]->getTurbineDelay(), dval);
        outfile->writeTitledValue(indent, "spill_delay", name, species[i]->getSpillDelay(), dval);
        outfile->writeTitledValue(indent, "rsw_delay", name, species[i]->getRswDelay(), dval);
        outfile->writeNewline();
        outfile->writeTitledValue(indent, "sluiceway_day_delay", name, species[i]->getSluicewayDayDelay(), dval);
        outfile->writeTitledValue(indent, "bypass_day_delay", name, species[i]->getBypassDayDelay(), dval);
        outfile->writeTitledValue(indent, "turbine_day_delay", name, species[i]->getTurbineDayDelay(), dval);
        outfile->writeTitledValue(indent, "spill_day_delay", name, species[i]->getSpillDayDelay(), dval);
        outfile->writeTitledValue(indent, "rsw_day_delay", name, species[i]->getRswDayDelay(), dval);
        outfile->writeNewline();
        eqn = species[i]->getFgeEqn();
        outfile->writeTitledValue(indent, "fge_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "fge_equation", name);
        outfile->writeNewline();
        eqn = species[i]->getRswEqn();
        outfile->writeTitledValue(indent, "rsw_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "rsw_equation", name);
        outfile->writeNewline();
        eqn = species[i]->getSpillEqn();
        outfile->writeTitledValue(indent, "spill_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "spill_equation", name);
        outfile->writeNewline();
        eqn = species[i]->getTransEqn();
        outfile->writeTitledValue(indent, "trans_mort_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "trans_mort_equation", name);
        outfile->writeNewline();
        eqn = species[i]->getDelayEqn();
        outfile->writeTitledValue(indent, "delay_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "delay_equation", name);
        outfile->writeNewline();
/*        outfile->writeTitledValue(indent, "fishway_mort", name, species[i]->getFishwayMort(), dval);
        outfile->writeNewline();
        eqn = species[i]->getFishwayMigrEqu();
        outfile->writeTitledValue(indent, "fishway_migr_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "fishway_migr_equation", name);
        outfile->writeNewline();
        eqn = species[i]->getFishwaySurvEqn();
        outfile->writeTitledValue(indent, "fishway_surv_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "fishway_surv_equation", name);
        outfile->writeNewline();*/
    }
}

void cmpPowerhouse::writeAllData (cmpFile *outfile, int indent)
{

}

void cmpPowerhouse::writeSecondData(cmpFile *outfile, int indent, bool outputAll)
{
    float dval = outputAll? 100000: 0;
    int total = species.count();
    cmpEquation eqn;
    QString name;

    outfile->writeValue(indent, "powerhouse_priority", priority, 0);
    outfile->writeValue(indent, "powerhouse_capacity", capacity, dval);
    outfile->writeValue(indent, "flow_min", threshold, dval);
//    outfile->writeValue(indent, "rsw_spill_cap", rswCapacity, dval);
//    outfile->writeStringNR(indent, "powerhouse_schedule");
//    outfile->writeBoolArray(1, schedule, false);
    for (int i = 0; i < total; i++)
    {
        name = species[i]->getName();
//        outfile->writeTitledValue(indent, "mean_forebay_transit_time", species[i]->getFbayTransTime(), dval);
        outfile->writeTitledValue(indent, "separation_prob", name, species[i]->getSeparationProb(), dval);
//        outfile->writeTitledValue(indent, "sluideway_proportion", name, species[i]->getSluicewayProp(), dval);
        outfile->writeTitledValue(indent, "sluiceway_mort", name, species[i]->getSluicewayMort(), dval);
        outfile->writeTitledValue(indent, "bypass_mort", name, species[i]->getBypassMort(), dval);
        outfile->writeTitledValue(indent, "turbine_mort", name, species[i]->getTurbineMort(), dval);
//        outfile->writeTitledValue(indent, "spill_mort", name, species[i]->getSpillMort(), dval);
//        outfile->writeTitledValue(indent, "transport_mort", name, species[i]->getTransportMort(), dval);
//        outfile->writeTitledValue(indent, "rsw_mort", name, species[i]->getRswMort(), dval);
        outfile->writeNewline();
        outfile->writeTitledValue(indent, "sluiceway_delay", name, species[i]->getSluicewayDelay(), dval);
        outfile->writeTitledValue(indent, "bypass_delay", name, species[i]->getBypassDelay(), dval);
        outfile->writeTitledValue(indent, "turbine_delay", name, species[i]->getTurbineDelay(), dval);
        outfile->writeTitledValue(indent, "spill_delay", name, species[i]->getSpillDelay(), dval);
        outfile->writeTitledValue(indent, "rsw_delay", name, species[i]->getRswDelay(), dval);
        outfile->writeNewline();
        outfile->writeTitledValue(indent, "sluiceway_day_delay", name, species[i]->getSluicewayDayDelay(), dval);
        outfile->writeTitledValue(indent, "bypass_day_delay", name, species[i]->getBypassDayDelay(), dval);
        outfile->writeTitledValue(indent, "turbine_day_delay", name, species[i]->getTurbineDayDelay(), dval);
        outfile->writeTitledValue(indent, "spill_day_delay", name, species[i]->getSpillDayDelay(), dval);
        outfile->writeTitledValue(indent, "rsw_day_delay", name, species[i]->getRswDayDelay(), dval);
        outfile->writeNewline();
        eqn = species[i]->getFgeEqn();
        outfile->writeTitledValue(indent, "fge_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "fge_equation", name);
        outfile->writeNewline();
/*        eqn = species[i]->getRswEqn();
        outfile->writeTitledValue(indent, "rsw_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "rsw_equation", name);
        outfile->writeNewline();
        eqn = species[i]->getSpillEqn();
        outfile->writeTitledValue(indent, "spill_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "spill_equation", name);
        outfile->writeNewline();
        eqn = species[i]->getTransEqn();
        outfile->writeTitledValue(indent, "trans_mort_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "trans_mort_equation", name);
        outfile->writeNewline();
        eqn = species[i]->getDelayEqn();
        outfile->writeTitledValue(indent, "delay_equation", name, eqn.getId(), 0);
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "delay_equation", name);
        outfile->writeNewline(); */
    }
}

int cmpPowerhouse::getPriority() const
{
    return priority;
}

void cmpPowerhouse::setPriority(int value)
{
    priority = value;
}

float cmpPowerhouse::getThreshold() const
{
    return threshold;
}

void cmpPowerhouse::setThreshold(float value)
{
    threshold = value;
}

float cmpPowerhouse::getCapacity() const
{
    return capacity;
}

void cmpPowerhouse::setCapacity(float value)
{
    capacity = value;
}

void cmpPowerhouse::setActive (int day, int start, int stop, bool active)
{
    BoolList bl = schedule[day];
    for (int i = start; i <= stop; i++)
        bl[i] = active;
}

bool cmpPowerhouse::getActive (int day, int slice)
{
    return schedule[day][slice];
}

void cmpPowerhouse::setFlowFraction (int day, float fraction)
{
    while (day < flowFraction.count())
        flowFraction.append(0.0);
    flowFraction[day] = fraction;
}

float cmpPowerhouse::getFlowFraction (int day)
{
    return flowFraction.at(day);
}

int cmpPowerhouse::getNumber() const
{
    return number;
}

void cmpPowerhouse::setNumber(int value)
{
    number = value;
}

int cmpPowerhouse::getSpeciesIndex(QString spec)
{
    int total = species.count();
    int index = -1;

    for (int i = 0; i < total; i++)
    {
        if (species[i]->getName().compare(spec, Qt::CaseInsensitive) == 0)
        {
            index = i;
            break;
        }
    }
    return index;
}

float cmpPowerhouse::getSpeciesRswCap(QString spec)
{
    int index = getSpeciesIndex(spec);
    return getSpeciesRswCap(index);
}

float cmpPowerhouse::getSpeciesRswCap(int index)
{
    return species[index]->getRswCapacity();
}

void cmpPowerhouse::setSpeciesRswCap(QString spec, float value)
{
    int index = getSpeciesIndex(spec);
    species[index]->setRswCapacity(value);
}



