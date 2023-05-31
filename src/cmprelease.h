#ifndef CMPRELEASE_H
#define CMPRELEASE_H

#include "cmpstock.h"
#include "cmpspecies.h"
#include "cmpreleasesite.h"
#include "cmpreleasesegmentdata.h"
#include "cmprealtime.h"
#include "cmpfile.h"

#include <QStringList>

class cmpRelease
{
public:
    cmpRelease();
    ~cmpRelease();

    void reset();

    const QString &getName() const;
    void setName(const QString &newName);

    void activate (bool newActive = true);
    int resetSegmentData ();
    int computeSegments ();

    bool parseData(cmpFile *cfile);
    bool parseToken(QString token, cmpFile*cfile);
    void writeData(cmpFile *ofile, bool outputAll);

    cmpReleaseSite *getSite ();
    void setSite(cmpReleaseSite *newSite);
    cmpReleaseSite *getDestination ();
    void setDestination(cmpReleaseSite *newSite);

    void setSpecies(cmpSpecies *newSpecies);

    void setStock(cmpStock *newStock);
    void setStockName(QString stkName);

    int getStartDay() const;
    void setStartDay(int newStartDay);

    RtInfo *getRtinfo() const;
    void setRtinfo(RtInfo *newRtinfo);

    const float &getNumber(int i);
    void setNumber(int i, const float &newNumber);

    int getAddSetting() const;
    void setAddSetting(int newAddSetting);

    bool getDirtyFlag() const;
    void setDirtyFlag(bool newDirtyFlag);

    float getInitialSpillExperience() const;
    void setInitialSpillExperience(float newInitialSpillExperience);

    float calculateTotalReleased();
    float getTotalReleased() const;
    void setTotalReleased(float newTotalReleased);

    const QList<cmpReleaseSegmentData *> &getRelSegments() const;
    void setRelSegments(const QList<cmpReleaseSegmentData *> &newRelSegments);

    const cmpReleaseSegmentStats &getStats() const;
    void setStats(const cmpReleaseSegmentStats &newStats);

    float getMigrOnsetMedian() const;
    void setMigrOnsetMedian(float newMigrOnsetMedian);

    const QString &getStockName() const;

    float getFishLength() const;
    void setFishLength(float newFishLength);

    const QString &getDestSiteName() const;
    void setDestSiteName(const QString &newDestSiteName);

private:
    QString name;
    bool active;
    cmpReleaseSite *relsite;
    QString destSiteName;
    cmpReleaseSite *destination;
    cmpSpecies *species;
    QString stockName;
    cmpStock *stock;
    float fishLength;
    int startDay;        /**< Offset into season */
    QList<float> number; /**< The release distribution up to [days_per_season] - startDay*/
    float curNumber;
//#ifdef REALTIME
    RtInfo *rtinfo;               /**< Realtime info (optional) */
//#endif

    int   addSetting;       /**< For determining how to add the release to the
                                                           * system. */
    bool  dirtyFlag;        /**< For use by GUI release editor. */

    float initialSpillExperience;/**< If the release is made in a segment
                               * that isn't immediately downstream from a dam,
                               * then the dam spill experience will be filled
                               * in with this value */
    float totalReleased;    /**< Total fish in last run */
    QList<cmpReleaseSegmentData *> relSegments; /** List of release segments (that hold data and stats) */
    cmpReleaseSegmentStats stats; /**< Summary statistics for this release */
    float migrOnsetMedian;  /**< Migration onset, median value
                                *  in days (computed) */
};

#endif // CMPRELEASE_H
