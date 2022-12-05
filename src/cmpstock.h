#ifndef CMPSTOCK_H
#define CMPSTOCK_H

#include "cmpspecies.h"
#include "cmpcommandsettings.h"
#include "cmpequation.h"
#include "cmpmontecarlomulti.h"
#include "cmpreachclass.h"
#include "cmpfile.h"

#include <QStringList>

/**
 * \class cmpStock
 * A cmpStock is a cmpSpecies that includes data for a specific stock.
 * This includes a list of cmpReachClass that indicate behavior of the
 * stock in groupings of reaches.
 *
 * A release includes a name of its stock.
 *
 */


class cmpStock : public cmpSpecies
{
public:
    cmpStock();
    ~cmpStock();

    void allocate(int steps, int numReachClasses);
    void setDefaults();

    void parseData(cmpFile *cfile);

    QString *getName() const;
    void setName(QString *newName);

    const cmpEquation *getMigrationEqn(int rc) const;
    void setMigrationEqn(int rc, cmpEquation *newMigrationEqn);

    float getMvCoef(int rc) const;
    void setMvCoef(int rc, float newMvCoef);

    float getDistanceCoeff(int rc) const;
    void setDistanceCoeff(int rc, float newDistanceCoeff);

    float getTimeCoeff(int rc) const;
    void setTimeCoeff(int rc, float newTimeCoeff);

    float getSigmaD(int rc) const;
    void setSigmaD(int rc, float newSigmaD);

    float getProcStdDev(int rc) const;
    void setProcStdDev(int rc, float newProcStdDev);

    float getMigrB1Factor(int rc, int step) const;
    void setMigrB1Factor(int rc, int step, float newMigrB1Factor);

    float getVvar(int rc) const;
    void setVvar(int rc, float newVvar);

    cmpEquation *getCustomSurvivalEqn(int rc) const;
    void setCustomSurvivalEqn(int rc, cmpEquation *newCustomSurvivalEqn);

    float getReachSurvivalCoef(int rc) const;
    void setReachSurvivalCoef(int rc, float newReachSurvivalCoef);

    const cmpMonteCarloMulti *getCovmat(int rc) const;
    void setCovmat(int rc, cmpMonteCarloMulti *newCovmat);

private:
    QString  *name;              /**< Name of the stock */
    QList<cmpReachClass *> reachClasses; /**< Reach classes for this stock. */

};

#endif // CMPSTOCK_H
