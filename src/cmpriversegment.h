#ifndef CMPRIVERSEGMENT_H
#define CMPRIVERSEGMENT_H

//#include "definitions.h"
#include "cmpfile.h"
#include "cmptributary.h"
#include "cmpgasdistribution.h"

#include <QObject>

class cmpRiver;

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

    cmpRiverSegment (cmpRiver *parent = nullptr);
    cmpRiverSegment (QString segName, cmpRiver *parent = nullptr);
    cmpRiverSegment (const cmpRiverSegment &rhs);
    ~cmpRiverSegment () override;

    cmpRiverSegment &operator =(const cmpRiverSegment &rhs);

    void setup ();
    void resetData();

    virtual bool parseData (cmpFile *infile);
    bool parseToken (QString token, cmpFile *infile);
    void outputData (cmpFile *outfile, bool outputAll);
    void writeAllData (cmpFile *outfile, int indent, bool outputAll);
    void writeConfigData (cmpFile *outfile, int indent, bool outputAll);
    void writeFlowData (cmpFile *outfile, int indent, bool outputAll);
    void writeGasData (cmpFile *outfile, int indent, bool outputAll);
    void writeTempData (cmpFile *outfile, int indent, bool outputAll);
    void writeTurbidData (cmpFile *outfile, int indent, bool outputAll);

    virtual bool parseDesc (cmpFile *descfile);
    bool parseDescToken (QString token, cmpFile *descfile);
    virtual void outputDesc(cmpFile *ofile);

    cmpRiverPoint * getCurrentPoint ();
    cmpRiverPoint * getTopPoint ();
    cmpRiverPoint * getBottomPoint ();
    cmpRiverPoint * getNextPointUp ();
    cmpRiverPoint * getNextPointDn ();
    void appendPoint (cmpRiverPoint *pt);
    QList<cmpRiverPoint *> &getCourse() {return course;}

    cmpRiver *getRiver();
    QString getRiverName() const;
    void setRiverName(QString value);

    QString getName() const;
    void setName(QString value);

    QString getAbbrev() const;
    void setAbbrev(QString value);

    const QList<cmpTributary *> &getTributaries() const;

    QList<cmpRiverPoint *> getCourse() const;
    bool addCoursePoint (cmpRiverPoint *pt);

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
        Reach,         /**< Reach (pool or free-flowing) segment */
        Dam,           /**< Dam segment */
        Headwater      /**< Headwater segment - required for every river */
//        Null           /**< not a segment */
    };
    SegmentType getType();
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

    int getDaysPerSeason() const;
    void setDaysPerSeason(int newDays_per_season);

    int getDaysPerYear() const;
    void setDaysPerYear(int newDays_per_year);

    int getStepsPerDay() const;
    void setStepsPerDay(int newStepsPerDay);

    float getWallSlope() const;
    void setWallSlope(float newWallSlope);

    int getOutputSettings() const;
    void setOutputSettings(unsigned int newOutput_settings);

    const QString &getTypeStr() const;

    float getGasTheta() const;
    void setGasTheta(float newGasTheta);

protected:
    cmpRiverSegment &copy (const cmpRiverSegment &rhs);

    //    River *river;

    QString riverName;
    QString name;
    QString typeStr;
    QString abbrev;

    QList<cmpRiverPoint *> course;
    cmpRiverPoint *currentPoint;
    int currentPointIndex;

    QList<cmpTributary *> tributaries;

    SegmentType type;     /**< type of river segment. */

    float widthUpper;
    float widthAve;         /**< Average width in feet */
    float widthLower;

    float depthUpper;
    float depthLower;

    float elevUpper;
    float elevLower;

    int outputFlags;

    int outputSettings;

    bool isRegPoint;         /**< Whether this is regulation point or not */
    bool readFlows;          /**< Whether flows are read from a data file */
    float flowMax;
    float flowMin;
    QList<float> flow;
    FlowLocation mainFlow;
    FlowLocation otherFlow;

    QList<float> temp;/**< Water temperature at each model day [days_per_season]*/
    bool readTemps;     /**< true if reading temps from data file,
                          *   false if not. */

    bool readGas;            /**< true if values are read from a data file */
    float gasTheta;
    cmpGasDistribution *gas_out; /**< Output gas distribution */
    QList<float> initial_gas; /**< Only defined if there is an initial gas
                               * vector which supercedes any in-river gas */

    bool readTurbidity;     /**< true if values are read from a data file */
    QList<float> turbidity; /**< Turbidity at each model time step */

    cmpRiverSegment *up;  /**< next segment up the same river.*/
    cmpRiverSegment *fork;/**< next segment up for different river. */
    cmpRiverSegment *down;/**< next segment down the (same) river. */

    int temp_1; /**< temporary value used in some calculations. */

    int daysPerSeason;
    int daysPerYear;
    int stepsPerDay;

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
    void allocateDays(int days);
    void calculateFlows ();
    void calculateFlowInputs ();
    void calculateTemps ();
    void calculateTempInputs ();
    virtual void calculateFlow ();
    virtual void calculateTemp ();
    virtual void calculateFish ();
    virtual void calculateStats ();


};

#endif // CMPRIVERSEGMENT_H
