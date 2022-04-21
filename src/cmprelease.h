#ifndef CMPRELEASE_H
#define CMPRELEASE_H

#include "cmpstock.h"
#include "cmpspecies.h"
#include "cmpreleasesite.h"
#include "cmpreleasesegmentdata.h"
#include "cmprealtime.h"

#include <QStringList>

class cmpRelease
{
public:
    cmpRelease();

    void activate (bool = true);


    QString name;
    cmpReleaseSite *site;
    cmpSpecies *species;
    cmpStock *stock;
    int startDay;        /**< Offset into season */
    QList<float> number; /**< The release distribution up to [Days_IN_SEASON] - startDay*/
#ifdef REALTIME
    RtInfo *rtinfo;               /**< Realtime info (optional) */
#endif

    int   addSetting;       /**< For determining how to add the release to the
                                                           * system. */
    int   dirtyFlag;        /**< For use by GUI release editor. */

    float initialSpillExperience;/**< If the release is made in a segment
                               * that isn't immediately downstream from a dam,
                               * then the dam spill experience will be filled
                               * in with this value */
    float totalReleased;  /**< Total fish in last run */
    QList<cmpReleaseSegmentData *> relSegments; /** List of release segments (hold data and stats) */
    cmpReleaseStats stats; /**< Summary statistics for this release */
    float migrOnsetMedian;/**< Migration onset, median value
                                *  in days (computed) */
};

#endif // CMPRELEASE_H
