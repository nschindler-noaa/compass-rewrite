#ifndef C_TRANSPORT_H
#define C_TRANSPORT_H

#include "Species.h"
#include "RiverSegment.h"
#include "Period.h"

/** /class Transport
 *  /brief Transport operation parameters */
class Transport
{
public:
    Transport ();
    ~Transport ();

    bool byDate;              /**< Start transport: true = by date,
                               *  false = by count. */

    /* Transport by date */
    int startDate;            /**< For transport start by date */
    QList<bool> active;      /**< Bool array for transport activity by
                               *   day: 1 = on, 0 = off.  */

    /* Transport by count */
    int startCount;           /**< Count at which to start transport. */
    int maxRestarts;          /**< Maximum number of restarts; -1 for infinite. */
    int totRestarts;          /**< Number of restarts. */
    int endCount;             /**< Low count for transport end. */
    int lowDays;              /**< Number of consecutive low count
                               *   days required to end transport. */
    int endDate;              /**< Hard date for transport end. */

    /* High-flow information */
    float hiFlPassedPct;      /**< Percentage passage of hfl_spec
                               *   for full transport. */
    float highFlow;           /**< High flow val for partial transport.*/
    Species *hiFlSpecies;     /**< Pointer to Species to return to river
                               *   during high flow. */

    /* Transport speed */
    float rate;               /**< Rate of transport 100-500 mi per day */

    QString source_seg;       /**< Segment transported from */
    QString target_seg;       /**< Segment where fish rejoin river */
};



#endif // C_TRANSPORT_H
