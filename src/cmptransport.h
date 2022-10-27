#ifndef CMPTRANSPORT_H
#define CMPTRANSPORT_H

//#include "cmpriversegment.h"
#include "cmpspecies.h"

#include <QStringList>

class cmpRiverSegment;
class cmpSpecies;

/** \class cmpTransport
 * \brief A structure to represent transporting fish along a river.
 *   These are usually used for transporting fish around dam systems.
 */

class cmpTransport
{
public:
    cmpTransport();
    ~cmpTransport ();

    void allocate(int days);

    cmpRiverSegment *getSource() const;
    void setSource(cmpRiverSegment *newSource);

    cmpRiverSegment *getTarget() const;
    void setTarget(cmpRiverSegment *newTarget);

    float getRate() const;
    void setRate(float newRate);

    bool getByDate() const;
    void setByDate(bool newByDate);

    int getStartDate() const;
    void setStartDate(int newStartDate);

    const QList<bool> &getActive() const;
    void setActive(const QList<bool> &newActive);

    int getStartCount() const;
    void setStartCount(int newStartCount);

    int getMaxRestarts() const;
    void setMaxRestarts(int newMaxRestarts);

    int getTotalRestarts() const;
    void setTotalRestarts(int newTotalRestarts);

    int getEndCount() const;
    void setEndCount(int newEndCount);

    int getLowDays() const;
    void setLowDays(int newLowDays);

    int getEndDate() const;
    void setEndDate(int newEndDate);

    float getHiFlowPassedPct() const;
    void setHiFlowPassedPct(float newHiFlowPassedPct);

    float getHighFlow() const;
    void setHighFlow(float newHighFlow);

    cmpSpecies *getHiFlowSpecies() const;
    void setHiFlowSpecies(cmpSpecies *newHiFlowSpecies);

private:
    cmpRiverSegment *source; /**< River segment from where transport removes fish. */
    cmpRiverSegment *target; /**< River segment to where transport returns fish to the river. */

    /* Transport speed */
    float rate;               /**< Rate of transport 100-500 mi per day */

    bool byDate;              /**< Start transport: true = by date,
                               *  false = by count. */

    /* Transport by date */
    int startDate;            /**< For transport start by date */
    QList<bool> active;      /**< Bool array for transport activity by
                               *   day: 1 = on, 0 = off.  */

    /* Transport by count */
    int startCount;           /**< Count at which to start transport. */
    int maxRestarts;          /**< Maximum number of restarts; -1 for infinite. */
    int totalRestarts;        /**< Number of restarts. */
    int endCount;             /**< Low count for transport end. */
    int lowDays;              /**< Number of consecutive low count
                               *   days required to end transport. */
    int endDate;              /**< Hard date for transport end. */

    /* High-flow information */
    float hiFlowPassedPct;      /**< Percentage passage of hfl_spec
                               *   for full transport. */
    float highFlow;           /**< High flow val for partial transport.*/
    cmpSpecies *hiFlowSpecies;  /**< Pointer to Species to return to river
                               *   during high flow. */

};

#endif // CMPTRANSPORT_H
