/** \file realtime.h
 * \brief Realtime mode functions. */

#include "definitions.h"

#ifdef REALTIME

#ifndef C_REALTIME_H
#define C_REALTIME_H

#include <QString>

#define REALTIME_ALTERNATIVE   "realtime"
#define REALTIME_FLOW_ARCHIVE  "flow.archive"
#define REALTIME_DATAFILE      "realtime.dat"
#define REALTIME_PREPROCESSOR  "rtrr"
#define REALTIME_OUTPUT_EXT    "real"
#define REALTIME_POSTPROCESSOR "realtime.sh"
#define REALTIME_OUTPUTPATH    "realtime/realtime.real"


/** Structure for holding realtime information. */
class RtInfo {
public:
    RtInfo();
    ~RtInfo();

protected:
    QString name;       //*< Name assigned to information */
    int dataDate;    //*< date of the data */
};

/** \fn rt_set_input_pathname Set the input pathname to a new value by copying the string. */
void rt_set_input_pathname (QString path);
/** \fn construct_rtinfo Allocate and return an rtinfo with the requested name if specified.
 * Makes a copy of the name string.
 * \return the RfInfo pointer or 0 on failure. */
RtInfo *construct_rtinfo (QString name);
/** \fn destroy_rtinfo Deletes an rtinfo and zeros the pointer to it.   */
void destroy_rtinfo (RtInfo **RtInfopp);
/** \fn clone_rtinfo Clone an rtinfo.  This is like a copy constructor.
 * \return the RfInfo pointer or 0 on failure */
RtInfo *clone_rtinfo (RtInfo *from);
/** \fn rt_reset_name Reset the name of an rtinfo struct.  Copies the string.
 * \return non-zero on success or 0 on failure. */
int rt_reset_name (RtInfo *rtinfo, QString name);
/** \fn realtime_output_datafile
 * Output a data file \e REALTIME_ALTERNATIVE/REALTIME_ALTERNATIVE.REALTIME_OUTPUT_EXT
 *  with the following information:
 *        - daily passage
 *        - daily % survival
 *        - daily % transport
 *        - daily spill (%)
 *        - daily spill (kcfs)
 *        - daily fpe (fish passage efficiency)
 *        - daily flow (kcfs)
 *        - daily %nsat in forebay
 *        - daily %nsat in tailrace
 * \param current_run This run
 * \return -1 if can't open output file or 0 otherwise.
 */
void rt_set_datafile (QString name);
int realtime_output_datafile (struct run *current_run);
void realtime_monte_callback();
void run_realtime (int mode);


#endif /* C_REALTIME_H */
#endif /* REALTIME */

