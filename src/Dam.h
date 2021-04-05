#ifndef C_DAM_H
#define C_DAM_H

#include <QList>

#include "RiverSegment.h"
#include "PowerHouse.h"
#include "Period.h"
#include "Basin.h"
#include "Equation.h"
#include "Species.h"
#include "Transport.h"
#include "damstructures.h"

/** \struct dam_species
 * \brief Species-specific information at a dam */
struct dam_species
{
        float spill_mort;     /**< Mortality in conventional spillways */
        float bypass_mort;    /**< Mortality in the bypass */
        float sluiceway_mort; /**< Mortality in the sluiceway */
        float turbine_mort;   /**< Mortality in the turbines */
        float transport_mort; /**< Mortality in transport */
        float rsw_mort;       /**< Mortality in the RSWs */
        float mean_forebay_transit_time; /**< In hours, for forebay pred mort */
        float separation_prob;/**< Chance of non-collection during a transport
                                * operation */
//	float pred[DAM_TIME_SLC];/* Predation given at each dam time slice */

//	float pred_pop_mean;  /* Predation population in the forebay */
//	float pred_pop_tailrace;/* Predation population in the tailrace */

        Equation spill_eqn;   /**< Spill equation to determine the proportion of
                                * fish routed to spillways (both conventional and RSW) */
        Equation fge_eqn;     /**< Fish guidance efficiency for determining the
                                * proportion of non-spilled fish routed through the bypass system */
        Equation trans_eqn;   /**< Transport mortality equation to estimate
                                * transport mortality based on water particle travel time */

        /** Delay equation that determines delay of fish at a dam depending on
         * species, time of day, season, and flow (relative to hydraulic
         * capacity). Yield a passage probability. Note that this has been mostly
         * superseded by the newer dam delay model that allows for simulating diel
         * passage. See \ref new_dam_delay. */
        Equation delay_eqn;
        Equation rsw_eqn;     /**< RSW efficiency which determines the proportion of
                                * fish that go through RSW */

        /** Proportion of non-spilled fish that pass through SBC or sluicway */
        float sluiceway_prop;

        float day_fge;        /**< Day time FGE */
        float night_fge;      /**< Night time FGE */

        /* Extra delay for passage routes */
        float spill_delay;    /**< Extra delay for fish entering conventional spillways */
        float bypass_delay;   /**< Extra delay for fish entering bypass system */
        float sluiceway_delay;/**< Extra delay for fish entering sluiceway */
        float turbine_delay;  /**< Extra delay for fish entering turbines */
        float rsw_delay;      /**< Extra delay for fish entering RSW */
        /* Probability that a fish arriving during the day will pass at night */
        float spill_day;      /**< Extra delay for fish arriving during the day when
                                * entering conventional spillways*/
        float bypass_day;     /**< Extra delay for fish arriving during the day when
                                * entering bypass system*/
        float sluiceway_day;  /**< Extra delay for fish arriving during the day
                                * when entering sluiceway*/
        float turbine_day;    /**< Extra delay for fish arriving during the day
                                * when entering turbines*/
        float rsw_day;        /**< Extra delay for fish arriving during the day when
                                * entering RSW */
};

/** \class Dam
 * \brief Dam-specific segment data (i.e. the dam parameters) */
class Dam : public RiverSegment
{
public:
    enum Location {
        Right,
        Left,
        Middle,
        None
    };

    Dam (QString dname, QString rivName = QString (""), QObject *parent = nullptr);
    Dam (const Dam &rhs);
    ~Dam ();
    void clear ();
    bool parse (CompassFile *infile);
    bool parseToken (QString token, CompassFile *infile);

    void outputDesc (CompassFile *outfile);

    Spillway *getSpillway() const;
    void setSpillway(Spillway *value);

    Basin *getBasin() const;
    void setBasin(Basin *value);

    dam_species *getSpecies() const;

    QList<PowerHouse *> getPowerhouses() const;
    void setPowerhouses(const QList<PowerHouse *> &value);
    int setPowerhouse (PowerHouse *house, int num = 0);
    int getNumPowerhouses ();

    Location getPhouseSide() const;
    void setPhouseSide(const Location &value);

    float getWidthTailrace() const;
    void setWidthTailrace(float value);

    float getLengthTailrace() const;
    void setLengthTailrace(float value);

    float getElevBase() const;
    void setElevBase(float value);

    float getElevForebay() const;
    void setElevForebay(float value);

    float getElevTailrace() const;
    void setElevTailrace(float value);

    float getFullHead() const;
    void setFullHead(float value);

    float getDepthForebay() const;
    void setDepthForebay(float value);

    float getDepthTailrace() const;
    void setDepthTailrace(float value);

    float getHeightBypass() const;
    void setHeightBypass(float value);

    int getCollector() const;
    void setCollector(int value);

    float getLengthBasin() const;
    void setLengthBasin(float value);

    float getSpecGrav() const;
    void setSpecGrav(float value);

    QList<float> getSpill() const;
    void setSpill(const QList<float> &value);

    QList<float> getSpillPlanned() const;
    void setSpillPlanned(const QList<float> &value);

    FloatPeriodList *getSpillPlannedDay() const;
    void setSpillPlannedDay(FloatPeriodList *value);

    FloatPeriodList *getSpillPlannedNight() const;
    void setSpillPlannedNight(FloatPeriodList *value);

    FloatPeriodList *getSpillLegacyPlanned() const;
    void setSpillLegacyPlanned(FloatPeriodList *value);

    FloatPeriodList *getSpillLegacyFish() const;
    void setSpillLegacyFish(FloatPeriodList *value);

    float getSpillMax() const;
    void setSpillMax(float value);

    QString &getSpillSideText();
    void setSpillSideText(const QString &text);
    Location getSpillSide() const;
    void setSpillSide(const Location &value);

    float getRswSpillMax() const;
    void setRswSpillMax(float value);

    BoolPeriodList *getRswActive() const;
    void setRswActive(BoolPeriodList *value);

    float getFlowProjectMin() const;
    void setFlowProjectMin(float value);

    float getFlowRiverMin() const;
    void setFlowRiverMin(float value);

    Transport *getTransport() const;
    void setTransport(Transport *value);

    QList<float> getDepthForebayDay() const;
    void setDepthForebayDay(const QList<float> &value);

    QList<float> getDepthTailraceDay() const;
    void setDepthTailraceDay(const QList<float> &value);

    QList<float> getDropRatioDay() const;
    void setDropRatioDay(const QList<float> &value);

    QList<float> getDropRatioDayTR() const;
    void setDropRatioDayTR(const QList<float> &value);

    QList<float> getDaylightProportion() const;
    void setDaylightProportion(const QList<float> &value);

    int getNumFishways();
    void setNumFishways(int num);
    Fishway *getFishway(int index) const;
    void setFishway(int index, Fishway *value);

private:
    /* Spillway information  */
    Spillway *spillway;

    /* Storage basin info */
    Basin *basin;      /**< Storage basin pointer. If no basin, this is nullptr. */

    QList<Fishway *>fishways;

    /** Pointer to the dam-species information struct */
    struct dam_species *species;

    /* physical characteristics; some may be computed from others,
     * depending on which were found in the river description file.
     */
    QList<PowerHouse *> powerhouses;
    Location phouseSide;


    float widthTailrace; /**< Tailrace width (ft) */
    float lengthTailrace;/**< Tailrace length in ft */
    float elevBase;      /**< Dam floor elevation (ft) */
    float elevForebay;   /**< Forebay elevation (ft) */
    float elevTailrace;  /**< Tailrace elevation (ft) */
    float fullHead;      /**< Full pool head */
    float depthForebay;  /**< Full pool forebay depth */
    float depthTailrace; /**< Nominal tailrace depth */
    float heightBypass;  /**< Height of bypass opening */

    int collector;        /**< Whether or not this dam is a collector dam.
                           * (i.e. it has PIT tag detectors and a
                           * different kind of bypass system which can
                           * produce latent effects) */

//    int numGates;           /**< Number of spill gates */
//    float widthGates;     /**< Width of each gate (ft) */
//    float spillPerGate;        /**< Max spill per gate */
//    float widthSpillway; /**< Spilway width */
    float lengthBasin;   /**< Stilling basin depth */
    float specGrav;            /**< Specific grav of roller */

    /* Spill */
    QList<float> spill; /**< Spill fraction given at each
                                        * time slice [DAM_SLICES_IN_SEASON]*/

    /** Planned spill vector is computed from planned_spill days/nights,
           then serves as input to compute_flow() [DAM_SLICES_IN_SEASON]*/
    QList<float> spillPlanned;

    FloatPeriodList *spillPlannedDay; /**< Planned spill during the day */
    FloatPeriodList *spillPlannedNight; /**< Planned spill during the night */

        /* These two variables are used to implement support for legacy tokens
         * planned_spill and fish_spill. Only planned_spill_day and
         * planned_spill_night should be used by new users and these legacy
         * tokens only set planned_spill_day and planned_spill_night. */
    FloatPeriodList *spillLegacyPlanned; /**< Planned spill during the day.
                           * This is used to implement support for legacy
                           * planned_spill tokens. */
    FloatPeriodList *spillLegacyFish; /**< Planned spill during the day. This
                           * is used to implement support for legacy
                           * fish_spill tokens. */

    float spillMax;      /**< Maximum unforced spill (kcfs) */
    Location spillSide;  /**< Left bank or right bank */
    QString spillSideText;

    /* Removable spill weirs */
    float rswSpillMax;  /**< Maximum flow (partitioned from spill)
                           * through RSW (setting this to 0 essentially
                           * turns off RSWs) */
    BoolPeriodList *rswActive; /**< Whether the RSW is
                           * active during a particular day (1 or 0) (Days in season)*/

    /* Flow */
//    float flowMax;       /**< For slider top-end */
    float flowProjectMin; /**< User data; used by modulators*/
    float flowRiverMin; /**< Calculated from river desc */
//    QList<float> flow;/**< Flow in KCFS given at each day [DAYS_IN_SEASON]*/

    /* Transport info */
    Transport *transport;/**< Pointer to transport operation information
                             * started at this dam. (this may be nullptr) */

    /* nsat stuff */
//    equation nsat_eqn;    /* Nitrogen supersaturation equation that
//                           * determines the production of gas during day
//                           * hours */
//    equation nsat_night_eqn;/* Nitrogen supersaturation equation during
//                           * night hours */
//    equation nsat_backup_eqn;/* Nitrogen supersaturation equation for when
//                           * the spill fraction drops out of the range
//                           * in which the day and night equations
//                           * reasonably model behavior */
//    float k_entrain;      /* Powerhouse side gas entrainment */

    /* actual depths, calculated from adjacent reaches, with drawdown */
    QList<float> depthForebayDay;/**< Forebay depth at each day [DAYS_IN_SEASON]*/
    QList<float> depthTailraceDay;/**< Tailrace depth at each day [DAYS_IN_SEASON]*/

    /* useful params, calculated during each simulation */
    QList<float> dropRatioDay;/**< Drop ratio. [DAYS_IN_SEASON]*/
    QList<float> dropRatioDayTR;/**< Drop ratio for the tailrace.[DAYS_IN_SEASON]*/
    QList<float> daylightProportion;/**< Proportion of
                           * the day that is light given at each dam time
                           * slice. [DAM_SLICES_IN_SEASON]*/

public slots:
    void allocate();
    void calculateFlow ();
    void calculateFlows ();
    void calculateTemp ();
    void calculateTemps ();
    void calculateSpill ();
    void calculateFish();
    void calculateStats();

    void deleteSpill ();
};

#endif // C_DAM_H


