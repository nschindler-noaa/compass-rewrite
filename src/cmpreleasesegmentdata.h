#ifndef CMPRELEASESEGMENTDATA_H
#define CMPRELEASESEGMENTDATA_H

#include "cmpriversegment.h"

#include <QStringList>

/** Statistics for a single water path for the complete season. */
class cmpPassageStats
{
public:
    cmpPassageStats ();
    ~cmpPassageStats ();

    int getFirstDay() const;
    void setFirstDay(int value);

    int getLastDay() const;
    void setLastDay(int value);

    float getTotFishIn() const;
    void setTotFishIn(float value);

    float getTotFishOut() const;
    void setTotFishOut(float value);

    float getMean() const;
    void setMean(float value);

    float getMedian() const;
    void setMedian(float value);

    float getMode() const;
    void setMode(float value);

    float getStdDev() const;
    void setStdDev(float value);

    void computeStats(QList<float> &dailyNum);

private:
    int  firstDay;        /**< First day of passage */
    int  lastDay;         /**< Last day of passage */
    float totFishIn;      /**< Total fish into segment */
    float totFishOut;     /**< Total fish passage out (premort) */
    float mean;           /**< Mean passage */
    float median;         /**< Median passage */
    float mode;           /**< Mode passage */
    float stdDev;         /**< Standard deviation of passage */
};

/** Release summary statistics structure. These are statistics for the entire
 *  release at a segment */
class cmpReleaseSegmentStats
{
public:
    cmpReleaseSegmentStats();
//    ~cmpReleaseSegmentStats();

    bool getDone() const;
    void setDone(bool newDone);

    const cmpPassageStats &getTotalStats() const;

    const cmpPassageStats &getInriverStats() const;

    const cmpPassageStats &getTransport() const;

    float getTurbineInTotal() const;
    void setTurbineInTotal(float newTurbineInTotal);

    float getTurbineOutTotal() const;
    void setTurbineOutTotal(float newTurbineOutTotal);

    float getSluicewayInTotal() const;
    void setSluicewayInTotal(float newSluicewayInTotal);

    float getSluicewayOutTotal() const;
    void setSluicewayOutTotal(float newSluicewayOutTotal);

    float getBypassInTotal() const;
    void setBypassInTotal(float newBypassInTotal);

    float getBypassOutTotal() const;
    void setBypassOutTotal(float newBypassOutTotal);

    float getSpillwayInTotal() const;
    void setSpillwayInTotal(float newSpillwayInTotal);

    float getSpillwayOutTotal() const;
    void setSpillwayOutTotal(float newSpillwayOutTotal);

    float getRswInTotal() const;
    void setRswInTotal(float newRswInTotal);

    float getRswOutTotal() const;
    void setRswOutTotal(float newRswOutTotal);

    float getPostmortTransportTotal() const;
    void setPostmortTransportTotal(float newPostmortTransportTotal);

    float getSpillEfficiency() const;
    void setSpillEfficiency(float newSpillEfficiency);

    struct delayed_mortality_table *getD() const;
    void setD(struct delayed_mortality_table *newD);

    float getInriverLatentMort() const;
    void setInriverLatentMort(float newInriverLatentMort);

    float getTransportLatentMort() const;
    void setTransportLatentMort(float newTransportLatentMort);

    float getDifferentialReturn() const;
    void setDifferentialReturn(float newDifferentialReturn);

    float getInriverReturn() const;
    void setInriverReturn(float newInriverReturn);

    float getTransportReturn() const;
    void setTransportReturn(float newTransportReturn);

    float getOverallReturn() const;
    void setOverallReturn(float newOverallReturn);

    float getPostBonnevilleTotal() const;
    void setPostBonnevilleTotal(float newPostBonnevilleTotal);

    float getNightPassage() const;
    void setNightPassage(float newNightPassage);

    float getFlowAverage() const;
    void setFlowAverage(float newFlowAverage);

private:
    /* summary statistics for a seg */
    bool done;                /**< Compute done flag */

    /** Passage statistics (separated by total, inriver, and transport) */
    cmpPassageStats totalStats,/**< Total stats */
        inriverStats,         /**< In-river only stats */
        transportStats;       /**< Transport stats */

    /* these vars track passage routes at dams only.  they represent
     * post-mortality passage through the various dam routes.
     */
    /* Route tracking */
    float turbineInTotal;     /**< Total number of fish entering the turbines */
    float turbineOutTotal;    /**< Total passed through turbines */
    float sluicewayInTotal;   /**< Total number of fish entering the sluiceway */
    float sluicewayOutTotal;  /**< Total passed through sluiceways */
    float bypassInTotal;      /**< Total number of fish entering the bypass system */
    float bypassOutTotal;     /**< Total passed through bypass system */
    float spillwayInTotal;    /**< Total number of fish entering the conventional
                                * spillways */
    float spillwayOutTotal;   /**< Total passed through conventional spillways */
    float rswInTotal;         /**< Total number of fish entering RSW */
    float rswOutTotal;        /**< Total passed through RSW */

    float postmortTransportTotal;/** Total that arrive at the destination
                                * (corresponds to transport_pass) */

    float spillEfficiency;    /**< Spill efficiency ((proportion of fish spilled)
                                * / (proportion of water spilled)) */

    // fish return data
    struct delayed_mortality_table *d; /**< Table for calculating delayed mortality
                                * ("D"). This is used at the default_transport_target
                                * segment */
    float inriverLatentMort;  /**< Inriver latent mortality */
    float transportLatentMort;/**< Transported latent mortality */
    float differentialReturn; /**< Differential return */
    float inriverReturn;      /**< Return rate for inriver */
    float transportReturn;    /**< Return rate for transported fish */
    float overallReturn;      /**< Overall return rate for fish (weighted average
                                *  of inriver/transported return rates) */
    float postBonnevilleTotal;/**< Fish estimated to survive (out of those
                                * that arrive at this segment, i.e. project_in_total) */

    // day/night passage
    float nightPassage;       /**< The proportion of fish that pass at night */
    float flowAverage;        /**< avg flow at indicator over run */
};

class cmpReleaseSegmentData
{
public:
    cmpReleaseSegmentData();
    ~cmpReleaseSegmentData();

    cmpRiverSegment *getRivSeg() const;
    void setRivSeg(cmpRiverSegment *newRivSeg);

    const QList<float> &getOutputTotal() const;
    void setOutputTotal(const QList<float> &newOutputTotal);

    const QList<float> &getSpillExperience() const;
    void setSpillExperience(const QList<float> &newSpillExperience);

    const QList<float> &getOutputTransport() const;
    void setOutputTransport(const QList<float> &newOutputTransport);

    const QList<float> &getDelayedSurvivalRate() const;
    void setDelayedSurvivalRate(const QList<float> &newDelayedSurvivalRate);

    const QList<float> &getToTransport() const;
    void setToTransport(const QList<float> &newToTransport);

    const QList<float> &getFromTransport() const;
    void setFromTransport(const QList<float> &newFromTransport);

    const QList<float> &getProjectIn() const;
    void setProjectIn(const QList<float> &newProjectIn);

    const QList<float> &getTurbineIn() const;
    void setTurbineIn(const QList<float> &newTurbineIn);

    const QList<float> &getTurbineOut() const;
    void setTurbineOut(const QList<float> &newTurbineOut);

    const QList<float> &getSluicewayIn() const;
    void setSluicewayIn(const QList<float> &newSluicewayIn);

    const QList<float> &getSluicewayOut() const;
    void setSluicewayOut(const QList<float> &newSluicewayOut);

    const QList<float> &getBypassIn() const;
    void setBypassIn(const QList<float> &newBypassIn);

    const QList<float> &getBypassOut() const;
    void setBypassOut(const QList<float> &newBypassOut);

    const QList<float> &getSpillwayIn() const;
    void setSpillwayIn(const QList<float> &newSpillwayIn);

    const QList<float> &getSpillwayOut() const;
    void setSpillwayOut(const QList<float> &newSpillwayOut);

    const QList<float> &getRswIn() const;
    void setRswIn(const QList<float> &newRswIn);

    const QList<float> &getRswOut() const;
    void setRswOut(const QList<float> &newRswOut);

    const QList<float> &getTransportPassed() const;
    void setTransportPassed(const QList<float> &newTransportPassed);

    const QList<float> &getTransportArrived() const;
    void setTransportArrived(const QList<float> &newTransportArrived);

    delayed_mortality_table *getD() const;
    void setD(delayed_mortality_table *newD);

    float getInriverLatentMort() const;
    void setInriverLatentMort(float newInriverLatentMort);

    float getTransportLatentMort() const;
    void setTransportLatentMort(float newTransportLatentMort);

    float getDifferentialReturn() const;
    void setDifferentialReturn(float newDifferentialReturn);

    float getInriverReturn() const;
    void setInriverReturn(float newInriverReturn);

    float getTransportReturn() const;
    void setTransportReturn(float newTransportReturn);

    float getOverallReturn() const;
    void setOverallReturn(float newOverallReturn);

    float getPostBonnevilleTotal() const;
    void setPostBonnevilleTotal(float newPostBonnevilleTotal);

    float getNopass() const;
    void setNopass(float newNopass);

    const cmpReleaseSegmentStats &getStats() const;
    void setStats(const cmpReleaseSegmentStats &newStats);

private:
    cmpRiverSegment *rivSeg;  /**< Segment that this instance points to */
    QList<float> outputTotal;            /**< Number that left by day */
    QList<float> spillExperience;   /**< Mean spill fraction experienced by fish that
                                * left that day, used for the Z15 model. Size
                                * is STEPS_IN_RUN */
    QList<float> outputTransport;   /**< The number of fish that pass which were
                                * transported in (this is only used at a
                                * transport destination).
                                * This is needed so that statistics (like mean,
                                * median, mode, etc.) can be calculated
                                * separately for inriver vs. transported fish*/
    QList<float> delayedSurvivalRate;/**< Rate of survival for fish that are delayed
                                * while passing through a downstream dam.
                                * Note that this is only calculated in segments
                                * that are directly upstream from a dam and the
                                * size is STEPS_IN_RUN */

    QList<float> toTransport;       /**< Number transported out (dam only)*/
    QList<float> fromTransport;     /**< Number transported in (reach only)*/
    QList<float> projectIn;        /**< After forebay pred; used for FPE */

    /* these vars track passage routes at dams only.  they represent
     * post-mortality passage through the various dam routes.
     */
    QList<float> turbineIn;        /**< Entering turbine; also for FPE */
    QList<float> turbineOut;      /**< Number surviving turbine */
    QList<float> sluicewayIn;      /**< Entering bypass */
    QList<float> sluicewayOut;    /**< Number surviving sluiceway */
    QList<float> bypassIn;         /**< Entering bypass */
    QList<float> bypassOut;       /**< Number surviving bypass */
    QList<float> spillwayIn;       /**< Entering conventional spillway */
    QList<float> spillwayOut;     /**< Number surviving conventional spillway */
    QList<float> rswIn;            /**< Entering RSW */
    QList<float> rswOut;          /**< Number surviving RSW */

    QList<float> transportPassed;    /**< Number surviving transport index by date
                                * of start of transport from collector dam */
    QList<float> transportArrived; /**< Number surviving transport indexed by date
                                * of arrival at the destination */

    // fish return data
    struct delayed_mortality_table *d;/**< This gets copied into stats.d when
                                * computing statistics but should be freed only
                                * from here (the pointer in stats gets NULLed
                                * when computing statistics) */

    float inriverLatentMort;  /**< Inriver latent mortality. Copied into stats. */
    float transportLatentMort;/**< Transported latent mortality. Copied into stats. */
    float differentialReturn; /**< Differential return. Copied into stats. */
    float inriverReturn;      /**< Return rate for inriver. Copied into stats. */
    float transportReturn;    /**< Return rate for transported fish. Copied into stats. */
    float overallReturn;      /**< Overall return rate for fish (weighted average
                                *  of inriver/transported return rates). Copied into stats. */
    float postBonnevilleTotal;/**< Fish estimated to survive (out of those
                                * that arrive at this segment, i.e. project_in_total). Copied into stats. */

    float nopass;             /**< Censored fish (fish that don't leave the model
                                * before simulation is complete) */
    cmpReleaseSegmentStats stats;/**< Summary statistics for this seg */
};

#endif // CMPRELEASESEGMENTDATA_H
