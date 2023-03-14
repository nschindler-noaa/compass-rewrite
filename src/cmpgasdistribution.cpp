#include "cmpgasdistribution.h"

cmpGasDistribution::cmpGasDistribution()
{

}

void cmpGasDistribution::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    float dval = outputAll? 1000000: 0;
    if (used)
    {
        outfile->writeFloatArray(indent, "output_gas", "On", gasRight, Data::None, 1, Data::Float, dval);
    }
    else
    {
        outfile->writeStringNR(indent, "output_gas", "Off");
    }
}

bool cmpGasDistribution::parseData(cmpFile *cfile)
{
    bool okay = true, end = false;
    QString token, tmpStr;

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Gas distribution data.");
            okay = false;
        }
        else if (token.compare("On", Qt::CaseInsensitive) == 0)
        {
            setUsed(true);
            cfile->readFloatArray(gasRight, 366, Data::None, 1, "gas distribution");

        }
        else if (token.compare("Off", Qt::CaseInsensitive) == 0)
        {
            setUsed(false);
            end = true;
        }
        else
        {
            cfile->pushToken(token);
            okay = false;
        }
    }
    return okay;
}

const QList<float> &cmpGasDistribution::getGasRight() const
{
    return gasRight;
}

void cmpGasDistribution::setGasRight(const QList<float> &newGasRight)
{
    gasRight = newGasRight;
}

const QList<float> &cmpGasDistribution::getGasLeft() const
{
    return gasLeft;
}

void cmpGasDistribution::setGasLeft(const QList<float> &newGasLeft)
{
    gasLeft = newGasLeft;
}

const QList<float> &cmpGasDistribution::getFlow() const
{
    return flow;
}

void cmpGasDistribution::setFlow(const QList<float> &newFlow)
{
    flow = newFlow;
}

const QList<float> &cmpGasDistribution::getFlow_fraction() const
{
    return flow_fraction;
}

void cmpGasDistribution::setFlow_fraction(const QList<float> &newFlow_fraction)
{
    flow_fraction = newFlow_fraction;
}

FlowLocation cmpGasDistribution::getFraction_side() const
{
    return fraction_side;
}

void cmpGasDistribution::setFraction_side(FlowLocation newFraction_side)
{
    fraction_side = newFraction_side;
}

bool cmpGasDistribution::getUsed() const
{
    return used;
}

void cmpGasDistribution::setUsed(bool newUsed)
{
    used = newUsed;
}
