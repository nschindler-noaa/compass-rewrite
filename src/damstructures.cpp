#include "damstructures.h"

Spillway::Spillway()
{
    width = 0;
    numGates = 0;
    perGate = 0;
    gateWidth = 0;
}

float Spillway::getWidth() const
{
    return width;
}

void Spillway::setWidth(float value)
{
    width = value;
}

int Spillway::getNumGates() const
{
    return numGates;
}

void Spillway::setNumGates(int value)
{
    numGates = value;
}

float Spillway::getPerGate() const
{
    return perGate;
}

void Spillway::setPerGate(float value)
{
    perGate = value;
}

float Spillway::getGateWidth() const
{
    return gateWidth;
}

void Spillway::setGateWidth(float value)
{
    gateWidth = value;
}


Fishway::Fishway()
{
    type = Ladder;
    length = 0.0;
    capacity = 0.0;
    velocity = 0.0;
}

FishwayType Fishway::getType() const
{
    return type;
}

bool Fishway::setType(QString &value)
{
    bool okay = true;
    if (value.contains("Ladder", Qt::CaseInsensitive))
        setType(Ladder);

    else if (value.contains("Bypass", Qt::CaseInsensitive))
        setType(Bypass);

    else if (value.contains("Lift", Qt::CaseInsensitive))
        setType(Lift);

    else if (value.contains("Trans", Qt::CaseInsensitive))
        setType(Trans);

    else
        okay = false;

    return okay;
}

void Fishway::setType(const FishwayType &value)
{
    type = value;
}

QString &Fishway::getTypeString()
{
    QString *fwType = new QString("");
    switch (type)
    {
    case Ladder:
        fwType->append("Ladder");
        break;
    case Bypass:
        fwType->append("Bypass");
        break;
    case Lift:
        fwType->append("Lift");
        break;
    case Trans:
        fwType->append("Trans");
        break;
    default:
        fwType->append("None");

    }
}

float Fishway::getLength() const
{
    return length;
}

void Fishway::setLength(float value)
{
    length = value;
}

float Fishway::getCapacity() const
{
    return capacity;
}

void Fishway::setCapacity(float value)
{
    capacity = value;
}

float Fishway::getVelocity() const
{
    return velocity;
}

void Fishway::setVelocity(float value)
{
    velocity = value;
}
