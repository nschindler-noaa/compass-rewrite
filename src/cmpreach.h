#ifndef CMPREACH_H
#define CMPREACH_H

#include "cmpriversegment.h"
#include "cmpequation.h"

#include <QMap>

class cmpRiver;

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
    explicit cmpReach (cmpRiver *parent = nullptr);
    cmpReach (QString rname, cmpRiver *parent = nullptr);
    cmpReach (cmpReach &rhs);
    ~cmpReach () override;

    void clear ();
    void resetData ();
    void allocateDays(int newDaysPerSeason);
    void allocateSteps(int newStepsPerDay);
    void allocate(int days, int steps, int gasSteps);

    bool parseData (cmpFile *infile) override;
    bool parseToken (QString token, cmpFile *infile);
    void writeData (cmpFile *outfile, int indent, bool outputAll);
    void writeConfigData (cmpFile *outfile, int indent, bool outputAll);
    void writeLossData (cmpFile *outfile, int indent, bool outputAll);
    void writeRivData (cmpFile *outfile, int indent, bool outputAll);

#ifdef PRED_CALC
    /** Species-specific parameters for reaches */
    struct reach_species *species;
#endif

    bool parseDesc (cmpFile *descfile) override;
    void outputDesc (cmpFile *outfile) override;

    int calculateFlow() override;
    void calculateFlows ();
    float computeVolume (float elev_chng, float upper_d, float lower_d, float wd, float slp_tan);
    float computeVelocity (float elev_chng, float upper_d, float lower_d, float avg_fl);
    float computeWTT (int startDay, int stopDay);
    int calculateTemp () override;
    int calculateTemps();
    void calculateFish() override;
    void calculateStats() override;

    int getReachClass() const;
    void setReachClass(int value);
    QString getReachClassStr() const;
    void setReachClassStr(QString rclass);

    float getLength() const;
    void setLength(float value);

    float getVolume() const;
    void setVolume(float value);

    float getsurfaceArea() const;
    void setsurfaceArea(float value);

    float getDepthAve() const;
    void setDepthAve(float value);

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

    float getGasDispExp() const;
    void setGasDispExp(float newGasDispExp);

    float getFreeFlowMax() const;
    void setFreeFlowMax(float newFreeFlowMax);

private:
    int rclass;            /**< Reach class number which should be on
                            * the interval [0..num_reach_classes-1] */
    QString reachClass;
    float length;          /**< Length of segment in miles */
    float volume;          /**< Volume at maximum reservoir level in feet^3*/
    float surfaceArea;     /**< Average area in feet^2 */
    float depthAve;           /**< Average maximum depth in feet */
//    float elevLower;      /**< Height above sea level, or bottom of
//                            *  river at the lower end of reach in feet. */
    float wallSlope;           /**< Angle of sides, from vertical; deg */

    float lossMin;          /**< Minimum loss at this reach */
    float lossMax;          /**< Maximum loss at this reach */
    QList<float> loss;      /**< Loss given at each day [days_per_season]*/

    QList<float> elevChange; /**< Delta (difference) from maximum [STEPS_IN_SEASON]*/

    QList<float> volumeCurr; /**< Current water volume [STEPS_IN_SEASON]*/
    QList<float> velocity;  /**< Current water velocity [STEPS_IN_SEASON]*/

    QList<float> tempDelta; /**< Modifies water_temp, if needed [STEPS_IN_SEASON]*/

    float waterParticleTT;  /**< particle travel time; computed */
    float freeFlowMax;
    cmpEquation *freeFlowEqn;

    float gasDispExp;

    QList<float> fishDensity;
    QList<float> birdDensity1;
    QList<float> birdDensity2;
    QMap<QString, float> predMean; /**< predation mean for each species. */

//    int stepsPerSeason;

};

#endif // CMPREACH_H
