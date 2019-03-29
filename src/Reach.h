#ifndef C_REACH_H
#define C_REACH_H

#include "RiverSegment.h"
#include "Scenario.h"

#define DEPTH_PADDING   0.95

#ifdef PRED_CALC
/** Species-specific parameters for reaches */
struct reach_species
{
    float pred_pop_mean;      /**< Predator population. */
};
#endif

/** Reach/reservoir-specific parameters. A reach is a segment of
 *  the actual river (where water flows). */
class Reach : public RiverSegment
{
    Q_OBJECT
public:
    Reach (QString rname, QString rivName = QString (""), QObject *parent = nullptr);

    bool parse (CompassFile *infile);
    bool parseToken (QString token, CompassFile *infile);

#ifdef PRED_CALC
    /** Species-specific parameters for reaches */
    struct reach_species *species;
#endif
    int rclass;           /**< Reach class number which should be on
                            * the interval [0..num_reach_classes-1] */
    float length;         /**< Length of segment in miles */
    float volume;         /**< Volume at maximum reservoir level in feet^3*/
    float surface_area;   /**< Average area in feet^2 */
    float depth;          /**< Average maximum depth in feet */
    float upper_depth;    /**< Maximum depth at upper end of reach in feet */
    float lower_depth;    /**< Maximum depth at lower end of reach in feet */
    float lower_elev;     /**< Height above sea level, or bottom of
                            *  river at the lower end of reach in feet. */
    float slope;          /**< Angle of sides, from vertical; deg */
    float elev_change[DAYS_IN_SEASON]; /**< Delta (difference) from maximum */

    float cur_volume[STEPS_IN_SEASON]; /**< Current water volume */
    float velocity[STEPS_IN_SEASON]; /**< Current water velocity */

    float loss_max;       /**< Maximum loss at this reach */
    float loss_min;       /**< Minimum loss at this reach */
    float loss[DAYS_IN_SEASON]; /**< Loss given at each day */

    float delta_temp[STEPS_IN_SEASON]; /**< Modifies water_temp, if needed */

    float water_particle_tt; /**< particle travel time; computed */

    void clear ();
    void calculateFlow ();
    void calculateFlows ();
    float computeVolume (float elev_chng, float upper_d, float lower_d, float wd, float slp_tan);
    float computeVelocity (float elev_chng, float upper_d, float lower_d, float avg_fl);
    float computeWTT (int startDay, int stopDay);
    void calculateTemp ();
    void calculateTemps ();
    void calculateFish();
    void calculateStats();

private:
};

#endif // C_REACH_H
