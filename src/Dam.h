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
        Middle
    };

    Dam (QString dname, QString rivName = QString (""), QObject *parent = 0);
    Dam (const Dam &rhs);
    ~Dam ();
    void clear ();
    bool parse (CompassFile *infile);
    bool parseToken (QString token, CompassFile *infile);

    /* Storage basin info */
    Basin *basin;      /**< Storage basin pointer. If no basin, this is NULL. */

    /** Pointer to the dam-species information struct */
    struct dam_species *species;

    /* physical characteristics; some may be computed from others,
     * depending on which were found in the river description file.
     */
    QList<PowerHouse *> powerhouses;
    Location phouse_side;


    float width;          /**< Tailrace width (ft) */
    float tailrace_length;/**< Tailrace length in ft */
    float base_elev;      /**< Dam floor elevation (ft) */
    float forebay_elev;   /**< Forebay elevation (ft) */
    float tailrace_elev;  /**< Tailrace elevation (ft) */
    float full_head;      /**< Full pool head */
    float full_fb_depth;  /**< Full pool forebay depth */
    float bypass_height;  /**< Height of bypass orifice */

    int collector;        /**< Whether or not this dam is a collector dam.
                           * (i.e. it has PIT tag detectors and a
                           * different kind of bypass system which can
                           * produce latent effects) */

    int ngates;           /**< Number of spill gates */
    float gate_width;     /**< Width of each gate (ft) */
    float pergate;        /**< Max spill per gate */
    float spillway_width; /**< Spilway width */
    float basin_length;   /**< Stilling basin depth */
    float sgr;            /**< Specific grav of roller */

    /* Spill */
    float spill[DAM_SLICES_IN_SEASON]; /**< Spill fraction given at each
                                        * time slice */

    /** Planned spill vector is computed from planned_spill days/nights,
           then serves as input to compute_flow() */
    float planned_spill[DAM_SLICES_IN_SEASON];

    PeriodList<float> *planned_spill_day; /**< Planned spill during the day */
    PeriodList<float> *planned_spill_night; /**< Planned spill during the night */

        /* These two variables are used to implement support for legacy tokens
         * planned_spill and fish_spill. Only planned_spill_day and
         * planned_spill_night should be used by new users and these legacy
         * tokens only set planned_spill_day and planned_spill_night. */
    PeriodList<float> *legacy_planned_spill; /**< Planned spill during the day.
                           * This is used to implement support for legacy
                           * planned_spill tokens. */
    PeriodList<float> *legacy_fish_spill; /**< Planned spill during the day. This
                           * is used to implement support for legacy
                           * fish_spill tokens. */

    float spill_cap;      /**< Maximum unforced spill (kcfs) */
    Location spill_side;/**< Left bank or right bank */

    /* Removable spill weirs */
    float rsw_spill_cap;  /**< Maximum flow (partitioned from spill)
                           * through RSW (setting this to 0 essentially
                           * turns off RSWs) */
    PeriodList<bool> *rsw_active; /**< Whether the RSW is
                           * active during a particular day (1 or 0) */

    /* Flow */
    float flow_max;       /**< For slider top-end */
    float flow_min_project; /**< User data; used by modulators*/
    float flow_min_river; /**< Calculated from river desc */
    float flow[DAYS_IN_SEASON];/**< Flow in KCFS given at each day */

    /* Transport info */
    Transport *transport;/**< Transport operation information.
                             * (this may be NULL) */

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
    float forebay_depth[DAYS_IN_SEASON];/**< Forebay depth at each day */
    float tailrace_depth[DAYS_IN_SEASON];/**< Tailrace depth at each day */

    /* useful params, calculated during each simulation */
    float drop_ratio[DAYS_IN_SEASON];/**< Drop ratio. */
    float tr_drop_ratio[DAYS_IN_SEASON];/**< Drop ratio for the tailrace.*/
    float daylight_proportion[DAM_SLICES_IN_SEASON];/**< Proportion of
                           * the day that is light given at each dam time
                           * slice. */

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


