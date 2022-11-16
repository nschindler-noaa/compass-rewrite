#ifndef CMPREALTIME_H
#define CMPREALTIME_H

/** \file realtime.h
 * \brief Realtime mode functions. */

//#include "definitions.h"
#include "cmpriversegment.h"

#include <QString>

#define REALTIME_ALTERNATIVE   "realtime"
#define REALTIME_FLOW_ARCHIVE  "flow.archive"
#define REALTIME_DATAFILE      "realtime.dat"
#define REALTIME_PREPROCESSOR  "rtrr"
#define REALTIME_OUTPUT_EXT    "real"
#define REALTIME_POSTPROCESSOR "realtime.sh"
#define REALTIME_OUTPUTPATH    "realtime/realtime.real"

class cmpRealtime
{
public:
    cmpRealtime();
    ~cmpRealtime();

    cmpRealtime(cmpRealtime &rhs);

    void copy(cmpRealtime &rhs);


    const QString &getName() const;
    void setName(const QString &newName);

    int getDataDate() const;
    void setDataDate(int newDataDate);

    /** Set the input pathname to a new value. */
    const QString &getInputPathname();
    void setInputPathname(const QString &newInputPathname);

    /** Set the input datafile name to a new value. */
    const QString &getDatafile();
    void setDatafile(const QString &newDatafile);

    /** Run the realtime mode, either in scenario (RT_SCENARIO) or (old) monte mode (RT_MONTE_C). */
    void run (int mode);
    int  runMonteC (QString filename, int mode);

    int selectSegment(cmpRiverSegment &rseg);

protected:
    QString name;                 /**< Name assigned to information */
    int dataDate;                 /**< date of the data */
    QString inputPathname; /**<  The input pathname   */
    QString datafile;      /**<  The name of the datafile */
};

typedef cmpRealtime RtInfo;

#endif // CMPREALTIME_H
