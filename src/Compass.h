#ifndef COMPASS_H
#define COMPASS_H

#include <QObject>
#include <QStringList>

#include "definitions.h"

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

class Compass : public QObject
{
    Q_OBJECT
public:
    explicit Compass(QObject *parent = 0);
    ~Compass ();

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

    /** List of reach classes */
    QStringList reach_class_names;

    /** List of release sites */
    QList<struct release_site *> release_sites;

    /** List of tributaries */
    QList<struct tributary *> tributaries;

    /** River mouth, the root of the river segment tree. */
    struct river_segment *river_mouth;

    /** Lattitude map limits and range */
    float lat_min, lat_max, lat_range; /* map limits */
    /** Longitude map limits and range */
    float lon_min, lon_max, lon_range;

    /** Number of species */
    int num_specs;
    /** Species structures */
    QList<struct species *> specs;
    /** Species names */
    QStringList spec_names;
    /** List of fish stocks */
    struct stock_info stocks;

    /** Pointers to current run struct, comparison run struct, and total run structs  */
    struct run *current, *comparison, runs[NRUNS];

    /** If set, this flag prevents output settings from being modified by the
     *  input data file parser (only). */
    int lock_output_settings;
    /** If set, this flag prevents variance suppression from being modified by the
     *  input data file parser (only). */
    int disable_variance_suppression;


    /** Dynamically allocated array of pointers to all dams (but not extra
     *  powerhouses) */
    QList<struct river_segment *> dams;
    /** Dynamically allocated array of pointers to all dams and powerhouses */
    QList<struct river_segment *> phouses;

    /** Number of dams and powerhouses as calculated at run time*/
    int num_dams,
               num_powerhouses;

    /** Default list of segments for a variety of purposes */
    QList <struct river_segment *> default_segment_list;
    //extern LIST *default_segment_list;

    /** Program version retrieval routine (but you should probably use version.h)*/

    QString get_program_version();

    /** Array of flags which turn individual types of mortality on and off.
     *  These are set from run.mortality_class during simulation_init()
     */
    #define N_MORTALITY_TYPE 3
    /** Turn on and off mortality types */
    int mortality_type[N_MORTALITY_TYPE];
    #define GAS_MORT 0
    #define PRED_MORT 1
    #define DO_GAS_MORT (mortality_type[GAS_MORT])
    #define DO_PRED_MORT (mortality_type[PRED_MORT])

    /** \section reach_classes Reach classes
     *
     *  Each reach has a
     *  small, non-negative number defining its class. Here we give the number
     *  of allowed classes and the name of each. There is an absolute limit of
     *  10 reach classes, which we expect will never be reached in practice.
     *
     * These classes are used to define parameters that are the same for a given
     * reach/species/stock but which vary depending on how far down in the river
     * it is (so the river is split up into several different reach classes).
     */
    /** Number of reach classes */
    int num_reach_classes;
    /** Reach class names (e.g. "Class_0") */
    QStringList reach_class_names;

    /* New support for a simple notes section */
    /** Maximum length of the notes section */
    #define	MAX_NOTES_SIZE	16384
    /*extern char *input_notes;    *< Notes on the scenario file */
    /*extern char *file_creation_date; *< File's creation date */
    /*extern char *file_creator;   /**< File's creator */
    /*extern char *file_modification_date; /**< Modification date */
    /*extern char *file_modifier;  /**< The person that modified the file last */

    /* Default input values. These disable as many features as possible
     * and when an input file is written out, only values that differ from
     * these defaults are written out. Thus, old parameters that are
     * generally disabled are hidden. */

    struct species species_defaults;   /**< Species default values */
    struct headwater_info headwater_defaults; /**< Headwater default values */
    struct release release_defaults; /**< Release default values */
    struct reach_species reach_species_defaults;/**< Reach species default values*/
    struct reach_info reach_defaults; /**< Reach default values */
    struct dam_species dam_species_defaults;	/**< Dam species default values */
    struct dam_info dam_defaults; /**< Dam default values */

    /** This is a segment that is the default transport target for the
     * I/O interface. Note that this is currently also where the
     * delayed mortality calculations are made. */

    struct river_segment *default_transport_target;

#ifdef WIN32
    /** Location in the Windows registry for the model's subkey
     * (currently only used for finding the installation path). */
#define	WINDOWS_REGISTRY_SUBKEY  "Software\\COMPASS"
#endif
    
signals:
    
public slots:
    
};




#endif // COMPASS_H
