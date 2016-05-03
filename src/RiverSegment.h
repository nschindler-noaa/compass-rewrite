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
    RiverSegment (QString rivName = QString(""), QObject *parent = 0);
    RiverSegment (const RiverSegment &rhs);
    ~RiverSegment ();

    void setup ();
    bool parseToken (QString token, CompassFile *infile);

    virtual bool parse (CompassFile *infile);

    QString *riverName;
    QString *name;
    QString *abbrev;

    QList<RiverPoint *> course;
    RiverPoint * topPoint ();
    RiverPoint * bottomPoint ();
    int currentPoint;
    RiverPoint * nextPointUp ();
    RiverPoint * nextPointDn ();

    QList<Tributary *> tributaries;

    float width;          /**< Average width in feet */

    enum SegmentType {
        Dam,
        Reach,
        Headwater
    };
    SegmentType type;     /**< type of river segment. */

    unsigned int output_flags;

    unsigned int output_settings;

    bool regPoint;
    bool read_flows;
    float flow_max;
    float flow_min;
    float flow[DAYS_IN_SEASON];

    float temp[DAYS_IN_SEASON];/**< Water temperature at each model day */
    bool read_temps;     /**< true if reading temps from data file,
                          *   false if not. */


    RiverSegment *up;  /**< next segment up the same river.*/
    RiverSegment *fork;/**< next segment up for different river. */
    RiverSegment *down;/**< next segment down the (same) river. */

    int temp_1; /**< temporary value used in some calculations. */

    /** Division of river into separate flows */
    enum FlowLocation {
        Right,         /**< Right side of the segment */
        Left,          /**< Left side of the segment */
        FlowDivisions  /**< Number of flow divisions */
    };

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
