#include "cmpbasin.h"
#include "cmpfile.h"

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

bool cmpBasin::parseDesc(QString text)
{
    bool okay = true;
    QStringList tokens = text.split(' ', QString::SkipEmptyParts);
    if (tokens.count() == 2)
    {
        setVolumeMin(tokens[0].toFloat(&okay));
        if (okay)
            setVolumeMax(tokens[1].toFloat(&okay));
        if (okay)
            setText(text);
    }
    else
    {
        setText();
        okay = false;
    }
    return okay;
}

void cmpBasin::setText(QString text)
{
    if (text.isEmpty())
    {
        valueStr.clear();
        valueStr.append(QString("%1 %2")
                    .arg(QString::number(volumeMin, 'g', 2)
                    .arg(QString::number(volumeMax, 'g', 2))));
    }
    else
    {
        valueStr = text;
    }
}

QString &cmpBasin::getText()
{
    return valueStr;
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


bool cmpBasin::parseData(cmpFile *infile, QString name)
{
    bool okay = true;
    QString token;
    okay = infile->readFloatArray(volume, volume.count(), Data::None, 1, QString("storage_volume for %1").arg(name));
    return okay;
}

void cmpBasin::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    float dval = outputAll? 1000000: 0;
    outfile->writeFloatArray(indent, QString("storage_volume"), QString(), volume, Data::None, 1, Data::Float, dval);
}


float cmpBasin::getFlowOnDay(int day) const
{
    float value = 0;
    if (day < flow.count())
        value = flow[day];
    return value;
}

void cmpBasin::setFlowOnDay(int day, const float &value)
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


float cmpBasin::getVolumeOnDay(int day) const
{
    float value = 0;
    if (day < volume.count())
        value = volume[day];
    return value;
}

void cmpBasin::setVolumeOnDay(int day, const float &value)
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

QList<float> &cmpBasin::getFlow()
{
    return flow;
}

void cmpBasin::setFlow(const QList<float> &newFlow)
{
    flow = newFlow;
}

QList<float> &cmpBasin::getVolume()
{
    return volume;
}

void cmpBasin::setVolume(const QList<float> &newVolume)
{
    volume = newVolume;
}

