#include "PowerHouse.h"
#include "Period.h"


PowerHouse::PowerHouse(int num)
{
    capacity = 0.0;
    setPriority(num);
    allocate();
}

PowerHouse::~PowerHouse()
{
    if (!schedule.isEmpty())
        schedule.clear();
    if (!flowFraction.isEmpty())
        flowFraction.clear();
}

void PowerHouse::allocate()
{
    if (!schedule.isEmpty())
        schedule.clear();
    if (!flowFraction.isEmpty())
        flowFraction.clear();

    for (int i = 0; i < DAYS_IN_SEASON; i++)
    {
        boolPeriod *nwprd = new boolPeriod();
        schedule.append(*nwprd);
        schedule[i].setStart(0);
        schedule[i].setStop(DAM_SLICES_PER_DAY - 1);
        schedule[i].setValue(false);
    }
    for (int i = 0; i < DAM_SLICES_IN_SEASON; i++)
        flowFraction[i] = 0.0;
}
int PowerHouse::getPriority() const
{
    return priority;
}

void PowerHouse::setPriority(int value)
{
    priority = value;
}

float PowerHouse::getThreshold() const
{
    return threshold;
}

void PowerHouse::setThreshold(float value)
{
    threshold = value;
}

float PowerHouse::getCapacity() const
{
    return capacity;
}

void PowerHouse::setCapacity(float value)
{
    capacity = value;
}

void PowerHouse::setActive (int day, int st, int sp, bool active)
{
    schedule[day].setValue(active);
}

bool PowerHouse::getActive (int day, int slice)
{
    return schedule[day].getValue();
}

void PowerHouse::setFlowFraction (int day, float fraction)
{
    while (day < flowFraction.count())
        flowFraction.append(0.0);
    flowFraction[day] = fraction;
}

float PowerHouse::getFlowFraction (int day)
{
    return flowFraction.at(day);
}

