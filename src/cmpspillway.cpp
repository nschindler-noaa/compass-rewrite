#include "cmpspillway.h"

cmpSpillway::cmpSpillway()
{
    width = 0;
    numGates = 0;
    perGate = 0;
    gateWidth = 0;
}

float cmpSpillway::getWidth() const
{
    return width;
}

void cmpSpillway::setWidth(float value)
{
    width = value;
}

int cmpSpillway::getNumGates() const
{
    return numGates;
}

void cmpSpillway::setNumGates(int value)
{
    numGates = value;
}

float cmpSpillway::getPerGate() const
{
    return perGate;
}

void cmpSpillway::setPerGate(float value)
{
    perGate = value;
}

float cmpSpillway::getGateWidth() const
{
    return gateWidth;
}

void cmpSpillway::setGateWidth(float value)
{
    gateWidth = value;
}
