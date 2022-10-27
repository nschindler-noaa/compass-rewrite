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
            schedule[i][j] = false;
    }
    for (int i = 0; i < damSlicesSeason; i++)
        flowFraction.append(0.0);
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

