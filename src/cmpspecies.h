#ifndef CMPSPECIES_H
#define CMPSPECIES_H

#include "cmpsettings.h"
#include "cmpequation.h"

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

    cmpEquation *getInriverLatentMortEqn() const;
    void setInriverLatentMortEqn(cmpEquation *newInriverLatentMortEqn);

private:
    QString name;               /**< Name of species/cohort */

    QList<float> reachPredCoef;    /**< Reach predation coefficients */
    QList<float> pprimeA;       /**< Used to calculate 'p' for growth calculations */
    QList<float> pprimeB;       /**< Used to calculate 'p' for growth calculations */
    float tailracePredCoef;     /**< Used in dam predation mortality */
    float forebayPredCoef;      /**< Used in dam predation mortality */

    cmpEquation *gasmortEqn;       /**< Gas bubble disease mortality equation */
    cmpEquation *fishdensEqn;       /**< Fish density, used in gas mortality calculations */

    float inriverLatentMort;/**< Latent mortality for post-Bonneville
                                * calculations (inriver) */
    float transportLatentMort;/**< Latent mortality for post-Bonneville
                                * calculations (transported) */

    /** Differential return for post-Bonneville calculations */
    float differentialReturn;

    /** This equation is used to estimate latent mortality of inriver fish
     *  and is used in the S3 vs WTT post-Bonneville algorithm */
    cmpEquation *inriverLatentMortEqn;

};

#endif // CMPSPECIES_H
