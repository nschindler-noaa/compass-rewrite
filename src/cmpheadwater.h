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

    void outputDesc (cmpFile *outfile) override;

    bool parseData (cmpFile *infile) override;
    bool parseToken (QString token, cmpFile *infile);
    void writeFlowData(cmpFile *outfile, int indent, bool outputAll) ;
    void writeRivData (cmpFile *outfile, int indent, bool outputAll);
    void writeData (cmpFile *outfile, int indent, bool outputAll);

    void allocate(int numdays, int numsteps, int gasSteps);
    int calculateFlow() override;
    void fillRegulated ();   /**< fills headwaters that are regulated and labels them as such. */
    void fillUnRegulated (); /**< fills headwaters not filled in previous - fillRegulated must be called first. */
    void calculateFlowInputs ();
    int calculateFlows();
    int calculateTemp () override;
    void calculateTempInputs ();
    int calculateTemps();

    float getFlowMean() const;
    void setFlowMean(float newFlowMean);

protected:
//    bool regulated;          /**< Whether this is a regulated or unregulated headwater */
    float flowCoefficient;   /**< For unregulated flow */

    float flowMean;          /**< Mean flow at this headwater */

    QList<float> elevChange; /**< Delta from max */
};

#endif // CMPHEADWATER_H
