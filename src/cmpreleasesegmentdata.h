#ifndef CMPRELEASESEGMENTDATA_H
#define CMPRELEASESEGMENTDATA_H

#include "cmpriversegment.h"

#include <QStringList>

/** Release summary statistics structure. These are statistics for the entire
 *  release at a segment */
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
    float stdDev;        /**< Standard deviation of passage */
};

class cmpReleaseStats
{
    /* summary statistics for a seg */
    bool done;                 /**< Compute done flag */

    /** Passage statistics (separated by total, inriver, and transport) */
    cmpPassageStats total, /**< Total stats */
        inriver,              /**< In-river only stats */
        transport;            /**< Transport stats */

#ifdef ROUTE_TRACKING
    /* these vars track passage routes at dams only.  they represent
     * post-mortality passage through the various dam routes.
     */
    /* Route tracking */
    float turbine_in_total;   /**< Total number of fish entering the turbines */
    float sluiceway_in_total; /**< Total number of fish entering the sluiceway */
    float bypass_in_total;    /**< Total number of fish entering the bypass system */
    float spillway_in_total;  /**< Total number of fish entering the conventional
                                * spillways */
    float rsw_in_total;       /**< Total number of fish entering RSW */

    float sluiceway_total;    /**< Total passed through sluiceways */
    float bypass_total;       /**< Total passed through bypass system */
    float turbine_total;      /**< Total passed through turbines */
    float spillway_total;     /**< Total passed through conventional spillways */
    float rsw_total;          /**< Total passed through RSW */
    float postmort_transport_total;/** Total that arrive at the destination
                                * (corresponds to transport_pass) */

    float spill_efficiency;   /**< Spill efficiency ((proportion of fish spilled)
                                * / (proportion of water spilled)) */
#endif
    struct delayed_mortality_table *d; /**< Table for calculating delayed mortality
                                * ("D"). This is used at the default_transport_target
                                * segment */
    float inriver_latent_mort;/**< Inriver latent mortality */
    float transport_latent_mort;/**< Transported latent mortality */
    float differential_return;/**< Differential return */
    float inriver_return;     /**< Return rate for inriver */
    float transport_return;   /**< Return rate for transported fish */
    float overall_return;     /**< Overall return rate for fish (weighted average
                                *  of inriver/transported return rates) */
    float post_bonneville_total;/**< Fish estimated to survive (out of those
                                    * that arrive at this segment, i.e.
                                                                * project_in_total) */

    float night_passage;      /**< The proportion of fish that pass at night */
    float avg_flow;           /**< avg flow at indicator over run */
};

class cmpReleaseSegmentData
{
public:
    cmpReleaseSegmentData();

public:
    cmpRiverSegment *rivSeg;  /**< Segment that this instance points to */
    float *output;            /**< Number that left that day */
    float *spillExperience;         /**< Mean spill fraction experienced by fish that
                                * left that day, used for the Z15 model. Size
                                * is STEPS_IN_RUN */
    float *outputTransport;  /**< The number of fish that pass which were
                                * transported in (this is only used at a
                                * transport destination).
                                * This is needed so that statistics (like mean,
                                * median, mode, etc.) can be calculated
                                * separately for inriver vs. transported fish*/
    float *delayed_survival_rate;/**< Rate of survival for fish that are delayed
                                * while passing through a downstream dam.
                                * Note that this is only calculated in segments
                                * that are directly upstream from a dam and the
                                * size is STEPS_IN_RUN */

    float *toTransport;     /**< Number transported out (dam only)*/
    float *fromTransport;      /**< Number transported in (reach only)*/
    float *project_in;        /**< After forebay pred; used for FPE */
#ifdef ROUTE_TRACKING
    /* these vars track passage routes at dams only.  they represent
     * post-mortality passage through the various dam routes.
     */
    float *turbine_in;        /**< Entering turbine; also for FPE */
    float *sluiceway_in;      /**< Entering bypass */
    float *bypass_in;         /**< Entering bypass */
    float *spillway_in;       /**< Entering conventional spillway */
    float *rsw_in;            /**< Entering RSW */

    float *sluiceway_pass;    /**< Number surviving sluiceway */
    float *bypass_pass;       /**< Number surviving bypass */
    float *turbine_pass;      /**< Number surviving turbine */
    float *spillway_pass;     /**< Number surviving conventional spillway */
    float *rsw_pass;          /**< Number surviving RSW */
    float *transport_pass;    /**< Number surviving transport index by date
                                    * of start of transport from collector dam */
    float *transport_arrived; /**< Number surviving transport indexed by date
                                    * of arrival at the destination */
#endif
    struct delayed_mortality_table *d;/**< This gets copied into stats.d when
                                * computing statistics but should be freed only
                                * from here (the pointer in stats gets NULLed
                                * when computing statistics) */

    float inriver_latent_mort;/**< Copied into stats */
    float transport_latent_mort;/**< Copied into stats */
    float differential_return;/**< Copied into stats */
    float inriver_return;     /**< Return rate for inriver (copied into stats)*/
    float transport_return;   /**< Return rate for transported fish (copied into
                                        * stats) */
    float overall_return;     /**< Copied into stats */
    float post_bonneville_total;/**< Copied into stats */


    float nopass;             /**< Censored fish (fish that don't leave the model
                            * before simulation is complete) */
    cmpReleaseStats stats;   /**< Summary statistics for this seg */
};

#endif // CMPRELEASESEGMENTDATA_H
