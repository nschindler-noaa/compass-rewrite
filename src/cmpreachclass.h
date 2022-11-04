#ifndef CMPREACHCLASS_H
#define CMPREACHCLASS_H

#include "cmpequation.h"
#include "cmpmontecarlomulti.h"

#include <QStringList>

class cmpReachClass
{
public:
    cmpReachClass(const QString &newName = QString());
    ~cmpReachClass();

    void setDefaults();
    void restoreMeanValues();
    void allocate(int steps);
    void deleteEqns();

    cmpEquation *getMigrationEqn() const;
    void setMigrationEqn(cmpEquation *newMigrationEqn);

    float getMvCoef() const;
    void setMvCoef(float newMvCoef);

    float getDistanceCoeff() const;
    void setDistanceCoeff(float newDistanceCoeff);

    float getTimeCoeff() const;
    void setTimeCoeff(float newTimeCoeff);

    float getSigmaD() const;
    void setSigmaD(float newSigmaD);

    float getProcStdDev() const;
    void setProcStdDev(float newProcStdDev);

    float getVvar() const;
    void setVvar(float newVvar);

    float getMigrB1Factor(int index) const;
    void setMigrB1Factor(int index, float value);
    const QList<float> &getMigrB1Factors() const;
    void setMigrB1Factors(const QList<float> &newMigrB1Factor);

    cmpEquation *getCustomSurvivalEqn() const;
    void setCustomSurvivalEqn(cmpEquation *newCustomSurvivalEqn);

    float getReachSurvivalCoef() const;
    void setReachSurvivalCoef(float newReachSurvivalCoef);

    cmpMonteCarloMulti *getCovmat() const;
    void setCovmat(cmpMonteCarloMulti *newCovmat);

    cmpEquation *getCopySurvivalEqn() const;
    void setCopySurvivalEqn(cmpEquation *newCopySurvivalEqn);

    QString *getName() const;
    void setName(QString *newName);

private:
    QString *name;           /**< Reach Class name. */
    cmpEquation *migrationEqn; /**< migration equation */
    float     mvCoef;        /**< migration variance  */
    float     distanceCoeff; /**< "a" in "sqrt( a * x^2 + b * t^2 )"  */
    float     timeCoeff;     /**< "b" in "sqrt( a * x^2 + b * t^2 )"  */

    float     sigmaD;        /**< reach survival error parameter  */
    float     procStdDev;    /**< reach survival process variation  */

    QList<float> migrB1Factor; /**< migration factor  (steps in season) */
    float     vvar;          /**< Herterogeneity of species - travel time distribution  */

    /** This equation is used with the CUSTOM_CLASS mortality class so that
     * additional X-T-Theta models may be implemented and used easily. */
    cmpEquation *customSurvivalEqn;

    /* COMPASS reservoir survival model stuff */
    /** Reach survival coefficient (alpha)  */
    float     reachSurvivalCoef;

    /** This is a pointer to a covariant matrix used in Monte Carlo mode to
     *  create variation in the custom survival equation. */
    cmpMonteCarloMulti*   covmat;
    /**< This is a copy of the reach class survival equation that
     *  is needed to restore the mean values after variation. */
    cmpEquation *copySurvivalEqn;

};

#endif // CMPREACHCLASS_H
