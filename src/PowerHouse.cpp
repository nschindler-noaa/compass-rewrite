#include "PowerHouse.h"
#include "Period.h"


PowerHouse::PowerHouse(int num)
{
    capacity = 0.0;
    setNumber(num);
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
        schedule.append(BoolList());
        for (int j = 0; j < DAM_SLICES_PER_DAY; j++)
            schedule[i][j] = false;
    }
    for (int i = 0; i < DAM_SLICES_IN_SEASON; i++)
        flowFraction.append(0.0);
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
    for (int j = st; j <= sp; j++)
        schedule[day][j] = active;
}

bool PowerHouse::getActive (int day, int slice)
{
    return schedule[day][slice];
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

int PowerHouse::getNumber() const
{
    return number;
}

void PowerHouse::setNumber(int value)
{
    number = value;
}

