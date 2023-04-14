#include "cmpdelayedmorttable.h"

#include "cmprelease.h"
#include "cmpreleasesegmentdata.h"

cmpDelayedMortColumn::cmpDelayedMortColumn()
{
    dam = nullptr;
}

cmpDelayedMortColumn::~cmpDelayedMortColumn()
{
    dam = nullptr;
    probPerDay.clear();
}

cmpDam *cmpDelayedMortColumn::getDam() const
{
    return dam;
}

void cmpDelayedMortColumn::setDam(cmpDam *newDam)
{
    dam = newDam;
}

cmpDelayedMortTable::cmpDelayedMortTable()
{
    transportTarget = QString();
}

cmpDelayedMortTable::cmpDelayedMortTable(QString transTarget, int game, cmpRelease *release)
{
    transportTarget = transTarget;
    if (release != nullptr)
        fill(game, release);
}

cmpDelayedMortTable::~cmpDelayedMortTable()
{

}

void cmpDelayedMortTable::fill(int game, cmpRelease *release)
{
    numCollectorDams = 0; /* Number of dams this release can pass through before getting to the delayed mortality calculation point */
    cmpReleaseSegmentData *rseg; /* Temporary for locating path */
    cmpDam *dam;
    bool passTransTarget = false;

    struct release_seg **dam_rsegs; /* Array of pointers to dam release segments so that data may be obtained from them */
    int dam_index; /* Index of the dam we are processing */

    /* Count dams */
    for (int i = 0, total = release->getRelSegments().count(); i < total; i++)
    {
        rseg = release->getRelSegments().at(i);
        if (rseg->getRivSeg()->getName() == transportTarget)
        {
            passTransTarget = true;
            break;
        }
        if (rseg->getRivSeg()->getType() == cmpRiverSegment::Dam)
        {
            dam = static_cast<cmpDam *>(rseg->getRivSeg());
            if(dam->getTransport() != nullptr)
            {
            /* Check to make sure that the proper tracking data exists, otherwise this can't be properly calculated */
                if (rseg->getBypassIn().isEmpty() || rseg->getTransportArrived().isEmpty())
                {
                    // output error "Tracking is not turned on at %1, so delayed mortality calculations could not be performed!\n"
                    return;
                }
                numCollectorDams++;
            }
        }
    }
    if (!passTransTarget)
    {
//        log_msg(L_ERROR, QString("Error calculating delayed mortality. The selected release doesn't pass through the transport destination\n"));
        return;
    }
/*
    if (*d)
    {
        delayed_mortality_table_free(*d);
        *d = NULL;
    }
*/
    /* Get pointers to the dam release segments */
/*    dam_rsegs = (struct release_seg**)calloc (num_collector_dams, sizeof(struct release_seg*));
    for (dam_index=0, rseg = release->rls_seg; rseg && rseg->riv_seg != default_transport_target; rseg = rseg->down)
    {
        if (rseg->riv_seg->seg_type == DAM_SEG && rseg->riv_seg->seg_info.dam.collector) dam_rsegs[dam_index++] = rseg;
    }

*/
    /* Construct the table with a column for each dam and for each possible number of bypass experiences */
/*    *d = (delayed_mortality_table*) calloc (1, sizeof(delayed_mortality_table));
    if (!(*d))
        return;

    (*d)->num_collector_dams = num_collector_dams;
*/
    /* Note that all values are initialized to zero */
/*    (*d)->columns = (struct delayed_mortality_column*) calloc (num_collector_dams, sizeof(struct delayed_mortality_column));
    if (!(*d)->columns)
        return;
    for (int i = 0; i < num_collector_dams; i++)
        (*d)->columns[i].prob = (double *) calloc (DAYS_IN_SEASON, sizeof(double));
*/
    /* Fill in the dam columns (the data is indexed by model step, so it must be
     * accumulated to get values for an entire day). */
/*    for (dam_index=0; dam_index < num_collector_dams; dam_index++)
    {
        int day;
*/
        /* Only do these calculations if this dam is transporting, otherwise all values are zero */
/*        if (dam_rsegs[dam_index]->riv_seg->seg_info.dam.transport)
        {
            for (day=0; day < DAYS_IN_SEASON; day++)
            {
                double transport_arrived = 0.0;
                double total = 0.0;
                int step_in_day;
*/
                /* Accumulate over a day */
/*                for (step_in_day=0; step_in_day < STEPS_PER_DAY; step_in_day++)
                {
                    transport_arrived += dam_rsegs[dam_index]->data->at(game)->transport_arrived[day*STEPS_PER_DAY + step_in_day];
                    total += dam_rsegs[dam_index]->data->at(game)->output[day*STEPS_PER_DAY + step_in_day] + dam_rsegs[dam_index]->data->at(game)->transport_arrived[day*STEPS_PER_DAY + step_in_day];
                }*/

                /* Final calculation for a single cell */
/*                if (total > 0)
                    (*d)->columns[dam_index].prob[day] = transport_arrived / total;
            }
        }
    }
*/
    /* Fill in inriver columns */
//    {
        /* Store bypass probabilities once so they are not recalculated */
//        double *bypass_probs = (double*) calloc(num_collector_dams, sizeof(double));

        /* Calculate bypass probabilities averaged over the season */
//        for (dam_index=0; dam_index < num_collector_dams; dam_index++)
//        {
            /* Obtained an average bypass probability weighted by passage through the dam */
/*            const struct release_seg_data *data = dam_rsegs[dam_index]->data->at(game);
            int step;
            double passage_total = 0.0;
            double sum = 0.0;

            for (step=0; step < STEPS_IN_SEASON; step++)
            {
                const double passage = data->output[step];
                passage_total += passage;
                if (passage > 0) sum += passage * (data->bypass_pass[step]/passage);
            }

            if (passage_total > 0)
                bypass_probs[dam_index] = sum / passage_total;
        }
*/
        /* Calculate number of bypass probabilities */
/*        (*d)->prob_num_bypasses = (double*) calloc(num_collector_dams + 1, sizeof(double));

        calculate_spill_experience((*d)->prob_num_bypasses, dam_rsegs, bypass_probs, game, num_collector_dams, 1.0, 0, 0);

        free(bypass_probs);
    }

    free(dam_rsegs);
*/
}

int cmpDelayedMortTable::getNumCollectorDams() const
{
    return numCollectorDams;
}

void cmpDelayedMortTable::setNumCollectorDams(int newNumCollectorDams)
{
    numCollectorDams = newNumCollectorDams;
}

const QString &cmpDelayedMortTable::getTransportTarget() const
{
    return transportTarget;
}

void cmpDelayedMortTable::setTransportTarget(const QString &newTransportTarget)
{
    transportTarget = newTransportTarget;
}
