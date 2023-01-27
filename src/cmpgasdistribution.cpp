#include "cmpgasdistribution.h"

cmpGasDistribution::cmpGasDistribution()
{

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
