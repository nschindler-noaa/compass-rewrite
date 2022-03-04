#ifndef CMPSTOCK_H
#define CMPSTOCK_H

#include "cmpspecies.h"
#include "cmpsettings.h"
#include "cmpequation.h"
#include "cmpmontecarlomulti.h"

#include <QStringList>

/**
 * \class stock
 * The general design is that there are a number of custom stocks,
 * designated in the river description file, the data for which are
 * gathered in the global stock_info struct below.  in addition, each
 * species contains one Stock object, which is "generic" data for that
 * species.

 * The key usage is that each release keeps a handle to one Stock
 * object, either one of the custom globals or the generic from the
 * appropriate species for that release.
 *
 * equation and float arrays are dimensioned by number of
 * \ref reach_classes. */


class cmpStock : public cmpSpecies
{
public:
    cmpStock();
    ~cmpStock();

    void allocate(int steps, int numReachClasses);
    void setDefaults();

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

    cmpEquation *getInriverReturnEqn() const;
    void setInriverReturnEqn(cmpEquation *newInriverReturnEqn);

    cmpEquation *getTransportReturnEqn() const;
    void setTransportReturnEqn(cmpEquation *newTransportReturnEqn);

    float getReachSurvivalCoef(int rc) const;
    void setReachSurvivalCoef(int rc, float newReachSurvivalCoef);

    const cmpMonteCarloMulti *getCovmat(int rc) const;
    void setCovmat(int rc, cmpMonteCarloMulti *newCovmat);

private:
    QString  *name;              /**< Name of the stock */
    QList<cmpEquation *> migrationEqn; /**< migration equation (number of reach classes) */
    QList<float>     mvCoef;         /**< migration variance (number of reach classes) */
    QList<float>     distanceCoeff; /**< "a" in "sqrt( a * x^2 + b * t^2 )" (number of reach classes) */
    QList<float>     timeCoeff;     /**< "b" in "sqrt( a * x^2 + b * t^2 )" (number of reach classes) */

    QList<float>     sigmaD;        /**< reach survival error parameter (number of reach classes) */
    QList<float>     procStdDev;       /**< reach survival process variation (number of reach classes) */

    QList<QList<float>> migrB1Factor;    /**< migration factor  (steps in season)(number of reach classes) */
    QList<float>     vvar;           /**< Herterogeneity of species - travel time distribution (number of reach classes) */

    /** This equation is used with the CUSTOM_CLASS mortality class so that
     * additional X-T-Theta models may be implemented and used easily. */
    QList<cmpEquation *> customSurvivalEqn;

    /* These equations are used to estimate a return rate for adults based on
     * arrival timing at the transport destination (i.e. below Bonneville) */
    cmpEquation  *inriverReturnEqn; /**< return rate for inriver fish */
    cmpEquation  *transportReturnEqn;/**< return rate for transported fish */

    /* COMPASS reservoir survival model stuff */
    /** Reach survival coefficient (alpha)  (number of reach classes)*/
    QList<float>     reachSurvivalCoef;

    /** This is a pointer to a covariant matrix used in Monte Carlo mode to
     *  create variation in the custom survival equation (number of reach classes). */
    QList<cmpMonteCarloMulti*>   covmat;
    QList<cmpEquation *> copySurvivalEqn; /**< A copy of each reach class survival equation
                                     *  is needed to restore the mean after variation (number of reach classes). */
};

#endif // CMPSTOCK_H
