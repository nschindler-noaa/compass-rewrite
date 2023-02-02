#ifndef CMPDAMSPECIES_H
#define CMPDAMSPECIES_H

#include "cmpequation.h"

#include <QStringList>

/** \class cmpDamSpecies
 * \brief Species information at a specific dam */
class cmpDamSpecies
{
public:
    cmpDamSpecies();
    cmpDamSpecies(QString &newname);
    cmpDamSpecies(cmpDamSpecies &rhs);

    void reset();
    cmpDamSpecies &copy(cmpDamSpecies &rhs);
    bool operator ==(cmpDamSpecies &rhs);

    float getSpillMort() const;
    void setSpillMort(float newSpillMort);

    float getBypassMort() const;
    void setBypassMort(float newBypassMort);

    float getSluicewayMort() const;
    void setSluicewayMort(float newSluicewayMort);

    float getTurbineMort() const;
    void setTurbineMort(float newTurbineMort);

    float getTransportMort() const;
    void setTransportMort(float newTransportMort);

    float getRswMort() const;
    void setRswMort(float newRswMort);

    float getMeanForebayTransitTime() const;
    void setMeanForebayTransitTime(float newMeanForebayTransitTime);

    float getSeparationProb() const;
    void setSeparationProb(float newSeparationProb);

    const cmpEquation &getSpillEqn() const;
    void setSpillEqn(const cmpEquation &newSpillEqn);

    const cmpEquation &getFgeEqn() const;
    void setFgeEqn(const cmpEquation &newFgeEqn);

    const cmpEquation &getTransEqn() const;
    void setTransEqn(const cmpEquation &newTransEqn);

    const cmpEquation &getDelayEqn() const;
    void setDelayEqn(const cmpEquation &newDelayEqn);

    const cmpEquation &getRswEqn() const;
    void setRswEqn(const cmpEquation &newRswEqn);

    float getSluicewayProp() const;
    void setSluicewayProp(float newSluicewayProp);

    float getDayFge() const;
    void setDayFge(float newDayFge);

    float getNightFge() const;
    void setNightFge(float newNightFge);

    float getSpillDelay() const;
    void setSpillDelay(float newSpillDelay);

    float getBypassDelay() const;
    void setBypassDelay(float newBypassDelay);

    float getSluicewayDelay() const;
    void setSluicewayDelay(float newSluicewayDelay);

    float getTurbineDelay() const;
    void setTurbineDelay(float newTurbineDelay);

    float getRswDelay() const;
    void setRswDelay(float newRswDelay);

    float getSpillDayDelay() const;
    void setSpillDayDelay(float newSpillDayDelay);

    float getBypassDayDelay() const;
    void setBypassDayDelay(float newBypassDayDelay);

    float getSluicewayDayDelay() const;
    void setSluicewayDayDelay(float newSluicewayDayDelay);

    float getTurbineDayDelay() const;
    void setTurbineDayDelay(float newTurbineDayDelay);

    float getRswDayDelay() const;
    void setRswDayDelay(float newRswDayDelay);

    const QString &getName() const;
    void setName(const QString &newName);

    float getRswCapacity() const;
    void setRswCapacity(float newRswCapacity);

protected:
    QString name;
    float sluicewayMort; /**< Mortality in the sluiceway */
    float bypassMort;    /**< Mortality in the bypass */
    float turbineMort;   /**< Mortality in the turbines */
    float spillMort;     /**< Mortality in conventional spillways */
    float rswMort;       /**< Mortality in the RSWs */
    float transportMort; /**< Mortality in transport */

    float meanForebayTransitTime; /**< In hours, for forebay pred mort */
    float separationProb;/**< Chance of non-collection during a transport
                            * operation */
    /** Proportion of non-spilled fish that pass through SBC or sluicway */
    float sluicewayProp;
    float rswCapacity;
//	float pred[DAM_TIME_SLC];/* Predation given at each dam time slice */

//	float predPopMean;  /* Predation population in the forebay */
//	float predPopTailrace;/* Predation population in the tailrace */

    float dayFge;        /**< Day time FGE */
    float nightFge;      /**< Night time FGE */

    /* Extra delay for passage routes */
    float sluicewayDelay;/**< Extra delay for fish entering sluiceway */
    float bypassDelay;   /**< Extra delay for fish entering bypass system */
    float turbineDelay;  /**< Extra delay for fish entering turbines */
    float spillDelay;    /**< Extra delay for fish entering conventional spillways */
    float rswDelay;      /**< Extra delay for fish entering RSW */
    /* Probability that a fish arriving during the day will pass at night */
    float sluicewayDayDelay;/**< Extra delay for fish arriving during the day
                            * when entering sluiceway*/
    float bypassDayDelay;/**< Extra delay for fish arriving during the day when
                            * entering bypass system*/
    float turbineDayDelay;/**< Extra delay for fish arriving during the day
                            * when entering turbines*/
    float spillDayDelay; /**< Extra delay for fish arriving during the day when
                            * entering conventional spillways*/
    float rswDayDelay;   /**< Extra delay for fish arriving during the day when
                            * entering RSW */

    cmpEquation fgeEqn;  /**< Fish guidance efficiency for determining the
                            * proportion of non-spilled fish routed through the bypass system */
    cmpEquation spillEqn;/**< Spill equation to determine the proportion of
                            * fish routed to spillways (both conventional and RSW) */
    cmpEquation transEqn;/**< Transport mortality equation to estimate
                            * transport mortality based on water particle travel time */

    /** Delay equation that determines delay of fish at a dam depending on
     * species, time of day, season, and flow (relative to hydraulic
     * capacity). Yield a passage probability. Note that this has been mostly
     * superseded by the newer dam delay model that allows for simulating diel
     * passage. See \ref new_dam_delay. */
    cmpEquation delayEqn;
    cmpEquation rswEqn;  /**< RSW efficiency which determines the proportion of
                            * fish that go through RSW */

};

#endif // CMPDAMSPECIES_H
