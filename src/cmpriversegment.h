#ifndef CMPRIVERSEGMENT_H
#define CMPRIVERSEGMENT_H

#include "definitions.h"
#include "cmpfile.h"
#include "cmptributary.h"

#include <QObject>

class cmpRiverSegment : public QObject
{
    Q_OBJECT
public:
    /** Indicator for sides or sections of the river. */
    enum Location {
        Right,
        Left,
        Middle,
        None
    };

    cmpRiverSegment (QObject *parent = nullptr);
    cmpRiverSegment (QString rivName, QObject *parent = nullptr);
    cmpRiverSegment (const cmpRiverSegment &rhs);
    ~cmpRiverSegment ();

    cmpRiverSegment &operator =(const cmpRiverSegment &rhs);
    void setup ();

    bool parseToken (QString token, cmpFile *infile);

    virtual bool parse (cmpFile *infile);

    cmpRiverPoint * getCurrentPoint ();
    cmpRiverPoint * getTopPoint ();
    cmpRiverPoint * getBottomPoint ();
    cmpRiverPoint * getNextPointUp ();
    cmpRiverPoint * getNextPointDn ();
    void appendPoint (cmpRiverPoint *pt);

    QString *getRiverName() const;
    void setRiverName(QString *value);

    QString *getName() const;
    void setName(QString *value);

    QString *getAbbrev() const;
    void setAbbrev(QString *value);

    const QList<cmpTributary *> &getTributaries() const;

    QList<cmpRiverPoint *> getCourse() const;
    bool addCoursePoint (cmpRiverPoint *pt);

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
        FlowRight,         /**< Right side of the segment */
        FlowLeft,          /**< Left side of the segment */
        FlowDivisions  /**< Number of flow divisions */
    };
    FlowLocation getMainFlow () const;
    void setMainFlow (FlowLocation loc);
    FlowLocation getOtherFlow () const;
    void setOtherFlow (FlowLocation loc);

    bool getReadTemps() const;
    void setReadTemps(bool value);

    cmpRiverSegment * getUpperSegment () {return up;}
    void setUpperSegment (cmpRiverSegment *upper) {up = upper;}
    cmpRiverSegment * getLowerSegment () {return down;}
    void setLowerSegment (cmpRiverSegment *lower) {down = lower;}
    cmpRiverSegment * getForkSegment () {return fork;}
    void setForkSegment (cmpRiverSegment *forksg) {fork = forksg;}

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

    int getTemp_1() const;
    void setTemp_1(int newTemp_1);

protected:
    cmpRiverSegment &copy (cmpRiverSegment &rhs);

    //    River *river;

    QString *riverName = new QString();
    QString *name;
    QString *abbrev;

    QList<cmpRiverPoint *> course;
    cmpRiverPoint *currentPoint;
    int currentPointIndex;

    QList<cmpTributary *> tributaries;

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


    cmpRiverSegment *up;  /**< next segment up the same river.*/
    cmpRiverSegment *fork;/**< next segment up for different river. */
    cmpRiverSegment *down;/**< next segment down the (same) river. */

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
    void allocateDays(int days = DAYS_IN_SEASON);
    void calculateFlows ();
    void calculateFlowInputs ();
    void calculateTemps ();
    void calculateTempInputs (int steps = STEPS_IN_SEASON, int daysteps = STEPS_PER_DAY);
    virtual void calculateFlow ();
    virtual void calculateTemp ();
    virtual void calculateFish ();
    virtual void calculateStats ();


};

#endif // CMPRIVERSEGMENT_H
