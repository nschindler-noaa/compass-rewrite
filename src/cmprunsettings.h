#ifndef CMPRUNSETTINGS_H
#define CMPRUNSETTINGS_H

#include "cmpequation.h"

#include <QStringList>

/** /brief Mortality class types. Mortality class determines what calculations
 *  to perform when determining mortality. */
enum MortalityClass {
    GasAndPredation, /**< Disovled gas effects and predation model. */
    XT,              /**< Distance and time model. */
    Custom,          /**< Custom mortality model using average water temperature, average flow,
                       *  spill experienced, reach length, fish density, bird density, and
                       *  passage time in days. see custom_survival.h. */
    numMortalityClasses // /**< The number of mortality classes   */
};

/** \class cmpRunSettings \brief A class that holds various data that are per run.
 *  These settings are mostly derived from the input data files. */
class cmpRunSettings
{
public:
    cmpRunSettings();

    int getNumDaysInSeason() ;
    void setNumDaysInSeason(int newNumDays);
    int getNumDaysInRun() ;
    void setNumDaysInRun(int newNumDays);

    int getDamSlicesPerDay() ;
    int getDamSlicesPerSeason() ;
    void setDamSlicesPerDay(int newDamSlicesPerDay);

    int getTimeStepsPerDay() ;
    int getTimeStepsPerSeason() ;
    void setTimeStepsPerDay(int newTimeStepsPerDay);

    bool getRouteTracking() const;
    void setRouteTracking(bool newRouteTracking);

    bool getCalcGas() const;
    void setCalcGas(bool newCalcGas);

    bool getCalcPred() const;
    void setCalcPred(bool newCalcPred);

    bool getCalcGrowth() const;
    void setCalcGrowth(bool newCalcGrowth);

    bool getDownstream() const;
    void setDownstream(bool newDownstream);

    bool getUpstream() const;
    void setUpstream(bool newUpstream);

    bool getDebug() const {return debug;}
    bool getDebugInput() const {return debugInput;}
    bool getDebugCalib() const {return debugCalib;}
    void setDebug(bool newDebug);

    bool getCalcTurbidity() const;
    void setCalcTurbidity(bool newCalcTurbidity);

    int getDayStart() const;
    void setDayStart(int newDayStart);

    int getNightStart() const;
    void setNightStart(int newNightStart);

private:
    /** Downstream (juvenile) or upstream (adult) model */
    bool migration;          /**< Type of migration JUVENILE or Downstream (true), or ADULT or Upstream (false). JUVENILE is the default. */
    bool interrupt;          /**< Interrupt current run? */

    bool compute_flow_break; /**< Cancel compute_all_flow? (bool), default false or go ahead and compute */
    bool summary;            /**< In batch monte mode, should we write summary.altn? (bool)*/
    struct monte_carlo_data *monte_data; /**< A pointer used in monte carlo mode */
    int  iterations;         /**< The number of times to run the monte carlo simulation */
    bool tt_calib;           /**< Calculate the travel table each time? (bool) */
    bool surv_calib;         /**< Calculate and store survival probs for each step at each reach? (bool) */
    bool consolidate_on_stock; /**< Consolidate releases on stock rather than species? - used in Monte Carlo Covariance */

    int numDaysInSeason;     /**< Number of days in the season */
    int numDaysInRun;        /**< Number of days in the run. This is usually the same as number
                              *  of days in the season, but can be shortened to 153 for speed for JUVENILE migration.*/
    int timeStepsPerDay;     /**< Number of time-steps per day (reaches), must be power of 2 and less or equal to dam-time-slices. */
    int timeStepsInSeason;   /**< Number of time-steps per season */
    int timeStepsPerRun;     /**< Number of time-steps per run */
    int seasonStartStep;     /**< The step of the first day of the season */
    int damSlicesPerDay;     /**< Number of dam time-slices per day, must be power of 2 and greater or equal to time-steps. */
    int damSlicesPerStep;    /**< Number of dam time-slices per step */
    int damSlicesInSeason;   /**< Number of dam time-slices per season */
    int damSlicesPerRun;     /**< Number of dam time-slices per run */
    int gasStepsPerDay;      /**< Number of gas steps per day (if different from dam_slices per day) */
    int spillStepsPerDay;    /**< Number of spill steps per day - if used, it is greater than 0 and usually equal to dam_slices_per_day */

    int dayStart;            /**< Time at which night ends for is_night() default (600 = 6 AM) */
    int nightStart;          /**< Time at which night begins for is_night() default (2000 = 8 PM) */

    /** Track fish routing through dams - default is true for downstream,
     *  meaningless for upstream */
    bool routeTracking;

    /** Whether to compute gas propagation. (bool), defaults to true - always computed if doing GAS mortality. */
    bool calcGas;
    /** Whether to compute tubidity (if we ever get such a model). Otherwise it passes values down stream. */
    bool calcTurbidity;
    /** Whether to compute predation mortality. Requires fish density and predator density arrays. */
    bool calcPred;
    /** Whether to compute growth, if false, it passes down a default value. (for juvenile only) */
    bool calcGrowth;


    float gas_dissp_exp;      /**< gas dissipation default for all reaches */ // 0.20
    float hw_flow_prop;  // 1.00

    cmpEquation *freeFlow;    /**< Default free flowing segment velocity equation (ft/sec) */ //ufree_eqn	9
//		parameter 0  0.000000
//		parameter 1  4.500000
//		parameter 2  0.000000
//	end ufree_eqn ()

    float freeFlowMax;        /**< maximum value for free flowing velocity */ //ufree_max 8.000000
    float preyEnergyDensity;  /**< Used in growth calculations   */ // 5400.000000
    float lengthWeightB0;     /**< Used in length to weight conversions. */ // -11.580000
    float lengthWeightB1;     /**< Used in length to weight conversions. */ // 3.033000
    float fork_threshold;     /**< Proportion of total flow a fork needs to be
                               *  "significant" - range [0.0-0.4] */ // 0.100000
    QString waterTravelUpperSegment;  /**< Upper seg for computation */ // Little_Goose_Pool
    QString waterTravelLowerSegment;  /**< Lower seg for computation */ // Estuary
    int waterTravelFirstDay;    /**< First day for computation */ // 1
    int waterTravelLastDay;     /**< Last day for computation */ // 365
    int waterTravelTime;        /**< Average water particle travel time */

    float min_migration_rate; /**<  Minimum migration rate in mi/day for the entire run */ // 1.000000
    bool suppress_variation; // On
    bool pred_vol_interaction; // Off
    bool age_dependent_fge; // Off
    bool truncate_travel_vect; // On
    bool truncate_res_survival; // On
    bool compute_mu_method;   /**< Whether to compute mu using down stream dam (true) or up stream dam (false). It defaults to false for backward compatibility */
    int year_traveltime_indicator; // 0
    int year_traveltime_offset;/**< Whether to use a parameter for eqns 52 and 59 for a tt offset */
    MortalityClass mortClass; /**< Which mortalities are calculated -
                               *  only a few combinations are allowed: GasAndPredation, XT, and Custom. */
    int post_bonneville_hypothesis; /**< Which hypothesis should be used for
                               * post-Bonneville calculations \ref PostBonHypothesis */

    bool debug;
    bool debugInput;
    bool debugCalib;

};


#endif // CMPRUNSETTINGS_H
