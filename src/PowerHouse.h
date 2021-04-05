#ifndef C_POWERHOUSE_H
#define C_POWERHOUSE_H

#include "Period.h"
#include "definitions.h"

class PowerHouse
{
public:
    PowerHouse(int num = 0);
    ~PowerHouse ();

    void allocate ();

    int getPriority() const;
    void setPriority(int value = 0);

    float getThreshold() const;
    void setThreshold(float value);

    float getCapacity() const;
    void setCapacity(float value);

    void setActive (int day, int st, int sp, bool active = true);
    bool getActive (int day, int slice);

    void setFlowFraction (int day, float fraction);
    float getFlowFraction (int day);

    int getNumber() const;
    void setNumber(int value);

private:
    int number;
    int priority;    /**< Ranking of each phouse. The best priority is
                      *   0 followed by increasing positive numbers. */
    float threshold; /**< Min flow controls when this powerhouse turns on */
    float capacity;  /**< Capacity in KCFS of this powerhouse. */

    BoolPeriodList schedule; /**< Powerhouse operation schedule
                      * (in case some powerhouses are only available at
                      * certain times). This is a two-tier period list. */

    /** The threshold and schedule are used at run time to determine
     * the actual percentage flow through this powerhouse, tracked
     * here as flow_fraction */
    QList<float> flowFraction;
};

#endif // C_POWERHOUSE_H
