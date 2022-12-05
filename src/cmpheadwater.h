#ifndef CMPHEADWATER_H
#define CMPHEADWATER_H

#include "cmpriversegment.h"
\
class cmpRiver;

class cmpHeadwater : public cmpRiverSegment
{
public:
    cmpHeadwater (cmpRiver *parent = nullptr);
    cmpHeadwater (QString hname, cmpRiver *parent = nullptr);

    void resetData ();

    bool parseData (cmpFile *infile) override;
    bool parseToken (QString token, cmpFile *infile);

    void allocateDays(int numdays);
    void calculateFlow () override;
    void fillRegulated ();   /**< fills headwaters that are regulated and labels them as such. */
    void fillUnRegulated (); /**< fills headwaters not filled in previous - fillRegulated must be called first. */
    void calculateFlowInputs ();
    void calculateFlows ();
    void calculateTemp () override;
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
/*    bool turbidityRead;       *< true if values are read from a data file */

};

#endif // CMPHEADWATER_H
