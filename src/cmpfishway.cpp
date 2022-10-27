#include "cmpfishway.h"

cmpFishway::cmpFishway()
{
    type = Ladder;
    length = 0.0;
    capacity = 0.0;
    velocity = 0.0;
}

bool cmpFishway::parseDesc(cmpFile *descfile)
{
    bool okay = true, end = false;
    QString token;
    QString na;

    while (okay && !end)
    {
        token = descfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            descfile->printEOF("Dam fishway description");
            okay = false;
        }
        else if (token.compare("type", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readString(token);
            setType(token);
        }
        else if (token.compare("length", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, length);
        }
        else if (token.compare("capacity", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, capacity);
        }
        else if (token.compare("velocity", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, velocity);
        }

        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            descfile->checkEnd("fishway");
            end = true;
        }
        else
        {
            descfile->unknownToken(token, "fishway");
        }
    }
    return okay;
}

void cmpFishway::writeDesc(cmpFile *outfile)
{
    outfile->writeString(1, "fishway");
    outfile->writeString(2, "type", getTypeString());
    outfile->writeValue(2, "length", length);
    outfile->writeValue(2, "capacity", capacity);
    outfile->writeValue(2, "velocity", velocity);
    outfile->writeEnd(1, "fishway");
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
