#ifndef CMPREACH_H
#define CMPREACH_H

#include "cmpriversegment.h"

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
class cmpReach : public cmpRiverSegment
{
    Q_OBJECT

public:
    explicit cmpReach (QObject *parent = nullptr);
    cmpReach (QString rname, QString rivName = QString (""), QObject *parent = nullptr);
    cmpReach (cmpReach &rhs);
    ~cmpReach () override;

    void clear ();
    bool parse (cmpFile *infile) override;
    bool parseToken (QString token, cmpFile *infile);

#ifdef PRED_CALC
    /** Species-specific parameters for reaches */
    struct reach_species *species;
#endif

    void outputDesc (cmpFile *outfile);

    void calculateFlow () override;
    void calculateFlows ();
    float computeVolume (float elev_chng, float upper_d, float lower_d, float wd, float slp_tan);
    float computeVelocity (float elev_chng, float upper_d, float lower_d, float avg_fl);
    float computeWTT (int startDay, int stopDay);
    void calculateTemp () override;
    void calculateTemps ();
    void calculateFish() override;
    void calculateStats() override;

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
    int rclass;            /**< Reach class number which should be on
                            * the interval [0..num_reach_classes-1] */
    float length;          /**< Length of segment in miles */
    float volume;          /**< Volume at maximum reservoir level in feet^3*/
    float surfaceArea;     /**< Average area in feet^2 */
    float depth;           /**< Average maximum depth in feet */
    float depthUpper;      /**< Maximum depth at upper end of reach in feet */
    float depthLower;      /**< Maximum depth at lower end of reach in feet */
//    float elevLower;      /**< Height above sea level, or bottom of
//                            *  river at the lower end of reach in feet. */
    float slope;           /**< Angle of sides, from vertical; deg */
    QList<float> elevChange; /**< Delta (difference) from maximum [STEPS_IN_SEASON]*/

    QList<float> volumeCurr; /**< Current water volume [STEPS_IN_SEASON]*/
    QList<float> velocity;  /**< Current water velocity [STEPS_IN_SEASON]*/

    float lossMax;          /**< Maximum loss at this reach */
    float lossMin;          /**< Minimum loss at this reach */
    QList<float> loss;      /**< Loss given at each day [DAYS_IN_SEASON]*/

    QList<float> tempDelta; /**< Modifies water_temp, if needed [STEPS_IN_SEASON]*/

    float waterParticleTT;  /**< particle travel time; computed */



};

#endif // CMPREACH_H
