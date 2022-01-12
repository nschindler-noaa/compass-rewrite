#ifndef C_RELEASE_H
#define C_RELEASE_H

/** \file
 * \brief  Structures related to releases and release data with
 *  functions provided by the release module.
 *
 * Other modules should use these rather than malloc'ing releases on their own.
 */

#include "RiverPoint.h"
#include "Reach.h"
#include "Stock.h"
#include "Species.h"
#include "Realtime.h"
#include "definitions.h"
#include "Period.h"

#include <QPointF>
#include <QList>

class Reach;

/** Release site structure. It contains name, location in lat/lon,
 *  pointer to the site's segment and position along that segment */
class ReleaseSite
{
public:
    ReleaseSite (QString rsname = QString (""));
    ~ReleaseSite ();

    QString *getName() const;
    void setName(QString *value);

    RiverPoint *getLatlon() const;
    void setLatlon(RiverPoint *value);

    void setSeg(Reach *value);
    Reach *getSeg() const;

    float getSeg_mile() const;
    void setSeg_mile(float value);

private:
    QString *name;            /**< Name of this site */
    RiverPoint *latlon;       /**< Geographic location */
    Reach *seg;               /**< Pointer to the segment */
    float seg_mile;           /**< Miles from bottom of segment */
};

/** Release summary statistics structure. These are statistics for the entire
 *  release at a segment */
class PassageStats
{
public:
    PassageStats ();
    ~PassageStats ();

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

class ReleaseStats
{
    /* summary statistics for a seg */
    bool done;                 /**< Compute done flag */

    /** Passage statistics (separated by total, inriver, and transport) */
    PassageStats total, /**< Total stats */
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

class ReleaseSegment
{
public:
    RiverSegment *rivSeg;  /**< Segment that this instance points to */
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
    ReleaseStats stats;   /**< Summary statistics for this seg */
};


class Release
{
public:
    Release ();
    Release (Release &rhs);
    ~Release ();

    const QString &getName() const;
    void setName(const QString &newName);

    ReleaseSite *getSite() const;
    void setSite(ReleaseSite *newSite);

    Species *getSpecies() const;
    void setSpecies(Species *newSpecies);

    Stock *getStock() const;
    void setStock(Stock *newStock);

    int getStartDay() const;
    void setStartDay(int newStartDay);

    const QList<float> &getDayNumber();
    const float &getNumber(int day) const;
    void setNumber(int day, const float &num);

    RtInfo *getRtinfo() const;
    void setRtinfo(RtInfo *newRtinfo);

    int getAddSetting() const;
    void setAddSetting(int newAddSetting);

    int getDirtyFlag() const;
    void setDirtyFlag(int newDirtyFlag);

    float getInitialSpillExperience() const;
    void setInitialSpillExperience(float newInitialSpillExperience);

    float getTotalReleased() const;
    void setTotalReleased(float newTotalReleased);

    float getMigrOnsetMedian() const;
    void setMigrOnsetMedian(float newMigrOnsetMedian);

    void appendRelSegment(ReleaseSegment *relseg);

    ReleaseStats &getReleaseStats();

private:
    QString name;
    ReleaseSite *site;
    Species *species;
    Stock *stock;
    int startDay;        /**< Offset into season */
    QList<float> number; /**< The release distribution up to [Days_IN_SEASON] - startDay*/
#ifdef REALTIME
    RtInfo *rtinfo;               /**< Realtime info (optional) */
#endif

    void activate (bool = true);

    int   addSetting;       /**< For determining how to add the release to the
                                                           * system. */
    int   dirtyFlag;        /**< For use by GUI release editor. */

    float initialSpillExperience;/**< If the release is made in a segment
                               * that isn't immediately downstream from a dam,
                               * then the dam spill experience will be filled
                               * in with this value */
    float totalReleased;  /**< Total fish in last run */
    QList<ReleaseSegment *> relSegments; /** List of release segments (hold data and stats) */
    ReleaseStats stats; /**< Summary statistics for this release */
    float migrOnsetMedian;/**< Migration onset, median value
                                *  in days (computed) */
};
/* Various structures to handle release sites and release tracking statistics.
 */

/** Release summary statistics structure. These are statistics for the entire
 *  release at a segment */
struct passage_stats
{
        int  first;           /**< First day of passage */
        int  last;            /**< Last day of passage */
        float in;             /**< Total fish into segment */
        float out;            /**< Total fish passage out (premort) */
        float mean;           /**< Mean passage */
        float median;         /**< Median passage */
        float mode;           /**< Mode passage */
        float std_dev;        /**< Standard deviation of passage */
};

struct rls_stats
{
    /* summary statistics for a seg */
    bool done;                 /**< Compute done flag */

    /** Passage statistics (separated by total, inriver, and transport) */
    struct passage_stats total, /**< Total stats */
        inriver,              /**< In-river only stats */
        transport;            /**< Transport stats */

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

/** Structure for release statistics at a segment that are calculated and then
 *  converted into summary statistics. */
struct rls_seg_data
{
    float *output;            /**< Number that left that day */
    float *dam_spill;         /**< Mean spill fraction experienced by fish that
                                * left that day, used for the Z15 model. Size
                                * is STEPS_IN_RUN */
    float *output_transport;  /**< The number of fish that pass which were
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

    float *transport_out;     /**< Number transported out (dam only)*/
    float *transport_in;      /**< Number transported in (reach only)*/
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
    struct rls_stats stats;   /**< Summary statistics for this seg */
};

/** Release segment (where calculated data is stored for a release at a
 *  segment). */
struct rls_seg
{
    struct rls_seg *up;       /**< Next one upstream */
    struct rls_seg *down;     /**< Next one downstream */
    struct river_segment *river_segment; /** The river segment that this data
                                * corresponds to */

    struct rls_seg_data **data;/**< Dynamic array of pointers to
                                 * seg data [games], nullptr terminated */
};

/** Release parameters */
struct release
{
    struct release *next;     /**< Next release (for scanning) */
    struct release *prev;     /**< Previous release (for scanning) */

    struct rls_seg *rls_seg;  /**< List of segments, first one MUST be a reach segment */

    ReleaseSite *site;/**< Site where the fish are released */
    Species *species;  /**< The species of fish in this release */
    Stock *stock;            /**< The stock of fish that is released */

#ifdef REALTIME
    RtInfo *rt;               /**< Realtime info (optional) */
#endif

    int    add_setting;       /**< For determining how to add the release to the
                                                           * system. */
    int    dirty_flag;        /**< For use by GUI release editor. */
    int    start_date;        /**< Offset into season */
    float   initial_spill_experience;/**< If the release is made in a segment
                               * that isn't immediately downstream from a dam,
                               * then the dam spill experience will be filled
                               * in with this value */
    float   number[STEPS_IN_SEASON];/**< The release distribution */

    float   total_released;  /**< Total fish in last run */
    struct  rls_stats stats; /**< Summary statistics for this release */
    float   migr_onset_median;/**< Migration onset, median value
                                *  in days (computed) */
    char    *temp;           /**< Temporary data field */
};

struct release *new_release ();  /* allocate and initialize */
void copy_release (struct release *from, struct release *to); /* copy data */
void activate_release (struct release *r); /* insert in global list */
void delete_release (struct release *r); /* free memory and remove */
void remove_all_releases ();    /* trash everything */
struct release *find_release ();
int alloc_seg_data (struct release *release, int games);
void delete_release_sites ();


#endif // C_RELEASE_H
