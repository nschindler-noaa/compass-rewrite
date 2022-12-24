#ifndef CMPDATASETTINGS_H
#define CMPDATASETTINGS_H

//#include "cmpequation.h"

#include <QStringList>

class cmpEquation;

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

enum ReturnHypothesis {
    SARvsDate,       /**< */
    S3vsWTT,         /**< This relates S3 (3rd year return rate) to water particle travel time (WTT).*/
    LatentMort,      /**< Latent mortality. */
    ConstantD,       /**< */
    numReturnHypotheses
};

/** \class cmpDataSettings \brief A class that holds various data that are per run.
 *  These settings are mostly derived from the input data files. */
class cmpDataSettings
{
public:
    cmpDataSettings();
    ~cmpDataSettings();

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

    bool getMigration() const;
    void setMigration(QString name);
    void setMigration(bool newMigration);

    bool getInterrupt() const;
    void setInterrupt(bool newInterrupt);

    bool getCompute_flow_break() const;
    void setCompute_flow_break(bool newCompute_flow_break);

    bool getSummary() const;
    void setSummary(bool newSummary);

    float getGasDisspExp() const;
    void setGasDisspExp(float newGasDisspExp);

    float getHwFlowProp() const;
    void setHwFlowProp(float newHwFlowProp);

    cmpEquation *getFreeFlowEqn() const;
    void setFreeFlowEqn(cmpEquation *newFreeFlowEqn);

    float getFreeFlowMax() const;
    void setFreeFlowMax(float newFreeFlowMax);

    float getPreyEnergyDensity() const;
    void setPreyEnergyDensity(float newPreyEnergyDensity);

    float getLengthWeightB0() const;
    void setLengthWeightB0(float newLengthWeightB0);

    float getLengthWeightB1() const;
    void setLengthWeightB1(float newLengthWeightB1);

    float getForkThreshold() const;
    void setForkThreshold(float newForkThreshold);

    const QString &getWaterTravelUpperSegment() const;
    void setWaterTravelUpperSegment(const QString &newWaterTravelUpperSegment);

    const QString &getWaterTravelLowerSegment() const;
    void setWaterTravelLowerSegment(const QString &newWaterTravelLowerSegment);

    int getWaterTravelFirstDay() const;
    void setWaterTravelFirstDay(int newWaterTravelFirstDay);

    int getWaterTravelLastDay() const;
    void setWaterTravelLastDay(int newWaterTravelLastDay);

    int getWaterTravelTime() const;
    void setWaterTravelTime(int newWaterTravelTime);

    float getMigrationRateMin() const;
    void setMigrationRateMin(float newMigrationRateMin);

    bool getSuppressVariation() const;
    void setSuppressVariation(bool newSuppressVariation);

    bool getPredVolInteraction() const;
    void setPredVolInteraction(bool newPredVolInteraction);

    bool getAgeDependentFge() const;
    void setAgeDependentFge(bool newAgeDependentFge);

    bool getTruncateTravelVect() const;
    void setTruncateTravelVect(bool newTruncateTravelVect);

    bool getTruncateResSurvival() const;
    void setTruncateResSurvival(bool newTruncateResSurvival);

    bool getComputeMuMethod() const;
    void setComputeMuMethod(bool newComputeMuMethod);

    int getYearTraveltimeIndicator() const;
    void setYearTraveltimeIndicator(int newYearTraveltimeIndicator);

    int getYearTraveltimeOffset() const;
    void setYearTraveltimeOffset(int newYearTraveltimeOffset);

    QString &getMortClassString();
    void setMortClass(QString &mortclass);
    MortalityClass getMortClass() const;
    void setMortClass(MortalityClass newMortClass);

    ReturnHypothesis getFishReturnHyp() const;
    QString getFishReturnHypStr() const;
    void setFishReturnHyp(ReturnHypothesis newFishReturnHyp);
    void setFishReturnHyp(QString name);

private:
    /** Downstream (juvenile) or upstream (adult) model */
    bool migration;          /**< Type of migration JUVENILE or Downstream (true), or ADULT or Upstream (false). JUVENILE is the default. */
    bool interrupt;          /**< Interrupt current run? */

    bool compute_flow_break; /**< Cancel compute_all_flow? (bool), default false or go ahead and compute */
    bool summary;            /**< In batch monte mode, should we write summary.altn? (bool)*/
//    struct monte_carlo_data *monte_data; /**< A pointer used in monte carlo mode */
    int  iterations;         /**< The number of times to run the monte carlo simulation */
    bool tt_calib;           /**< Calculate the travel table each time? (bool) */
    bool surv_calib;         /**< Calculate and store survival probs for each step at each reach? (bool) */
    bool consolidate_on_stock; /**< Consolidate releases on stock rather than species? - used in Monte Carlo Covariance */

    int numDaysInSeason;     /**< Number of days in the season */
    int numDaysInRun;        /**< Number of days in the run. This is usually the same as number
                              *  of days in the season, but can be shortened to 153 for speed for JUVENILE migration.*/
    int timeStepsPerDay;     /**< Number of time-steps per day (reaches), must be power of 2 and less or equal to dam-time-slices. */
    int timeStepsPerSeason;   /**< Number of time-steps per season */
    int timeStepsPerRun;     /**< Number of time-steps per run */
    int seasonStartStep;     /**< The step of the first day of the season */
    int damSlicesPerDay;     /**< Number of dam time-slices per day, must be power of 2 and greater or equal to time-steps. */
    int damSlicesPerStep;    /**< Number of dam time-slices per step */
    int damSlicesPerSeason;   /**< Number of dam time-slices per season */
    int damSlicesPerRun;     /**< Number of dam time-slices per run */
    int gasStepsPerDay;      /**< Number of gas steps per day (if different from dam_slices per day) */
    int spillStepsPerDay;    /**< Number of spill steps per day - if used, it is greater than 0 and usually equal to dam_slices_per_day */

    int dayStart;            /**< Time at which night ends for is_night() default (600 = 6 AM) */
    int nightStart;          /**< Time at which night begins for is_night() default (2000 = 8 PM) */

    bool realTime;           /**< To produce realtime output for more analysis */
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

    float gasDisspExp;      /**< gas dissipation default for all reaches. Default is 0.20 */
    float hwFlowProp;  /**< Default is 1.00 */

    cmpEquation *freeFlowEqn;    /**< Default free flowing segment velocity equation (ft/sec) */

    float freeFlowMax;        /**< maximum value for free flowing velocity. Default is 8.000000 */
    float preyEnergyDensity;  /**< Used in growth calculations. Default is 5400.000000 */
    float lengthWeightB0;     /**< Used in length to weight conversions. Default is -11.580000 */
    float lengthWeightB1;     /**< Used in length to weight conversions. Default is 3.033000 */
    float forkThreshold;     /**< Proportion of total flow a fork needs to be
                               *  "significant" - range [0.0-0.4]. Default is 0.100000 */
    QString waterTravelUpperSegment;  /**< Upper seg for computation. Default is Little_Goose_Pool */
    QString waterTravelLowerSegment;  /**< Lower seg for computation. Default is Estuary */
    int waterTravelFirstDay;  /**< First day for computation. Default is 1 */
    int waterTravelLastDay;   /**< Last day for computation. Default is 365 */
    int waterTravelTime;      /**< Average water particle travel time */

    float migrationRateMin;   /**< Minimum migration rate in mi/day for the entire run. Default is 1.000000 */
    bool suppressVariation;   /**< Variation suppression to speed up calcs, default is true */
    bool predVolInteraction;  /**< Predation and volume interaction, default is false */
    bool ageDependentFge;     /**< Age dependent Fish Guidance Efficiency, defaul is false */
    bool truncateTravelVect;  /**< Truncate travel vector to speed up calcs, default is true */
    bool truncateResSurvival; /**< Truncate reservoir survival to speed up calcs, default is true */
    bool computeMuMethod;   /**< Whether to compute mu using down stream dam (true) or up stream dam (false).
                               *   It defaults to false for backward compatibility */
    int yearTraveltimeIndicator; // 0
    int yearTraveltimeOffset; /**< Whether to use a parameter for eqns 52 and 59 for a tt offset */
    MortalityClass mortClass; /**< Which mortalities are calculated - only a few combinations are allowed:
                               *   GasAndPredation, XT, and Custom. */
    QString mortClassString; /**< string representation of mort class */
    ReturnHypothesis fishReturnHyp; /**< Which hypothesis should be used for
                               *   fish return calculations \ref OceanHypothesis */

    bool debug;       /**< Flag to turn on debugging messages. */
    bool debugInput;  /**< Debug data input */
    bool debugCalib;  /**< Debug calibration calculations (produce csv file) */

};


#endif // CMPDATASETTINGS_H
