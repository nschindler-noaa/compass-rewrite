#ifndef CMPSPECIES_H
#define CMPSPECIES_H

#include "cmpcommandsettings.h"
#include "cmpequation.h"
#include "cmpreachclass.h"
#include "cmpfile.h"

#include <QStringList>

/** \class cmpSpecies
 *  \brief Species parameters.
 *
 * Major species groups are:
 * - Chinook 0 (sub-yearlings)
 * - Chinook 1 (yearlings)
 * - Coho
 * - Sockeye
 * - Steelhead.
 *
 * All float arrays are dimensioned by number of \ref reach_classes.
 */

class cmpSpecies
{
public:
    cmpSpecies();
    cmpSpecies(const cmpSpecies &rhs);
    ~cmpSpecies();

    void copy(const cmpSpecies &rhs);

    const QString &getName() const;
    void setName(const QString &newName);
    void setDefaults();

    bool parseData(cmpFile *cfile);
    bool readReachClassValue(QString &newString, int &rc, float &value);
    void writeData(cmpFile *outfile, int indent, bool outputAll);
    void writeReachClassData(cmpFile *outfile, int indent, bool outputAll);
    void writeSpeciesData(cmpFile *outfile, int indent, bool outputAll);
    void writeFishReturnEqns(cmpFile *outfile, int indent, bool outputAll);

    void allocateClasses(int numReachClasses);
    void allocate(int steps, int numReachClasses);

    const QStringList &getReachClassNames() const;
    void setReachClassNames(const QStringList &newReachClassNames);
    const QString &getReachClassName (int rc) const;
    void setReachClassName(int rc, QString &newName);

    const cmpReachClass *getReachClass(int rc) const;
    void setReachClass(int rc, cmpReachClass *newReachClass);

    const cmpEquation *getMigrationEqn(int rc) const;
    void setMigrationEqn(int rc, cmpEquation *newMigrationEqn);

    float getMigrVarCoef(int rc) const;
    void setMigrVarCoef(int rc, float newMvCoef);

    float getDistCoeff(int rc) const;
    void setDistCoeff(int rc, float newDistanceCoeff);

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

    float getReachPredCoef(int rc) const;
    void setReachPredCoef(int rc, float newReachPredCoef);

    float getPprimeA(int rc) const;
    void setPprimeA(int rc, float newPprimeA);

    float getPprimeB(int rc) const;
    void setPprimeB(int rc, float newPprimeB);

    float getTailracePredCoef() const;
    void setTailracePredCoef(float newTailracePredCoef);

    float getForebayPredCoef() const;
    void setForebayPredCoef(float newForebayPredCoef);

    cmpEquation *getGasmortEqn() const;
    void setGasmortEqn(cmpEquation *newGasmortEqn);

    cmpEquation *getFishdensEqn() const;
    void setFishdensEqn(cmpEquation *newFishdensEqn);

    float getInriverLatentMort() const;
    void setInriverLatentMort(float newInriverLatentMort);

    float getTransportLatentMort() const;
    void setTransportLatentMort(float newTransportLatentMort);

    float getDifferentialReturn() const;
    void setDifferentialReturn(float newDifferentialReturn);

    cmpEquation *getInriverReturnEqn() const;
    void setInriverReturnEqn(cmpEquation *newInriverReturnEqn);

    cmpEquation *getTransportReturnEqn() const;
    void setTransportReturnEqn(cmpEquation *newTransportReturnEqn);

    cmpEquation *getInriverLatentMortEqn() const;
    void setInriverLatentMortEqn(cmpEquation *newInriverLatentMortEqn);

protected:
    QString name;               /**< Name of species/cohort */
    QStringList reachClassNames;/**< list of reach class names */
    QList<cmpReachClass *> reachClasses; /**< list of reach classes */

    float tailracePredCoef;     /**< Used in dam predation mortality */
    float forebayPredCoef;      /**< Used in dam predation mortality */

    cmpEquation *gasMortEqn;    /**< Gas bubble disease mortality equation */
    cmpEquation *fishDepthEqn;  /**< Fish density, used in gas mortality calculations */

    // return (ocean survival)
    float inriverLatentMort;    /**< Latent mortality for post-Bonneville
                                 * calculations (inriver) */
    float transportLatentMort;  /**< Latent mortality for post-Bonneville
                                 * calculations (transported) */

    /** Differential return for post-Bonneville calculations */
    float differentialReturn;

    /* These equations are used to estimate a return rate for adults based on
     * arrival timing at the transport destination (i.e. ocean survival) */
    cmpEquation  *inriverReturnEqn; /**< return rate for inriver fish */
    cmpEquation  *transportReturnEqn;/**< return rate for transported fish */

    /** This equation is used to estimate latent mortality of inriver fish
     *  and is used in the S3 vs WTT post-Bonneville algorithm */
    cmpEquation *inriverLatentMortEqn;

};

//typedef cmpSpecies cmpStock ;

#endif // CMPSPECIES_H
