#ifndef CMPRSW_H
#define CMPRSW_H

#include "cmpequation.h"

#include <QStringList>

/** \class cmpRSW
 * \brief A structure to represent a removable spill weir associated with a dam.
 *
 */

class cmpRSW
{
public:
    cmpRSW();

    void allocate(int days);
    float getMaxSpill() const;
    void setMaxSpill(float newMaxSpill);

    float getMortality() const;
    void setMortality(float newMortality);

    cmpEquation *getEfficiency() const;
    void setEfficiency(cmpEquation *newEfficiency);

    float getDelay() const;
    void setDelay(float newDelay);

    float getDelayDay() const;
    void setDelayDay(float newDelayDay);

    bool getActive(int index) const;
    void setActive(int index, const bool &newActive);

private:
    float maxSpill;  /**< Maximum flow (partitioned from spill)
                      * through RSW (setting this to 0 essentially
                      * turns off RSWs), */
    float mortality; /**< Mortality in the RSWs, */
    cmpEquation *efficiency;/**< RSW efficiency which determines the
                      * proportion of fish that go through RSW, */
    float delay;     /**< Extra delay for fish entering RSW, */
    float delayDay;  /**< Extra delay for fish arriving during the day
                      * when entering RSW, */
    QList<bool> active; /**< Whether the RSW is active during a
                      * particular day. (Days in season)*/
};

#endif // CMPRSW_H
