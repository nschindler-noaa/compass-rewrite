#include <QVector>

#include "Basin.h"

Basin::Basin()
{
    volumeMin = 0;
    volumeMax = 100;
    allocate (366);
}

void Basin::allocate(int numDays){
    flow.clear();
    volume.clear();
    for (int i = 0; i < numDays; i++) {
        flow.append(0);
        volume.append(0);
    }
}

float Basin::getFlow(int day) const
{
    float value = 0;
    if (day < flow.count())
        value = flow[day];
    return value;
}

void Basin::setFlow(int day, const float &value)
{
    while (flow.count() <= day)
        flow.append(0);
    flow[day] = value;
}

float Basin::getVolumeMin() const
{
    return volumeMin;
}

void Basin::setVolumeMin(float value)
{
    volumeMin = value;
}

float Basin::getVolumeMax() const
{
    return volumeMax;
}

void Basin::setVolumeMax(float value)
{
    volumeMax = value;
}

float Basin::getVolume(int day) const
{
    float value = 0;
    if (day < volume.count())
        value = volume[day];
    return value;
}

void Basin::setVolume(int day, const float &value)
{
    while (volume.count() <= day)
        volume.append(0);
    volume[day] = value;
}
