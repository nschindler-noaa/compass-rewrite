#ifndef CMPDELAYEDMORTTABLE_H
#define CMPDELAYEDMORTTABLE_H

#include "cmpdam.h"

#include <QStringList>

class cmpRelease;

/* \file
 * \brief Defines the interface for delayed mortality-related calculations
 *  (namely the delayed mortality table) */

/** Data column for delayed mortality table */
class cmpDelayedMortColumn
{
public:
    cmpDelayedMortColumn();
    ~cmpDelayedMortColumn();

    cmpDam *getDam() const;
    void setDam(cmpDam *newDam);

private:
    /** Pointer to the dam */
    cmpDam *dam;
    /** Probability of being transported in on a given day from a given dam */
    QList<double> probPerDay;  //[DAYS_IN_SEASON]

};


/** Delayed mortality table which can be used in calculations and output */
class cmpDelayedMortTable
{
public:
    cmpDelayedMortTable();
    cmpDelayedMortTable(QString transTarget, int game, cmpRelease *release);
    ~cmpDelayedMortTable();

    void fill(int game, cmpRelease *release);

    int getNumCollectorDams() const;
    void setNumCollectorDams(int newNumCollectorDams);

    const QString &getTransportTarget() const;
    void setTransportTarget(const QString &newTransportTarget);

private:
    int numCollectorDams; /**< Number of collector dams */

    /** For each collector dam, set up a data column */
    QList<cmpDelayedMortColumn *> columns;

    /** Indexed by the number of bypasses, this is the probability over the
     * whole season of inriver fish experiencing this number of
     * bypasses. */
    double *probNumBypasses;
    QString transportTarget;

};




//** Construct and fill a delayed mortality table */
//void create_delayed_mortality_table (\
//	struct release *release, delayed_mortality_table **d, int game);
//** Delete a delayed mortality table */
//void delayed_mortality_table_free (delayed_mortality_table *d);


#endif // CMPDELAYEDMORTTABLE_H
