#include "cmpbasin.h"

cmpBasin::cmpBasin()
{
    volumeMin = 0;
    volumeMax = 100;
    allocate (366);
}

cmpBasin::~cmpBasin()
{
    flow.clear();
    volume.clear();
}


void cmpBasin::allocate(int numDays)
{
    flow.clear();
    volume.clear();
    for (int i = 0; i < numDays; i++)
    {
        flow.append(0);
        volume.append(0);
    }
}


float cmpBasin::getFlow(int day) const
{
    float value = 0;
    if (day < flow.count())
        value = flow[day];
    return value;
}

void cmpBasin::setFlow(int day, const float &value)
{
    while (flow.count() <= day)
        flow.append(0);
    flow[day] = value;
}


float cmpBasin::getVolumeMin() const
{
    return volumeMin;
}

void cmpBasin::setVolumeMin(float value)
{
    volumeMin = value;
}


float cmpBasin::getVolumeMax() const
{
    return volumeMax;
}

void cmpBasin::setVolumeMax(float value)
{
    volumeMax = value;
}


float cmpBasin::getVolume(int day) const
{
    float value = 0;
    if (day < volume.count())
        value = volume[day];
    return value;
}

void cmpBasin::setVolume(int day, const float &value)
{
    float newValue = value;
    if (newValue < volumeMin)
        newValue = volumeMin;
    if (newValue > volumeMax)
        newValue = volumeMax;
    while (volume.count() <= day)
        volume.append(0);
    volume[day] = newValue;
}

