#ifndef C_RIVERSEGMENT_H
#define C_RIVERSEGMENT_H

#include <QObject>
#include <QList>
#include <QString>

#include "CompassFile.h"
#include "RiverPoint.h"
#include "Tributary.h"

#include "definitions.h"

//#define DAYS_IN_SEASON 365

class RiverSegment : public QObject
{
    Q_OBJECT
public:
    RiverSegment (QString rivName = QString(""), QObject *parent = nullptr);
    RiverSegment (const RiverSegment &rhs);
    ~RiverSegment ();

    RiverSegment &operator =(const RiverSegment &rhs);
    void setup ();
    bool parseToken (QString token, CompassFile *infile);

    virtual bool parse (CompassFile *infile);

    RiverPoint * getCurrentPoint ();
    RiverPoint * getTopPoint ();
    RiverPoint * getBottomPoint ();
    RiverPoint * getNextPointUp ();
    RiverPoint * getNextPointDn ();
    void appendPoint (RiverPoint *pt);

    QString *getRiverName() const;
    void setRiverName(QString *value);

    QString *getName() const;
    void setName(QString *value);

    QString *getAbbrev() const;
    void setAbbrev(QString *value);

    QList<Tributary *> getTributaries() const;

    QList<RiverPoint *> getCourse() const;
    bool addCoursePoint (RiverPoint *pt);

    bool getRegPoint() const;
    void setRegPoint(bool value);

    bool getReadFlows() const;
    void setReadFlows(bool value);

    float getFlowMax() const;
    void setFlowMax(float value);

    float getFlowMin() const;
    void setFlowMin(float value);

    /** Division of river into separate flows */
    enum FlowLocation {
        Right,         /**< Right side of the segment */
        Left,          /**< Left side of the segment */
        FlowDivisions  /**< Number of flow divisions */
    };
    FlowLocation getMainFlow () const;
    void setMainFlow (FlowLocation loc);
    FlowLocation getOtherFlow () const;
    void setOtherFlow (FlowLocation loc);

    bool getReadTemps() const;
    void setReadTemps(bool value);

    RiverSegment * getUpperSegment () {return up;}
    void setUpperSegment (RiverSegment *upper) {up = upper;}
    RiverSegment * getLowerSegment () {return down;}
    void setLowerSegment (RiverSegment *lower) {down = lower;}
    RiverSegment * getForkSegment () {return fork;}
    void setForkSegment (RiverSegment *forksg) {fork = forksg;}

    /** Type of river segment */
    enum SegmentType {
        Null = -1,     /**< not a segment */
        Dam,           /**< Dam segment */
        Reach,         /**< Reach (pool or free-flowing) segment */
        Headwater      /**< Headwater segment - required for every river */
    };
    SegmentType getType() const;
    void setType(const SegmentType &value);


    bool getIsRegPoint() const;
    void setIsRegPoint(bool value);

    float getWidthUpper() const;
    void setWidthUpper(float value);

    float getWidthAve() const;
    void setWidthAve(float value);

    float getWidthLower() const;
    void setWidthLower(float value);

    float getElevUpper() const;
    void setElevUpper(float value);

    float getElevLower() const;
    void setElevLower(float value);

protected:
    RiverSegment &copy (RiverSegment &rhs);

    //    River *river;

    QString *riverName = new QString();
    QString *name;
    QString *abbrev;

    QList<RiverPoint *> course;
    RiverPoint *currentPoint;
    int currentPointIndex;

    QList<Tributary *> tributaries;

    SegmentType type;     /**< type of river segment. */

    float widthUpper;
    float widthAve;         /**< Average width in feet */
    float widthLower;

    float elevUpper;
    float elevLower;


    unsigned int output_flags;

    unsigned int output_settings;

    bool isRegPoint;
    bool readFlows;
    float flowMax;
    float flowMin;
    QList<float> flow;
    FlowLocation mainFlow;
    FlowLocation otherFlow;

    QList<float> temp;/**< Water temperature at each model day [DAYS_IN_SEASON]*/
    bool readTemps;     /**< true if reading temps from data file,
                          *   false if not. */


    RiverSegment *up;  /**< next segment up the same river.*/
    RiverSegment *fork;/**< next segment up for different river. */
    RiverSegment *down;/**< next segment down the (same) river. */

    int temp_1; /**< temporary value used in some calculations. */


private:
/*    GasDistribution *gas_in; *< Input gas distribution */
/*    GasDistribution *gas_out;*< Output gas distribution */
/*    float gas_theta;      *< Mixing parameter */
/*    float *initial_gas;   *< Only defined if there is an initial gas
                        * vector which supercedes any in-river gas */
    /* * This equation is not actually stored in data files; it is
     *  included only for insight into gas distribution mechanisms.
    equation	gas_dist_eqn;*/

/*    float turbidity[STEPS_IN_SEASON]; *< Turbidity given at each model time step */
/*    int  turbidity_flag;  *< 1 if input_turb = "0N" (reading turbidity), 0 if "Off" (propegate from above)*/

/*    float pred_dist_coef; *< "Predator distribution coefficient" -
                        *  used in V1.6 pred/vol interaction. */

signals:

public slots:
    void allocate();
    void calculateFlows ();
    void calculateFlowInputs ();
    void calculateTemps ();
    void calculateTempInputs ();
    virtual void calculateFlow ();
    virtual void calculateTemp ();
    virtual void calculateFish ();
    virtual void calculateStats ();
};



#endif // C_RIVERSEGMENT_H
