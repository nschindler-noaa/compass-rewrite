#ifndef CMPHEADWATER_H
#define CMPHEADWATER_H

#include "cmpriversegment.h"

class cmpHeadwater : public cmpRiverSegment
{
public:
    cmpHeadwater (QString hname, QString rivName = QString (""), QObject *parent = nullptr);

    void reset ();

    bool parse (cmpFile *infile);
    bool parseToken (QString token, cmpFile *infile);

    void allocateDays(int numdays);
    void calculateFlow ();
    void fillRegulated ();   /**< fills headwaters that are regulated and labels them as such. */
    void fillUnRegulated (); /**< fills headwaters not filled in previous - fillRegulated must be called first. */
    void calculateFlowInputs ();
    void calculateFlows ();
    void calculateTemp ();
    void calculateTempInputs ();
    void calculateTemps ();

protected:
    bool flowsRead;     /**< true if flow values are read from a data file. */
    bool regulated;          /**< Whether this is a regulated or unregulated headwater */
    float flowCoefficient;   /**< For unregulated flow */

    float flowMean;          /**< Mean flow at this headwater */

        QList<float> elevChange; /**< Delta from max */
/*    GasDistribution *gas_out; *< Output gas distribution */
/*    float *initial_gas;       *< Only defined if there is an initial gas
                               * vector which supercedes any in-river gas */

/*    float turbidity[STEPS_IN_SEASON]; *< Turbidity at each model
                                                            *time step */
/*    bool turbidity_flag;       *< 1 if input_turb = "0N" (reading turbidity),
                                                            * 0 if "Off" (propogate from above)*/

};

#endif // CMPHEADWATER_H
