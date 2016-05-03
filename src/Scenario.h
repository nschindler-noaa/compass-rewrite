#ifndef C_SCENARIO_H
#define C_SCENARIO_H

#include <QObject>
#include <QString>

#include "RiverSystem.h"
#include "Results.h"
#include "settings.h"

class RiverSystem;

/** Post-Project hypotheses/algorithms */
enum PostProjectHypothesis
{
    SAR_vs_Date,          /**< SAR vs date */
    Latent_Mortality,     /**< Latent mortality */
    Constant_D,           /**< Constant delayed mortality*/
    S3_vs_WTT,            /**< S3 vs water travel time */
    NumPostPrjHyp         /**< The number of hypotheses */
};
/** The actual input tokens for post-Bonneville hypothesis
 *  selection.
extern QStringList post_bonneville_hypothesis_names;//[NUM_POST_BONNEVILLE];*/
/** The display hypothesis names. They may contain spaces and other characters
 *  not allowed in input tokens.
extern QStringList post_bonneville_hypothesis_names_formatted;//[NUM_POST_BONNEVILLE];*/

enum MortalityClass
{
    Gas_and_Predation = 0,/**< Gas and predation survival model */
    XT_Survival,          /**< XT reservoir survival model */
    Custom_Survival,      /**< Custom reservoir survival model (also uses distance and time, see custom_survival.h) */
    NumMortalityClasses   /**< Valid number of mortality classes. The run struct mortality_class must
                            *  be in range [0..NumMortalityClasses-1] */
};
/** Names of mortality classes for use as input tokens */
extern QStringList mortality_class_names;

/** Performing XT calculations
#define DO_XT        (mortality_class == XTSurvival)*/
/** Performing custom calculations
#define DO_CUSTOM    (mortality_class == Custom)*/

/** Average system-wide water particle travel time for transport mort */
struct water_travel_time
{
    RiverSegment *upper;  /**< Upper seg for computation */
    RiverSegment *lower;  /**< Lower seg for computation */
    int first_day;			      /**< First day for computation */
    int last_day;			      /**< Last day for computation */

    float time;				      /**< Computed water travel time */
};
/** Entire run parameters */
class Scenario : public QObject
{
    Q_OBJECT
public:
    explicit Scenario (QObject *parent = 0);
    Scenario (Settings *set, QObject *parent = 0);
    ~Scenario ();

    bool stop;

    Settings * settings;    /**< this is just a pointer, do not delete */
    RiverSystem * river;    /**< this is where all river data resides */

    QList <struct release *> release_list; /**< Releases */

    int   iterations;             /**< Monte carlo iterations */
#ifdef GAS_CALC
    float gas_dissp_exp;	      /**< z */
#endif
    float flow_proportion;        /**< For unregulated headwater flow modulation */
    float Ufree;		          /**< Free flowing velocity constant (kfs) */
    float fork_threshold;         /**< Proportion of total flow a fork needs to be
                                   *  "significant" - range [0.0-0.4] */
#ifdef PRED_CALC
    float prey_energy_density;    /**< Value == 0 => no growth calculated */
#endif
#ifdef GROWTH_CALC
    float length_weight_b0;       /**< Also used in growth calculations. */
    float length_weight_b1;       /**< Also used in growth calculations. */
#endif
    float min_migr_rate;	      /**<  Minimum migration rate for the entire run */

    RiverSegment *flow_seg;/**< Segment with indicative flow	 */
#ifdef REALTIME
    QString  rt_report_date;      /**< Realtime report date */
#endif

    struct water_travel_time water_travel;

    unsigned int flags;		      /**< Run time flags */
    MortalityClass mortality_class; /**< Which mortalities are calculated -
                                   *  only a few combinations are allowed. */
                                  /** mortality_class:
                                   * - GAS_AND_PREDATION_CLASS = gas & predation
                                   * - XT_CLASS = XT reservoir survival model
                                   * - CUSTOM_CLASS = Custom reservoir survival model */

    PostProjectHypothesis post_proj_hypothesis; /**< Which hypothesis should be used for
                                     * post-Bonneville calculations \ref PostBonHypothesis */

    bool changeFlow;
    bool changeTemp;
    bool changeSpill;
    bool changeRelease;

    float calc_segment_WTT (RiverSegment *seg);
    float calc_WTT ();

signals:
    void start (int numyrs, int numrls, int numgms);
    void complete ();
    void cancelled ();
    void running ();
    void progress (int year, int rls, int game);

public slots:
    bool initialize ();
    bool constructRiver (Settings *settings);
    bool run (Results *result, Settings *settings);
    bool runScenario (Results *result, Settings *settings);
    bool runMonteCarlo (Results *result, Settings *settings);
    bool runMonteCarloMV (Results *result, Settings *settings);
    bool runMultiScenario (Results *result, Settings *settings);
    bool runScenarioRT (Results *result);
    bool runMonteCarloRT (Results *result, Settings *settings);
    bool runCalibration (Results *result);
    void cancel ();
};



#endif // C_SCENARIO_H
