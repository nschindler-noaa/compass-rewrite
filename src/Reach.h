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

    int getRclass() const;
    void setRclass(int value);

    float getLength() const;
    void setLength(float value);

    float getVolume() const;
    void setVolume(float value);

    float getsurfaceArea() const;
    void setsurfaceArea(float value);

    float getDepth() const;
    void setDepth(float value);

    float getDepthUpper() const;
    void setDepthUpper(float value);

    float getDepthLower() const;
    void setDepthLower(float value);

    float getSlope() const;
    void setSlope(float value);

    float getLossMax() const;
    void setLossMax(float value);

    float getLossMin() const;
    void setLossMin(float value);

private:
    int rclass;           /**< Reach class number which should be on
                            * the interval [0..num_reach_classes-1] */
    float length;         /**< Length of segment in miles */
    float volume;         /**< Volume at maximum reservoir level in feet^3*/
    float surfaceArea;   /**< Average area in feet^2 */
    float depth;          /**< Average maximum depth in feet */
    float depthUpper;    /**< Maximum depth at upper end of reach in feet */
    float depthLower;    /**< Maximum depth at lower end of reach in feet */
//    float elevLower;     /**< Height above sea level, or bottom of
//                            *  river at the lower end of reach in feet. */
    float slope;          /**< Angle of sides, from vertical; deg */
    float elevChange[DAYS_IN_SEASON]; /**< Delta (difference) from maximum */

    float volumeCurr[STEPS_IN_SEASON]; /**< Current water volume */
    float velocity[STEPS_IN_SEASON]; /**< Current water velocity */

    float lossMax;       /**< Maximum loss at this reach */
    float lossMin;       /**< Minimum loss at this reach */
    float loss[DAYS_IN_SEASON]; /**< Loss given at each day */

    float tempDelta[STEPS_IN_SEASON]; /**< Modifies water_temp, if needed */

    float waterParticleTT; /**< particle travel time; computed */

};

#endif // C_REACH_H
