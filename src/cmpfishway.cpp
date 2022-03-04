#include "cmpfishway.h"

cmpFishway::cmpFishway()
{
    type = Ladder;
    length = 0.0;
    capacity = 0.0;
    velocity = 0.0;
}

cmpFishway::Type cmpFishway::getType() const
{
    return type;
}

bool cmpFishway::setType(QString &value)
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

void cmpFishway::setType(const cmpFishway::Type &value)
{
    type = value;
}

QString &cmpFishway::getTypeString()
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

float cmpFishway::getLength() const
{
    return length;
}

void cmpFishway::setLength(float value)
{
    length = value;
}

float cmpFishway::getCapacity() const
{
    return capacity;
}

void cmpFishway::setCapacity(float value)
{
    capacity = value;
}

float cmpFishway::getVelocity() const
{
    return velocity;
}

void cmpFishway::setVelocity(float value)
{
    velocity = value;
}
