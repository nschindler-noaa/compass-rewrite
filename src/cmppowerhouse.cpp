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
    if (!flowFraction.isEmpty())
        flowFraction.clear();
}

void cmpPowerhouse::allocate(int days, int slicesPerDay)
{
    int damSlicesDay = slicesPerDay;
    int damSlicesSeason = days * damSlicesDay;

    schedule.allocate(days);

    if (!flowFraction.isEmpty())
        flowFraction.clear();
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

cmpDamSpecies *cmpPowerhouse::getSpecies(QString name)
{
    int index = getSpeciesIndex(name);
    return getSpecies(index);
}

cmpDamSpecies *cmpPowerhouse::getSpecies(int index)
{
    if (index < 0 || index >= species.count())
        return nullptr;
    return species[index];
}

void cmpPowerhouse::writeData (cmpFile *outfile, int indent, bool outputAll)
{
    float fdefault = outputAll? 1000000: 0;
//    int idefault = outputAll? 100000: 0;
    int total = species.count();
    cmpEquation eqn;
    QString name;

    if (number > 1)
        outfile->writeValue(indent, "powerhouse_priority", priority);
    outfile->writeValue(indent, "flow_min", threshold, Data::Fixed, fdefault);
    outfile->writeValue(indent, "powerhouse_capacity", capacity, Data::Fixed, fdefault);
    schedule.writeData(outfile, indent, "powerhouse_schedule", outputAll);
//    outfile->writeString(indent, "powerhouse_schedule", "0:365 (0:24)");
    outfile->writeValue(indent, "rsw_spill_cap", rswCapacity, Data::Fixed, fdefault);
    outfile->writeNewline();
    for (int i = 0; i < total; i++)
    {
        writeSpeciesData(outfile, indent, outputAll, i);
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

    outfile->writeValue(indent, "powerhouse_priority", priority);
    outfile->writeValue(indent, "flow_min", threshold, Data::Fixed, dval);
    outfile->writeValue(indent, "powerhouse_capacity", capacity, Data::Fixed, dval);
    outfile->writeString(indent, "powerhouse_schedule", "0:365 (0:24)");
    //    writeSchedule(outfile, indent, outputAll);
    outfile->writeValue(indent, "rsw_spill_cap", rswCapacity, Data::Fixed, dval);
    outfile->writeNewline();
    for (int i = 0; i < total; i++)
    {
        writeSecondSpeciesData(outfile, indent, outputAll, i);
    }
}

void cmpPowerhouse::writeSpeciesData(cmpFile *outfile, int indent, bool outputAll, int index)
{
    float fdefault = outputAll? 1000000: 0;
    int idefault = outputAll? 100000: 0;
    cmpEquation eqn;
    QString name;

    name = species[index]->getName();
    outfile->writeTitledValue(indent, "mean_forebay_transit_time", name, species[index]->getMeanForebayTransitTime(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "separation_prob", name, species[index]->getSeparationProb(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "sluiceway_prop", name, species[index]->getSluicewayProp(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "sluiceway_mort", name, species[index]->getSluicewayMort(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "bypass_mort", name, species[index]->getBypassMort(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "turbine_mort", name, species[index]->getTurbineMort(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "spill_mort", name, species[index]->getSpillMort(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "rsw_mort", name, species[index]->getRswMort(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "transport_mort", name, species[index]->getTransportMort(), Data::Fixed, fdefault);
    outfile->writeNewline();
    outfile->writeTitledValue(indent, "sluiceway_delay", name, species[index]->getSluicewayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "bypass_delay", name, species[index]->getBypassDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "turbine_delay", name, species[index]->getTurbineDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "spill_delay", name, species[index]->getSpillDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "rsw_delay", name, species[index]->getRswDelay(), Data::Fixed, fdefault);
    outfile->writeNewline();
    outfile->writeTitledValue(indent, "sluiceway_day_delay", name, species[index]->getSluicewayDayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "bypass_day_delay", name, species[index]->getBypassDayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "turbine_day_delay", name, species[index]->getTurbineDayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "spill_day_delay", name, species[index]->getSpillDayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "rsw_day_delay", name, species[index]->getRswDayDelay(), Data::Fixed, fdefault);
    outfile->writeNewline();
    eqn = species[index]->getFgeEqn();
    if (eqn.getId() != idefault)
    {
        outfile->writeTitledValue(indent, "fge_equation", name, eqn.getId());
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "fge_equation", name);
        outfile->writeNewline();
    }
    eqn = species[index]->getSpillEqn();
    if (eqn.getId() != idefault)
    {
        outfile->writeTitledValue(indent, "spill_equation", name, eqn.getId());
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "spill_equation", name);
        outfile->writeNewline();
    }
    eqn = species[index]->getRswEqn();
    if (eqn.getId() != idefault)
    {
        outfile->writeTitledValue(indent, "rsw_equation", name, eqn.getId());
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "rsw_equation", name);
        outfile->writeNewline();
    }
    eqn = species[index]->getTransEqn();
    if (eqn.getId() != idefault)
    {
        outfile->writeTitledValue(indent, "trans_mort_equation", name, eqn.getId());
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "trans_mort_equation", name);
        outfile->writeNewline();
    }
    eqn = species[index]->getDelayEqn();
    if (eqn.getId() != idefault)
    {
        outfile->writeTitledValue(indent, "delay_equation", name, eqn.getId());
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "delay_equation", name);
        outfile->writeNewline();
    }
    outfile->writeTitledValue(indent, "fishway_mortality", name, species[index]->getFishwayMort(), Data::Fixed, fdefault);
    outfile->writeNewline();
    eqn = species[index]->getFishwayMigrEqn();
    if (eqn.getId() != idefault)
    {
        outfile->writeTitledValue(indent, "fishway_migr_equation", name, eqn.getId());
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "fishway_migr_equation", name);
        outfile->writeNewline();
    }
    eqn = species[index]->getFishwaySurvEqn();
    if (eqn.getId() != idefault)
    {
        outfile->writeTitledValue(indent, "fishway_surv_equation", name, eqn.getId());
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "fishway_surv_equation", name);
        outfile->writeNewline();
    }
}

void cmpPowerhouse::writeSecondSpeciesData(cmpFile *outfile, int indent, bool outputAll, int index)
{
    float fdefault = outputAll? 1000000: 0;
    int idefault = outputAll? 100000: 0;
    cmpEquation eqn;
    QString name;

    name = species[index]->getName();
    outfile->writeTitledValue(indent, "sluiceway_prop", name, species[index]->getSluicewayProp(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "sluiceway_mort", name, species[index]->getSluicewayMort(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "bypass_mort", name, species[index]->getBypassMort(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "turbine_mort", name, species[index]->getTurbineMort(), Data::Fixed, fdefault);
    outfile->writeNewline();
    outfile->writeTitledValue(indent, "sluiceway_delay", name, species[index]->getSluicewayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "bypass_delay", name, species[index]->getBypassDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "turbine_delay", name, species[index]->getTurbineDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "spill_delay", name, species[index]->getSpillDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "rsw_delay", name, species[index]->getRswDelay(), Data::Fixed, fdefault);
    outfile->writeNewline();
    outfile->writeTitledValue(indent, "sluiceway_day_delay", name, species[index]->getSluicewayDayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "bypass_day_delay", name, species[index]->getBypassDayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "turbine_day_delay", name, species[index]->getTurbineDayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "spill_day_delay", name, species[index]->getSpillDayDelay(), Data::Fixed, fdefault);
    outfile->writeTitledValue(indent, "rsw_day_delay", name, species[index]->getRswDayDelay(), Data::Fixed, fdefault);
    outfile->writeNewline();
    eqn = species[index]->getFgeEqn();
    if (eqn.getId() != idefault)
    {
        outfile->writeTitledValue(indent, "fge_equation", name, eqn.getId());
        eqn.writeParameters(outfile, indent+1, outputAll);
        outfile->writeEnd(indent, "fge_equation", name);
        outfile->writeNewline();
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
    cmpDaySchedule bl = schedule.getDaySchedule(day);
    bl.setHours(start, stop, active);
}

bool cmpPowerhouse::getActive (int day, int slice)
{
    return schedule.getDaySchedule(day).isOn(slice);
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


float cmpPowerhouse::getRswCapacity() const
{
    return rswCapacity;
}

void cmpPowerhouse::setRswCapacity(float newRswCapacity)
{
    rswCapacity = newRswCapacity;
}

cmpSchedule &cmpPowerhouse::getSchedule()
{
    return schedule;
}

void cmpPowerhouse::setSchedule(const cmpSchedule &newSchedule)
{
    schedule = newSchedule;
}



