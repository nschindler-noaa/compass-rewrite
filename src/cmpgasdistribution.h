#ifndef CMPGASDISTRIBUTION_H
#define CMPGASDISTRIBUTION_H

#include <QStringList>

enum FlowLocation {
    FLOW_RIGHT = 0,      /**< Right side of the dam */
    FLOW_LEFT,           /**< Left side of the dam */
    NUM_FLOW_PARTITIONS  /**< Number of possible flow divisions */
};

class cmpGasDistribution
{
public:
    cmpGasDistribution();

    const QList<float> &getGasRight() const;
    void setGasRight(const QList<float> &newGasRight);

    const QList<float> &getGasLeft() const;
    void setGasLeft(const QList<float> &newGasLeft);

    const QList<float> &getFlow() const;
    void setFlow(const QList<float> &newFlow);

    const QList<float> &getFlow_fraction() const;
    void setFlow_fraction(const QList<float> &newFlow_fraction);

    FlowLocation getFraction_side() const;
    void setFraction_side(FlowLocation newFraction_side);

private:
    QList<float> gasRight;//[DAM_SLICES_IN_SEASON];  /* size DAM_SLICES_IN_SEASON */
    QList<float> gasLeft;
    QList<float> flow;			/* size DAYS_IN_SEASON [DAYS_IN_SEASON]*/
    QList<float> flow_fraction;/*[DAM_SLICES_IN_SEASON];		 fraction of flow on one side,
                       size DAM_SLICES_IN_SEASON */
    FlowLocation fraction_side;		/* which side the fraction refers to */

};

#endif // CMPGASDISTRIBUTION_H
