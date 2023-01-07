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
    void writeData (cmpFile *outfile, int indent, bool outputAll);

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
    bool regulated;          /**< Whether this is a regulated or unregulated headwater */
    float flowCoefficient;   /**< For unregulated flow */

    float flowMean;          /**< Mean flow at this headwater */

    QList<float> elevChange; /**< Delta from max */
};

#endif // CMPHEADWATER_H
