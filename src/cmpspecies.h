#ifndef CMPSPECIES_H
#define CMPSPECIES_H

#include "cmpcommandsettings.h"
#include "cmpequation.h"
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

    const QString &getName() const;
    void setName(const QString &newName);
    void allocate(int numReachClasses);
    void setDefaults();

    bool parseData(cmpFile *cfile);

    const QList<float> &getReachPredCoef() const;
    void setReachPredCoef(const QList<float> &newReachPredCoef);

    const QList<float> &getPprimeA() const;
    void setPprimeA(const QList<float> &newPprimeA);

    const QList<float> &getPprimeB() const;
    void setPprimeB(const QList<float> &newPprimeB);

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

    float getReachSurvivalCoef(int rc) const;
    void setReachSurvivalCoef(int rc, float newReachSurvivalCoef);

    cmpEquation *getInriverLatentMortEqn() const;
    void setInriverLatentMortEqn(cmpEquation *newInriverLatentMortEqn);

    const QStringList &getReachClassNames() const;
    void setReachClassNames(const QStringList &newReachClassNames);

protected:
    QString name;               /**< Name of species/cohort */
    QStringList reachClassNames; /**< reference list of reach class names */

    QList<float> reachPredCoef; /**< Reach predation coefficients */
    QList<float> pprimeA;       /**< Used to calculate 'p' for growth calculations */
    QList<float> pprimeB;       /**< Used to calculate 'p' for growth calculations */
    float tailracePredCoef;     /**< Used in dam predation mortality */
    float forebayPredCoef;      /**< Used in dam predation mortality */

    cmpEquation *gasmortEqn;    /**< Gas bubble disease mortality equation */
    cmpEquation *fishdensEqn;   /**< Fish density, used in gas mortality calculations */

    float inriverLatentMort;    /**< Latent mortality for post-Bonneville
                                 * calculations (inriver) */
    float transportLatentMort;  /**< Latent mortality for post-Bonneville
                                 * calculations (transported) */

    /** Differential return for post-Bonneville calculations */
    float differentialReturn;

    /* These equations are used to estimate a return rate for adults based on
     * arrival timing at the transport destination (i.e. below Bonneville) */
    cmpEquation  *inriverReturnEqn; /**< return rate for inriver fish */
    cmpEquation  *transportReturnEqn;/**< return rate for transported fish */

    /** This equation is used to estimate latent mortality of inriver fish
     *  and is used in the S3 vs WTT post-Bonneville algorithm */
    cmpEquation *inriverLatentMortEqn;

};

#endif // CMPSPECIES_H
