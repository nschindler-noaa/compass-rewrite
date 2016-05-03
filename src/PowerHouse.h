#ifndef C_POWERHOUSE_H
#define C_POWERHOUSE_H

#include "Period.h"
#include "definitions.h"

class PowerHouse
{
public:
    PowerHouse(int num = 0);
    int number;
    int priority;    /**< Ranking of each phouse. The best priority is
                      *   0 followed by increasing positive numbers. */
    float threshold; /**< Min flow controls when this powerhouse turns on */
    float capacity;  /**< Capacity in KCFS of this powerhouse. */

    PeriodList<bool> *schedule; /**< Powerhouse operation schedule
                      * (in case some powerhouses are only available at
                      * certain times). This is a two-tier period list. */

    /** The threshold and schedule are used at run time to determine
     * the actual percentage flow through this powerhouse, tracked
     * here as flow_fraction */
    float flow_fraction[DAM_SLICES_IN_SEASON];

};

#endif // C_POWERHOUSE_H
