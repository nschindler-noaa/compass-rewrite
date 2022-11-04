/** \file realtime.c
 * Realtime mode function implementations. */

#include "cmprealtime.h"
//#include "compass.h"
//#include "monte.h"
//#include "run_mode.h"
#include "cmplog.h"

//#include "batch.h"
//#include "compass_tokens.h"
//#include "fpe.h"
//#include "nsat.h"
//#include "release_util.h"
#include "cmpfile.h"
//#include "strtools.h"

#include <QDir>
#include <QFile>
#include <QList>
#include <QString>


#include <iostream>
#include <cstring>
#include <cstdlib>

/** Structure for holding realtime information. */
cmpRealtime::cmpRealtime()
{
    inputPathname = QString();
    datafile = QString();
}

cmpRealtime::~cmpRealtime()
{

}

cmpRealtime::cmpRealtime(cmpRealtime &rhs)
{
    copy(rhs);
}

void cmpRealtime::copy(cmpRealtime &rhs)
{
    setName(rhs.getName());
    setDataDate(rhs.getDataDate());
    setInputPathname(rhs.getInputPathname());
    setDatafile(rhs.getDatafile());
}

const QString &cmpRealtime::getName() const
{
    return name;
}

void cmpRealtime::setName(const QString &newName)
{
    name = newName;
}

int cmpRealtime::getDataDate() const
{
    return dataDate;
}

void cmpRealtime::setDataDate(int newDataDate)
{
    dataDate = newDataDate;
}

const QString &cmpRealtime::getInputPathname()
{
    return inputPathname;
}

void cmpRealtime::setInputPathname(const QString &newInputPathname)
{
    inputPathname = newInputPathname;
}

const QString &cmpRealtime::getDatafile()
{
    return datafile;
}

void cmpRealtime::setDatafile(const QString &newDatafile)
{
    datafile = newDatafile;
}



//#ifdef REALTIME

/*void  run_realtime(int mode);
void  realtime_monte_callback();
int  realtime_output_datafile();

static int  run_realtime_monte(QString filename, int mode);
static void  fput_special_array(QFile *fp, QString title, unsigned char indent_count,
                int start_date, int slice_size,
                float *array, int percentiles);
static void  fput_float_array(QFile *fp, QString title, unsigned char indent_count,
                             unsigned array_count, float *array);
static int  selected_segment();*/


/** Run the realtime mode, either in scenario (RT_SCENARIO) or (old) monte mode (RT_MONTE_C). */
void cmpRealtime::run (int mode)
{
    if (datafile.isEmpty())
        datafile = QString(REALTIME_DATAFILE);
    if (!file_status (datafile))
    {
        cmpLog::outlog->add(cmpLog::Fatal, QString("Can't access data file \"%1\" for realtime run!\n").arg(
            datafile));
        return;
    }
    else
    {
        runMonteC (datafile, mode);
    }
}

/**
 *	Output a data file \e REALTIME_ALTERNATIVE/REALTIME_ALTERNATIVE.REALTIME_OUTPUT_EXT
    with the following information:
 *        - daily passage
 *        - daily % survival
 *        - daily % transport
 *        - daily spill (%)
 *        - daily spill (kcfs)
 *        - daily fpe (fish passage efficiency)
 *        - daily flow (kcfs)
 *        - daily %nsat in forebay
 *        - daily %nsat in tailrace
 */
int realtime_output_datafile_daily (struct run *current_run)
{
    QString outputpath;
    QString tempstore;
    QFile *output = new QFile();
    struct river_segment	*segment;
    QList<QList <struct release *> > assoc_list, assocp;
    bool all = true;

    outputpath = QString ("%1/%2.%3")
        .arg(REALTIME_ALTERNATIVE).arg(REALTIME_ALTERNATIVE).arg(REALTIME_OUTPUT_EXT);
    output->setFileName (outputpath);

    if (!(output->open (QIODevice::WriteOnly)))// = fopen(outputpath, "w")))
        return -1;

    assoc_list = rt_associate_releases(current_run->release_list);
    output_string (output, 0, "# Easy-parse Real-time Data (as used in hospitals)");
    output_string (output, 0, TKS_RT_REPORT_DATE, runs[0].rt_report_date);

    for (segment = river_mouth ; segment ; segment = segment->next)
    {// right now, all segments are selected
    QString seg_name (*(segment->seg_name));//->replace (' ', '_'));
    output_string (output, 0, "Segment", seg_name);

    if (segment->seg_type == DAM_SEG)
    {
        all = true;
        FlowLocation spill_side = DAMPTR(segment)->spill_side;
        FlowLocation phouse_side = (spill_side == FLOW_LEFT) ? FLOW_RIGHT : FLOW_LEFT;

        fput_special_array(output, "Spill_gas", 1, 0, (runMode.dam_slices_per_day),
                   DAMPTR(segment)->gas_out->gas[spill_side], 1);
        fput_special_array(output, "Phouse_gas", 1, 0, (runMode.dam_slices_per_day),
                   DAMPTR(segment)->gas_out->gas[phouse_side], 1);
        fput_special_array(output, "Temperature", 1, 0,
                   (runMode.time_steps_per_day), DAMPTR(segment)->water_temp, 1);
    }
    else if (segment->seg_type == REACH_SEG)
    {
        all = false;
        fput_special_array(output, "Gas_left", 1, 0, (runMode.dam_slices_per_day),
                   RCHPTR(segment)->gas_out->gas[FLOW_LEFT], 1);
        fput_special_array(output, "Gas_right", 1, 0, (runMode.dam_slices_per_day),
                   RCHPTR(segment)->gas_out->gas[FLOW_RIGHT], 1);
        fput_special_array(output, "Temperature", 1, 0,
                   (runMode.time_steps_per_day), RCHPTR(segment)->water_temp, 1);
    }

    else if (segment->seg_type == HEADWATER_SEG)
    {
        if (HDWPTR(segment)->initial_gas != nullptr)
            fput_special_array(output, "Nsat", 1, 0, (runMode.dam_slices_per_day),
                   HDWPTR(segment)->initial_gas, 1);
/* 	    fput_float_array(output, "Nsat", 1, DAYS_IN_SEASON,
                 HDWPTR(segment)->nsat); */
    }
    for (int cnt = 0; cnt < assoc_list.count(); cnt++)
//	for (assocp = assoc_list; assocp; assocp = assocp->next)
    {
        QList<struct release *>release_list = assoc_list.at (cnt);
//	    LIST *release_list = (LIST *) assocp->item;
        int first_day;
        float rls_total;
        struct release_seg_data *data = collapse_release_data(release_list, segment,
                    &first_day, &rls_total);

        /* grab the first release for header info */
        struct release *release = (struct release *) release_list.first();

        if (data)
        {
            output_string_nr (output, 0, "Release", *(release->site->name));
            output_string_nr (output, 0, " Species", *(release->species->name));
            output_string (output, 0, " Stock", (release->rt? *release->rt->name: "?"));
            output_string (output, 2, QString ("StartDate %1 DataDate %2 Count %3")
                .arg(QString::number (first_day))
                .arg(QString::number (release->rt? release->rt->data_date: 0))
                .arg(QString::number (rls_total, 'f', 0)));

        fput_special_array(output, "Passage", 1, first_day,
                   (runMode.time_steps_per_day), data->output, 0);
        for (int i = 0; i < (runMode.time_steps_per_season); i++)
        {
            data->stats.transport_in_total += data->transport_in[i];
            data->stats.premort_transport_total += data->transport_out[i];
        }
        if (data->stats.transport_in_total > .001)
        {
            fput_special_array(output, "Transport_in", 1,
                       first_day,
                       (runMode.time_steps_per_day), data->transport_in, 0);
        }
        if (all && data->stats.premort_transport_total > .001)
        {
            fput_special_array(output, "Transport_out", 1,
                   first_day,
                   (runMode.time_steps_per_day), data->transport_out, 0);
        }
#ifdef ROUTE_TRACKING
        fput_special_array(output, "Project_in", 1,
                   first_day,
                   STEPS_PER_DAY, data->project_in, 0);
        if (all) fput_special_array(output, "Bypass_in", 1,
                   first_day,
                   STEPS_PER_DAY, data->bypass_in, 0);
        if (all) fput_special_array(output, "Bypass_out", 1,
                   first_day,
                   STEPS_PER_DAY, data->bypass_pass, 0);
        if (all && data->sluiceway_pass && data->bypass_pass && data->turbine_pass && data->spillway_pass && data->rsw_pass)
        {
            /* project_out represents pre-tailrace mort passage */
            int i;
            float *project_out = (float *) calloc (STEPS_IN_SEASON, sizeof(float));
//		    bzero((char *)project_out, sizeof(float)*STEPS_IN_SEASON);
            for (i=first_day*STEPS_PER_DAY; i < STEPS_IN_SEASON; ++i)
            {
                project_out[i] = data->sluiceway_pass[i] + data->bypass_pass[i] + data->turbine_pass[i]
                    + data->spillway_pass[i] + data->rsw_pass[i];
            }
            fput_special_array(output, "Project_out", 1,
                       first_day,
                       STEPS_PER_DAY, project_out, 0);
            free (project_out);
        }
#endif
        if (all)
        {
            float *fpe_vec = (float *) calloc ((runMode.time_steps_per_season), sizeof(float));
//		    bzero((char *)fpe_vec, sizeof(float)*STEPS_IN_SEASON);
            if (compute_fpe (fpe_vec, data))
                fput_special_array (output, "FPE", 1,
                       first_day,
                       (runMode.time_steps_per_day), fpe_vec, 1);
            free (fpe_vec);
        }
        }
//        free (data);
        release_list.clear();
    }
    output_newline (output);
//	putc('\n', output);
    }

    output->close();
//    fclose(output);
    free_assoc_list(assoc_list);
    return 0;
}

/**
 *	Output a data file \e REALTIME_ALTERNATIVE/REALTIME_ALTERNATIVE.REALTIME_OUTPUT_EXT
    with the following information:
 *        - stepwise passage
 *        - stepwise % survival
 *        - stepwise % transport
 *        - stepwise spill (%)
 *        - stepwise spill (kcfs)
 *        - stepwise fpe (fish passage efficiency)
 *        - daily flow (kcfs)
 *        - stepwise %nsat in forebay
 *        - stepwise %nsat in tailrace
 */
int realtime_output_datafile_stepwise (struct run *current_run)
{
    QString outputpath;
    QString tempstore;
    QFile *output = new QFile();
    struct river_segment	*segment;
    QList<QList <struct release *> > assoc_list, assocp;
    bool all = true;

    outputpath = QString ("%1/%2.%3")
        .arg(REALTIME_ALTERNATIVE).arg(REALTIME_ALTERNATIVE).arg(REALTIME_OUTPUT_EXT);
    output->setFileName (outputpath);

    if (!(output->open (QIODevice::WriteOnly)))// = fopen(outputpath, "w")))
        return -1;

    assoc_list = rt_associate_releases(current_run->release_list);
    output_string (output, 0, "# Easy-parse Real-time Data (as used in hospitals)");
    output_string (output, 0, TKS_RT_REPORT_DATE, runs[0].rt_report_date);

    for (segment = river_mouth ; segment ; segment = segment->next)
    {// right now, all segments are selected
    QString seg_name (*(segment->seg_name));//->replace (' ', '_'));
    output_string (output, 0, "Segment", seg_name);

    if (segment->seg_type == DAM_SEG)
    {
        all = true;
        FlowLocation spill_side = DAMPTR(segment)->spill_side;
        FlowLocation phouse_side = (spill_side == FLOW_LEFT) ? FLOW_RIGHT : FLOW_LEFT;

        fput_float_array(output, "Spill_gas", 1, (runMode.dam_slices_per_season),
                   DAMPTR(segment)->gas_out->gas[spill_side]);
        fput_float_array(output, "Phouse_gas", 1, (runMode.dam_slices_per_season),
                   DAMPTR(segment)->gas_out->gas[phouse_side]);
        fput_float_array(output, "Temperature", 1, (runMode.time_steps_per_season),
                   DAMPTR(segment)->water_temp);
    }
    else if (segment->seg_type == REACH_SEG)
    {
        all = false;
        fput_float_array(output, "Gas_left", 1, (runMode.dam_slices_per_season),
                   RCHPTR(segment)->gas_out->gas[FLOW_LEFT]);
        fput_float_array(output, "Gas_right", 1, (runMode.dam_slices_per_season),
                   RCHPTR(segment)->gas_out->gas[FLOW_RIGHT]);
        fput_float_array(output, "Temperature", 1, (runMode.time_steps_per_season),
                   RCHPTR(segment)->water_temp);
    }

    else if (segment->seg_type == HEADWATER_SEG)
    {
        if (HDWPTR(segment)->initial_gas != nullptr)
            fput_float_array(output, "Nsat", 1, (runMode.time_steps_per_season),
                   HDWPTR(segment)->initial_gas);
    }
    for (int cnt = 0; cnt < assoc_list.count(); cnt++)
//	for (assocp = assoc_list; assocp; assocp = assocp->next)
    {
        QList<struct release *>release_list = assoc_list.at (cnt);
//	    LIST *release_list = (LIST *) assocp->item;
        int first_day;
        float rls_total;
        struct release_seg_data *data = collapse_release_data(release_list, segment,
                    &first_day, &rls_total);

        /* grab the first release for header info */
        struct release *release = (struct release *) release_list.first();

        if (data)
        {
            output_string_nr (output, 0, "Release", *(release->site->name));
            output_string_nr (output, 0, " Species", *(release->species->name));
            output_string (output, 0, " Stock", (release->rt? *release->rt->name: "?"));
            output_string (output, 2, QString ("StartDate %1 DataDate %2 Count %3")
                .arg(QString::number (first_day))
                .arg(QString::number (release->rt? release->rt->data_date: 0))
                .arg(QString::number (rls_total, 'f', 0)));

        fput_step_array(output, "Passage", 1, first_day, (runMode.time_steps_per_day),
                         data->output);
        for (int i = 0; i < (runMode.time_steps_per_season); i++)
        {
            data->stats.transport_in_total += data->transport_in[i];
            data->stats.premort_transport_total += data->transport_out[i];
        }
        if (data->stats.transport_in_total > .001)
        {
            fput_step_array(output, "Transport_in", 1, first_day, (runMode.time_steps_per_day),
                             data->transport_in);
        }
        if (all && data->stats.premort_transport_total > .001)
        {
            fput_step_array(output, "Transport_out", 1, first_day, (runMode.time_steps_per_day),
                             data->transport_out);
        }
#ifdef ROUTE_TRACKING
        fput_step_array(output, "Project_in", 1, first_day, STEPS_PER_DAY,
                         data->project_in);
        if (all)
        {
            fput_step_array(output, "Bypass_in", 1, first_day, STEPS_PER_DAY,
                             data->bypass_in);
            fput_step_array(output, "Bypass_out", 1, first_day, STEPS_PER_DAY,
                             data->bypass_pass);
            if (data->sluiceway_pass && data->bypass_pass && data->turbine_pass && data->spillway_pass && data->rsw_pass)
            {
                /* project_out represents pre-tailrace mort passage */
                int i;
                float *project_out = (float *) calloc (STEPS_IN_SEASON, sizeof(float));
//                bzero((char *)project_out, sizeof(float)*STEPS_IN_SEASON);
                for (i=first_day*STEPS_PER_DAY; i < STEPS_IN_SEASON; ++i)
                {
                    project_out[i] = data->sluiceway_pass[i] + data->bypass_pass[i] + data->turbine_pass[i]
                        + data->spillway_pass[i] + data->rsw_pass[i];
                }
                fput_step_array(output, "Project_out", 1, first_day, STEPS_PER_DAY,
                                 project_out);
                free (project_out);
            }
        }
#endif
        if (all)
        {
            float *fpe_vec = (float *) calloc ((runMode.time_steps_per_season), sizeof(float));
//            bzero((char *)fpe_vec, sizeof(float)*STEPS_IN_SEASON);
            if (compute_fpe (fpe_vec, data))
                fput_step_array (output, "FPE", 1, first_day, (runMode.time_steps_per_day), fpe_vec);
            free (fpe_vec);
        }
        }
        release_list.clear();
    }
    output_newline (output);
    }

    output->close();

    free_assoc_list(assoc_list);
    return 0;
}


/** Collapse an array to daily values and output.
 * Some arrays require special treatment, consisting of collapsing the
 * timesteps into days.  This is done by dumping to temp array before
 * output.  It is assumed that the arrays passed in are of size
 * DAYS_IN_SEASON * STEPS_(or SLICES)_PER_DAY.  After collapsing to daily
 * values, the range from start date to DAYS_IN_SEASON is output.
 */
void fput_special_array(QFile *fp, QString title, unsigned char indent_count,
                int start_date, int slice_size,
                float *farray, int percentiles)
{
    float *outvec = (float *) calloc ((runMode.days_in_season), sizeof(float));//0;
    int i = 0;

    if (!farray) return;
    if (!outvec)  return;

    /* percentiles are combined by averaging, others by summing */
    if (percentiles)
        for (i = 0; i < (runMode.days_in_season) * slice_size; ++i)
            outvec[i / slice_size] += (farray[i] / slice_size);
    else
        for (i = 0; i < (runMode.days_in_season) * slice_size; ++i)
            outvec[i / slice_size] += farray[i];

    fput_float_array(fp, title, indent_count,
             (runMode.days_in_season) - start_date,
             &outvec[start_date]);

    free (outvec);
}

/** Output an array without collapsing.
 * fput_float_array() only strips off trailing zeros.  This routine uses
 * the first starting step (or slice) as the start of the output.  After
 * truncating the array, the range from start date to DAYS_IN_SEASON
 * is output in steps.
 */
void fput_step_array (QFile *fp, QString title, unsigned char indent_count,
                      int first_day, int steps_per_day, float *farray)
{
    int start_step = first_day * steps_per_day;
    int total_steps = steps_per_day * (runMode.days_in_season);
    int array_steps = total_steps - start_step;
    float *outvec = (float *) calloc (array_steps, sizeof(float));
    int i = 0, j;

    if (!farray) return;
    if (!outvec)  return;


    for (i = start_step, j = 0; i < total_steps; ++i, j++)
        outvec[j] = farray[i];

    fput_float_array(fp, title, indent_count, array_steps, outvec);

    free (outvec);
}

#define VALUES_PER_LINE	 5      /**< Number of items per line. */
#define FLOAT_FORMAT    "%.2f"  /**< Decimal precision. */
/** Output a float array. Assumes one array value per day. Ouputs five
 * values on a line with two decimal place precision.
 */
void fput_float_array(QFile *fp, QString title, unsigned char indent_count,
                             unsigned array_count, float *farray)
{
    unsigned  loop_count;
    unsigned  i;

    if (!farray) return;
    /* trim trailing zeros, but always print at least one */
    while (array_count > 1 && farray[array_count-1] < 0.005)
        --array_count;
    if (array_count == 0 && farray[array_count] == 0.0)
        return;

    if (indent_count > 1)
    {
        i = indent_count - 1;
        while (i--)
            output_tab (fp);
    }

    if (title.isEmpty())
        title = QString ("Array");

    output_int (fp, 0, title, array_count);
    loop_count = 0;
    while (1)
    {
        if (loop_count % VALUES_PER_LINE == 0)
        {
            i = indent_count;
            while (i--)
                output_tab (fp);
        }

        output_float_nr (fp, 0, "", farray[loop_count], 2);

        if (++loop_count >= array_count) break;	/* Real exit point */

        if (loop_count % VALUES_PER_LINE == 0)
            output_newline (fp);
        else
            output_space (fp);
   }
    output_newline (fp);
}

int cmpRealtime::selectSegment(cmpRiverSegment &segment)
{
    segment.setTemp_1(-1);
    return 1;		// For now, all are selected
}

int cmpRealtime::runMonteC (QString filename, int mode)
{
    struct monte_carlo_data	monte_data;
    struct altern_data		alternative;
    struct input_file		input_file;
    uint err = 0;
    QDir alt_dir (QDir::current());

    char dbuf[MAXPATHLEN];
    char fbuf[256];

    monte_data.nalternatives 	= 1;
    monte_data.alts 		= &alternative;

    if (mode==RT_MONTE_C)
        monte_data.flags =
            MF_REALTIME_MONTE | ARCHIVE_SPILL | ARCHIVE_ELEV | ARCHIVE_FLOW;
    else
        monte_data.flags = MF_REALTIME_MONTE;

//    alternative.load_directory   = strcpy (dbuf, current_dir().toUtf8().data());
    strcpy (dbuf, current_dir().toUtf8().data());
    alternative.load_directory   = dbuf;
    alternative.nyears		 = 1;
    alternative.ngames 		 = 1;
    alternative.alternative_name = REALTIME_ALTERNATIVE;
    if (mode == RT_MONTE_C)
    {
        alternative.flow_archive     = REALTIME_FLOW_ARCHIVE;
        alternative.use_flow_from    = ARCHIVE_FLOW;
        alternative.use_spill_from   = ARCHIVE_SPILL;
        alternative.use_elev_from    = ARCHIVE_ELEV;
    }
    else
    {
        alternative.flow_archive     = nullptr;
        alternative.use_flow_from    = 0;
        alternative.use_spill_from   = 0;
        alternative.use_elev_from    = 0;
    }
    alternative.multi            = 0;
    alternative.input_files      = &input_file;
    alternative.changed          = 0;
    if (filename.isEmpty())
        input_file.file_name	= REALTIME_DATAFILE;
    else
    {
        strcpy (fbuf, filename.toUtf8().data());
        input_file.file_name = fbuf;
    }

    input_file.list		= (char **) 0;

    /* Create the directory to be made. */
    QDir::current().mkdir (REALTIME_ALTERNATIVE);
    /* Check to see if directory got made. */
    alt_dir.setPath (REALTIME_ALTERNATIVE);
    if (!alt_dir.exists())
    {
        cmpLog::outlog->add(cmpLog::Error, "Can't create realtime alternative directory!\n");
        return -1;
    }
    return run_monte_carlo(&monte_data);
}


/** Some routines for handling the realtime data stored in releases.  All
 * rtinfo structs must be created and deleted using these routines, for
 * proper operation of the module.  An ivariant on the rtinfo struct is
 * that an allocated "name" always exists.
 */

/** Copy the data from one rtinfo to another.  if the from "param" is null,
 * fill with generic data.  \return the RfInfo pointer or 0 on failure.
 */
RtInfo *copy_rtinfo (RtInfo &from, RtInfo &to)
{
    to.copy(from);
    return &to;
}

/** Allocate and return an rtinfo with the requested name if specified.
 * Makes a copy of the name string.  \return the RfInfo pointer or 0 on failure.
 */
RtInfo *construct_rtinfo (QString name)
{
    RtInfo *rtinfo = new RtInfo();
    rtinfo->setName(name);
    return rtinfo;
}

/** Deletes an rtinfo and zeros the pointer to it.
 */
void destroy_rtinfo (RtInfo **rtinfopp)
{
    RtInfo *ptr = *rtinfopp;
    if (ptr != nullptr)
    {
        delete (ptr);
        *rtinfopp = nullptr;
    }
}

/** Clone an rtinfo.  This is like a copy constructor. \return the RfInfo pointer or 0 on failure */
RtInfo *clone_rtinfo (RtInfo *from)
{
    RtInfo *newInfo = nullptr;
    if (from != nullptr)
    {
        newInfo = new RtInfo(*from);
    }

    return newInfo;
}

/** Reset the name of an rtinfo struct.  Copies the string.  \return non-zero on success or 0 on
 * failure.
 */
int rt_reset_name (RtInfo *rtinfo, QString &name)
{
    if (!name.isEmpty())
    {
        rtinfo->setName(name);
        return 1;
    }
    else
    {
        return 0;
    }
}

//#endif	/* REALTIME */

