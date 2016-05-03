#ifndef COMPASS_H
#define COMPASS_H

#include "definitions.h"
#include <QStringList>

/** Average system-wide water particle travel time for transport mortality */
struct water_travel
{
        struct river_segment *upper;  /**< Upper seg for computation */
        struct river_segment *lower;  /**< Lower seg for computation */
        int first_day;                /**< First day for computation */
        int last_day;		      /**< Last day for computation  */

        float time;                   /**< Computed water travel time */
};
typedef struct water_travel WaterTravel;

enum MortalityClass
{
    GasAndPredation = 0,  /**< Gas and predation survival model */
    XTSurvival,           /**< XT reservoir survival model */
    Custom,               /**< Custom reservoir survival model (also uses distance and time, see custom_survival.h) */
    NumMortalityClasses   /**< Valid number of mortality classes. The run struct mortality_class must
                            *  be in range [0..NumMortalityClasses-1] */
};
/** Names of mortality classes for use as input tokens */
extern QStringList mortality_class_names;

/** Performing XT calculations */
#define DO_XT        (current->mortality_class == XTSurvival)
/** Performing custom calculations */
#define DO_CUSTOM    (current->mortality_class == Custom)

/** Post-Bonneville hypotheses/algorithms */
enum PostBonHypothesis
{
        SARvsDate,                /**< SAR vs Date */
        LatentMortality,          /**< Latent Mortality */
        ConstantDelayedMortality, /**< Constant Delayed Mortality*/
        S3vsWaterTravelTime,      /**< S3 vs Water Travel Time */
        NumPostBonHypotheses      /**< The number of hypotheses */
};
/** The input tokens for post-Bonneville hypothesis selection. */
extern QStringList post_bonneville_hypothesis_names;
/** The display hypothesis names. They may contain spaces and other characters
 *  not allowed in input tokens. */
extern QStringList post_bonneville_hypothesis_names_formatted;


/** Entire run parameters */
struct run
{
    QList <struct release *> release_list; /**< Releases */

    int   iterations;             /**< Monte carlo iterations */
#ifdef GAS_CALC
    float gas_dissp_exp;	  /**< z */
#endif
    float flow_proportion;        /**< For unregulated headwater flow modulation */
    float Ufree;		  /**< Free flowing velocity constant (kfs) */
    float fork_threshold;         /**< Proportion of total flow a fork needs to be
                                   *  "significant" - range [0.0-0.4] */
#ifdef PRED_CALC
    float prey_energy_density;    /**< Value == 0 => no growth calculated */
#endif
#ifdef GROWTH_CALC
    float length_weight_b0;       /**< Also used in growth calculations. */
    float length_weight_b1;       /**< Also used in growth calculations. */
#endif
    float min_migr_rate;	  /**<  Minimum migration rate for the entire run */

    struct  river_segment *flow_seg;/**< Segment with indicative flow	 */
#ifdef REALTIME
    QString  rt_report_date;      /**< Realtime report date */
#endif

    WaterTravel water_travel;

    unsigned int flags;		  /**< Run time flags */
    MortalityClass mortality_class; /**< Which mortalities are calculated -
                                   *  only a few combinations are allowed. */
                                  /** mortality_class:
                                   * - GAS_AND_PREDATION_CLASS = gas & predation
                                   * - XT_CLASS = XT reservoir survival model
                                   * - CUSTOM_CLASS = Custom reservoir survival model */

    PostBonHypothesis post_bonneville_hypothesis; /**< Which hypothesis should be used for
                                     * post-Bonneville calculations \ref PostBonHypothesis */
};




#endif // COMPASS_H
