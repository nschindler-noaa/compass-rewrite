#include "Release.h"

/** \file
 * \brief Manage release structures.
 *
 * This section contains the function definitions for creating,
 * destroying, and managing releases.  These functions must be used for
 * proper memory management to occur.
 */
#include <iostream>
#include <cstdlib>

#include "Log.h"
//#include "batch.h"
#include "Realtime.h"
//#include "release.h"
//#include "point.h"
//#include "strtools.h"

/* these items defined in "release_frame.c" */
extern int release_mod_flag,
           release_exist_flag;

/* This section contains the function definitions for creating,
 * destroying, and managing releases.  These functions must be used for
 * proper memory management to occur.
 */

/* * Allocate a release and fill with default data.
 * \return A pointer to the newly allocated release structure

struct release *new_release ()
{
    struct release *rls = (struct release *) malloc(sizeof(struct release));
    if (!rls)
        return 0;
#ifdef REALTIME
    rls->rt = 0;
#endif
    copy_release(0, rls);
    return rls;
}*/
Release::Release()
{
}

/** Copy the data from one release to another. The "new" release must
 * be allocated already. If the "from" release is nullptr, fill
 * the "new" release with default data.
 */
Release::Release (Release &rhs)
{
    name = new QString (*rhs.name);
    site = rhs.site;
    species = rhs.species;
    stock = rhs.stock;
    startDay = rhs.startDay;
    fishReleased.clear();
    for (int i = 0; i < rhs.fishReleased.count(); i++)
        fishReleased.append(rhs.fishReleased[i]);
#ifdef REALTIME
//    if (rhs.rt)
//        rt = clone_rtinfo (rhs.rt);
#endif

}


/** Free all data associated with a release.  External modules other than
 * "release_frame.c" should use delete_release() below.
 */
Release::~Release ()
{
#ifdef REALTIME
//    destroy_rtinfo (&rt);
#endif
    delete name;
//   segments.clear ();
}

/** Dump all releases in the system. */



/** Insert an allocated release into the system release list.  Construct
 * subsidary data areas (e.g. rls_seg) as necessary.
 */
void Release::activate ()
{

}
/*
void activate_release (struct release *rls)
{
    struct rls_seg *rls_seg;
    struct river_segment *river_seg;
    void update_release_frame();
    void add_to_menu();
        rls->next = current->release_list;
    if (rls->next)
        {
                rls->next->prev = rls;
    }
    current->release_list = rls;
    rls->prev = nullptr;
    rls_seg = (struct rls_seg *)0;

    for (river_seg = rls->site->seg; river_seg; river_seg = river_seg->down)
        {
                if (rls_seg)
                {
                        if (! (rls_seg->down =
                           (struct rls_seg *)malloc(sizeof(struct rls_seg))))
                        {
                                log_msg(L_ERROR, "no rls seg mem for release %s\n",
                                        rls->site->name->toUtf8().data());
                                return;
                        }
                        bzero((char *)rls_seg->down, sizeof(struct rls_seg));
                        rls_seg->down->up = rls_seg;
                        rls_seg = rls_seg->down;
                }
                else
                {
                        if (! (rls_seg =
                           (struct rls_seg *)malloc(sizeof(struct rls_seg))))
                        {
                                log_msg(L_ERROR, "no rls seg mem for release %s\n",
                                        rls->site->name->toUtf8().data());
                                return;
                        }
                        bzero((char *)rls_seg, sizeof(struct rls_seg));
                        rls_seg->up = (struct rls_seg *)0;
                        rls->rls_seg = rls_seg;
                }
                rls_seg->data = nullptr;
                rls_seg->down = (struct rls_seg *)0;
                rls_seg->river_segment = river_seg;
    }
}
 Delete a release structure and free the memory
void delete_release (struct release *rls)
{
        struct rls_seg *rls_seg;
        struct release *find_release();
        void update_release_values();
        void set_release_label();
        void set_frame_species();
        void update_release_frame();
        void add_to_menu();

        if (rls->prev)
                rls->prev->next = rls->next;
        else
                current->release_list = rls->next;

        if (rls->next)
                rls->next->prev = rls->prev;
        while ((rls_seg = rls->rls_seg) != nullptr)
        {
            if (rls_seg->data)
                {
                int i = 0;
                while (rls_seg->data[i])
                {
                    if (rls_seg->data[i] != (struct rls_seg_data *) -1)
                    {
                                if (rls_seg->data[i]->output)
                                        free(rls_seg->data[i]->output);
                                if (rls_seg->data[i]->length)
                                        free(rls_seg->data[i]->length);
                                if (rls_seg->data[i]->dam_spill)
                                        free(rls_seg->data[i]->dam_spill);
                                if (rls_seg->data[i]->output_transport)
                                        free(rls_seg->data[i]->output_transport);
                                if (rls_seg->data[i]->transport_in)
                                        free(rls_seg->data[i]->transport_in);
                                if (rls_seg->data[i]->transport_in_length)
                                        free(rls_seg->data[i]->transport_in_length);
                                if (rls_seg->data[i]->transport_out)
                                        free(rls_seg->data[i]->transport_out);
                                if (rls_seg->data[i]->delayed_survival_rate)
                                        free (rls_seg->data[i]->delayed_survival_rate);
#ifdef ROUTE_TRACKING
                                if (rls_seg->data[i]->project_in)
                                        free(rls_seg->data[i]->project_in);
                                if (rls_seg->data[i]->turbine_in)
                                        free(rls_seg->data[i]->turbine_in);
                                if (rls_seg->data[i]->sluiceway_in)
                                        free(rls_seg->data[i]->sluiceway_in);
                                if (rls_seg->data[i]->bypass_in)
                                        free(rls_seg->data[i]->bypass_in);
                                if (rls_seg->data[i]->spillway_in)
                                        free(rls_seg->data[i]->spillway_in);
                                if (rls_seg->data[i]->rsw_in)
                                        free(rls_seg->data[i]->rsw_in);

                                if (rls_seg->data[i]->sluiceway_pass)
                                        free(rls_seg->data[i]->sluiceway_pass);
                                if (rls_seg->data[i]->bypass_pass)
                                        free(rls_seg->data[i]->bypass_pass);
                                if (rls_seg->data[i]->turbine_pass)
                                        free(rls_seg->data[i]->turbine_pass);
                                if (rls_seg->data[i]->spillway_pass)
                                        free(rls_seg->data[i]->spillway_pass);
                                if (rls_seg->data[i]->rsw_pass)
                                        free(rls_seg->data[i]->rsw_pass);
                                if (rls_seg->data[i]->transport_pass)
                                        free(rls_seg->data[i]->transport_pass);
                                if (rls_seg->data[i]->transport_arrived)
                                        free(rls_seg->data[i]->transport_arrived);
#endif
                                if (rls_seg->data[i]->d)
                                {
                                        delayed_mortality_table_free(rls_seg->data[i]->d);
                                        rls_seg->data[i]->d = nullptr;
                                        rls_seg->data[i]->stats.d = nullptr;
                                }
                                free(rls_seg->data[i]);
                    }
                    i++;
                }
                free(rls_seg->data);
            }
            rls->rls_seg = rls_seg->down;
            free(rls_seg);
                if (rls->rls_seg)
                        rls->rls_seg->up = nullptr;
        }
        if (rls->stats.d)
        {
                delayed_mortality_table_free(rls->stats.d);
                rls->stats.d = nullptr;
        }

    free_release(rls);
}


/** Allocate the game pointers for this release for the specified number
 * of games.  Make sure the list of pointers to rls_seg_data items
 * [ngames] is big enough.
 *	\return 0 on error,
 *		1 on success
 */
/*
int alloc_seg_data (struct release *release, int games)
{
    struct rls_seg *rls_seg = release->rls_seg;
    int i;

    while (rls_seg)
    {
        if (! rls_seg->data)		/* alloc the game pointer list,
                                           nullptr terminated */
/*        {
            if ( ! (rls_seg->data =
                (struct rls_seg_data **)
                    malloc(sizeof(struct rls_seg_data *) * (games+1))))
            {
                        log_msg(L_FATAL, "no mem for data ptrs for release %s\n",
                                release->site->name->toUtf8().data());
                        return(0);
            }
            /* use -1 as place holder, nullptr as terminator */
/*            for (i = 0; i < games; i++)
                        rls_seg->data[i] = (struct rls_seg_data *) -1;
            rls_seg->data[games] = nullptr;
        }
        else
        {
            /* more game pointers needed? */
/*            i = 0;
            while (i < games && rls_seg->data[i] != nullptr)
                i++;

            if (i < games)
            {
                        struct rls_seg_data **tmp;

                        if (! (tmp = (struct rls_seg_data **)
                                   realloc(rls_seg->data,
                                           sizeof(struct rls_seg_data *) * (games+1))))
                        {
                                log_msg(L_FATAL,
                                        "cannot realloc data ptrs for release %s\n",
                                        release->site->name->toUtf8().data());
                                return(0);
                        }
                        rls_seg->data = tmp;
                        for ( ; i < games; i++)
                                rls_seg->data[i] = (struct rls_seg_data *) -1;
                        rls_seg->data[games] = nullptr;
            }
        }

        /* alloc data segs for all games if needed */
/*        for (i=0; i < games; i++)
        {
            if (rls_seg->data[i] == (struct rls_seg_data *) -1)
            {
                        if ( !(rls_seg->data[i] = (struct rls_seg_data *)
                                   malloc(sizeof(struct rls_seg_data))))
                        {
                                log_msg(L_FATAL,
                                        "no mem for game %d data for release %s\n",
                                        i, release->site->name->toUtf8().data());
                                return(0);
                        }
                        else
                        {
                                rls_seg->data[i]->output = nullptr;
                                rls_seg->data[i]->length = nullptr;
                                rls_seg->data[i]->dam_spill = nullptr;
                                rls_seg->data[i]->output_transport = nullptr;
                                rls_seg->data[i]->transport_out = nullptr;
                                rls_seg->data[i]->transport_in = nullptr;
                                rls_seg->data[i]->transport_in_length = nullptr;
                                rls_seg->data[i]->delayed_survival_rate = nullptr;
#ifdef ROUTE_TRACKING
                                rls_seg->data[i]->project_in = nullptr;
                                rls_seg->data[i]->turbine_in = nullptr;
                                rls_seg->data[i]->sluiceway_in = nullptr;
                                rls_seg->data[i]->bypass_in = nullptr;
                                rls_seg->data[i]->spillway_in = nullptr;
                                rls_seg->data[i]->rsw_in = nullptr;

                                rls_seg->data[i]->sluiceway_pass = nullptr;
                                rls_seg->data[i]->bypass_pass = nullptr;
                                rls_seg->data[i]->turbine_pass = nullptr;
                                rls_seg->data[i]->spillway_pass = nullptr;
                                rls_seg->data[i]->rsw_pass = nullptr;
                                rls_seg->data[i]->transport_pass = nullptr;
                                rls_seg->data[i]->transport_arrived = nullptr;
#endif
                                rls_seg->data[i]->d = nullptr;
                        }
            }
        }
        rls_seg = rls_seg->down;
    }
    return(1);
}


/** Clear the data area for this release and game.  Call alloc_seg_data to
 * realloc the game pointers if necessary.  rls_seg_data and the output array
 * will be allocated here if necessary.
 *	\return 0 on error,
 *		1 on success
 */

/*
int clear_seg_data (struct release *release, int game)
{
    struct rls_seg *rls_seg = release->rls_seg;

    /* alloc/realloc the game pointers if necessary, being sure to pass
     * in the number of games, which is game+1, since game starts at 0
     */
/*    if (! alloc_seg_data(release, game+1))
        return(0);

    while (rls_seg)
    {
        /* zero data areas.  */
/*        rls_seg->data[game]->nopass = 0;
        bzero((char *)&rls_seg->data[game]->stats, sizeof(struct rls_stats));
        if ( !rls_seg->data[game]->output)
            if ( !(rls_seg->data[game]->output = (float *) malloc(STEPS_IN_RUN * sizeof(float))))
            {
                        log_msg(L_DBGMSG, "Out of heap memory...realloc'ing\n");
                        if (!(rls_seg->data[game]->output = (float *) malloc(STEPS_IN_RUN * sizeof(float))))
                        {
                                log_msg(L_FATAL, "No memory for output heap!\n");
                                return(0);
                        }
            }
        if ( !rls_seg->data[game]->length)
            if ( !(rls_seg->data[game]->length = (float *) malloc(STEPS_IN_RUN * sizeof(float))))
            {
                        log_msg(L_DBGMSG, "Out of heap memory...realloc'ing\n");
                        if (!(rls_seg->data[game]->length = (float *) malloc(STEPS_IN_RUN * sizeof(float))))
                        {
                                log_msg(L_FATAL, "No memory for length heap!\n");
                                return(0);
                        }
            }
        if ( !rls_seg->data[game]->dam_spill)
            if ( !(rls_seg->data[game]->dam_spill = (float *) malloc(STEPS_IN_RUN * sizeof(float))))
            {
                        log_msg(L_DBGMSG, "Out of heap memory...realloc'ing\n");
                        if (!(rls_seg->data[game]->dam_spill = (float *) malloc(STEPS_IN_RUN * sizeof(float))))
                        {
                                log_msg(L_FATAL, "No memory for dam_spill heap!\n");
                                return(0);
                        }
            }
        if ( !rls_seg->data[game]->output_transport)
            if ( !(rls_seg->data[game]->output_transport = (float *) malloc(STEPS_IN_RUN * sizeof(float))))
            {
                        log_msg(L_DBGMSG, "Out of heap memory...realloc'ing\n");
                        if (!(rls_seg->data[game]->output_transport = (float *) malloc(STEPS_IN_RUN * sizeof(float))))
                        {
                                log_msg(L_FATAL, "No memory for output_transport heap!\n");
                                return(0);
                        }
            }
        bzero((char *)rls_seg->data[game]->output,
              sizeof(float) * STEPS_IN_RUN);
    bzero((char *)rls_seg->data[game]->length,
                  sizeof(float) * STEPS_IN_RUN);
    bzero((char *)rls_seg->data[game]->dam_spill,
                  sizeof(float) * STEPS_IN_RUN);
    bzero((char *)rls_seg->data[game]->output_transport,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->transport_out)
            bzero((char *)rls_seg->data[game]->transport_out,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->transport_in)
            bzero((char *)rls_seg->data[game]->transport_in,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->transport_in_length)
            bzero((char *)rls_seg->data[game]->transport_in_length,
                  sizeof(float) * STEPS_IN_RUN);
#ifdef ROUTE_TRACKING
        if (rls_seg->data[game]->project_in)
            bzero((char *)rls_seg->data[game]->project_in,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->turbine_in)
            bzero((char *)rls_seg->data[game]->turbine_in,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->sluiceway_in)
            bzero((char *)rls_seg->data[game]->sluiceway_in,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->bypass_in)
            bzero((char *)rls_seg->data[game]->bypass_in,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->spillway_in)
            bzero((char *)rls_seg->data[game]->spillway_in,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->rsw_in)
            bzero((char *)rls_seg->data[game]->rsw_in,
                  sizeof(float) * STEPS_IN_RUN);

        if (rls_seg->data[game]->sluiceway_pass)
            bzero((char *)rls_seg->data[game]->sluiceway_pass,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->bypass_pass)
            bzero((char *)rls_seg->data[game]->bypass_pass,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->turbine_pass)
            bzero((char *)rls_seg->data[game]->turbine_pass,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->spillway_pass)
            bzero((char *)rls_seg->data[game]->spillway_pass,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->rsw_pass)
            bzero((char *)rls_seg->data[game]->rsw_pass,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->transport_pass)
            bzero((char *)rls_seg->data[game]->transport_pass,
                  sizeof(float) * STEPS_IN_RUN);
        if (rls_seg->data[game]->transport_arrived)
            bzero((char *)rls_seg->data[game]->transport_arrived,
                  sizeof(float) * STEPS_IN_RUN);

        if (rls_seg->data[game]->stats.d)
        {
                delayed_mortality_table_free(rls_seg->data[game]->stats.d);
                rls_seg->data[game]->stats.d = nullptr;
        }
#endif
        rls_seg = rls_seg->down;
    }
    return(1);
}
/** Find a release given the release site and species.
 *  \return A pointer to the release found, or 0
 */
/*struct release *find_release (struct release_site *site, struct species *species)
{
        struct release *rls;

        for (rls = current->release_list ; rls ; rls = rls->next)
        {
                if (site && rls->site != site) continue;
                if (species && rls->species != species) continue;
                return rls;
        }
        return 0;
}
/** Delete a release site and all subsequent sites */
/*void delete_release_site (struct release_site* rsite)
{
        if (rsite->next != nullptr)
                delete_release_site (rsite->next);
        rsite->next = nullptr;

        delete_all_points (rsite->latlon);
//	free (rsite->latlon);
        rsite->latlon = nullptr;

//	if (rsite->name)
//		if (strcmp(rsite->name, "McNary Tailrace"))
//		free (rsite->name);
        rsite->name->clear();
        delete rsite->name;
        free (rsite);
        rsite = nullptr;
}
/** Delete all release sites (uses global variable release_sites) */
/*void delete_release_sites ()
{
        for (int i = 0; i < release_sites.count(); i++)
                delete_release_site (release_sites[i]);
        release_sites.clear();
}*/

ReleaseSite::ReleaseSite (QString rsname)
{
    if (rsname.isEmpty ())
        name = nullptr;
    else
        name = new QString (rsname);
    latlon = new RiverPoint ();
}

ReleaseSite::~ReleaseSite ()
{
    if (name != nullptr)
        delete name;
    delete latlon;
}

QString *ReleaseSite::getName() const
{
    return name;
}

void ReleaseSite::setName(QString *value)
{
    name = value;
}

RiverPoint *ReleaseSite::getLatlon() const
{
    return latlon;
}

void ReleaseSite::setLatlon(RiverPoint *value)
{
    latlon = value;
}

void ReleaseSite::setSeg(Reach *value)
{
    seg = value;
}

Reach *ReleaseSite::getSeg() const
{
    return seg;
}

float ReleaseSite::getSeg_mile() const
{
    return seg_mile;
}

void ReleaseSite::setSeg_mile(float value)
{
    seg_mile = value;
}


PassageStats::PassageStats ()
{
    firstDay = 0;
    lastDay = 1;
    totFishIn = 0;
    totFishOut = 0;
    mean = 0;
    median = 0;
    mode = 0;
    std_dev = 0;
}
PassageStats::~PassageStats ()
{}


int PassageStats::getFirstDay() const
{
    return firstDay;
}

void PassageStats::setFirstDay(int value)
{
    firstDay = value;
}

int PassageStats::getLastDay() const
{
    return lastDay;
}

void PassageStats::setLastDay(int value)
{
    lastDay = value;
}

float PassageStats::getTotFishIn() const
{
    return totFishIn;
}

void PassageStats::setTotFishIn(float value)
{
    totFishIn = value;
}

float PassageStats::getTotFishOut() const
{
    return totFishOut;
}

void PassageStats::setTotFishOut(float value)
{
    totFishOut = value;
}

float PassageStats::getMean() const
{
    return mean;
}

void PassageStats::setMean(float value)
{
    mean = value;
}

float PassageStats::getMedian() const
{
    return median;
}

void PassageStats::setMedian(float value)
{
    median = value;
}

float PassageStats::getMode() const
{
    return mode;
}

void PassageStats::setMode(float value)
{
    mode = value;
}

float PassageStats::getStd_dev() const
{
    return std_dev;
}

void PassageStats::setStd_dev(float value)
{
    std_dev = value;
}
