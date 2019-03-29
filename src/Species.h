#ifndef C_SPECIES_H
#define C_SPECIES_H

#include "Equation.h"
#include "Stock.h"

/** \brief Species parameters. This definition includes default or generic
 *  Stock values.
 *
 * Major species groups are:
 * - Chinook 0 (sub-yearlings)
 * - Chinook 1 (yearlings)
 * - Coho
 * - Sockeye
 * - Steelhead
 *
 *
 * All float arrays are dimensioned by number of \ref reach classes.
 */

class Species : public Stock
{
public:
    Species (QString name = QString (""));
    ~Species ();

    void allocate (int num); /**< allocate enough room for all reach classes*/

    Equation getInriverLatentMortEqn() const;
    void setInriverLatentMortEqn(const Equation &value);

    float getDifferentialReturn() const;
    void setDifferentialReturn(float value);

    float getTransportLatentMort() const;
    void setTransportLatentMort(float value);

    float getInriverLatentMort() const;
    void setInriverLatentMort(float value);

    Equation getFishDensEqn() const;
    void setFishDensEqn(const Equation &value);

    Equation getGasMortEqn() const;
    void setGasMortEqn(const Equation &value);

    float getForebayPredCoef() const;
    void setForebayPredCoef(float value);

    float getTailracePredCoef() const;
    void setTailracePredCoef(float value);

    float getPprimeB(int index) const;
    void setPprimeB(int index, const float &value);

    float getPprimeA(int index) const;
    void setPprimeA(int index, const float &value);

    float getReachPredCoef(int index) const;
    void setReachPredCoef(int index, const float &value);

protected:
    //    QString *name;               /**< Name of species */
    //    Stock stock;             /**< Generic stock params this species */
    QList<float> reachPredCoef;    /**< Reach predation coefficients [10]*/
    QList<float> pprimeA;       /**< Used to calculate 'p' for growth algorithm. [10]*/
    QList<float> pprimeB;       /**< Used to calculate 'p' for growth algorithm. [10]*/
    float tailracePredCoef;     /**< Used in dam predation mortality. */
    float forebayPredCoef;      /**< Used in dam predation mortality. */
    /* even newer gmort stuff */
    Equation gasMortEqn;       /**< Gas bubble disease mortality equation */
    Equation fishDensEqn;       /**< Used in gas mortality calculations */

    float inriverLatentMort;/**< Latent mortality for post-Bonneville
                               * calculations (inriver) */
    float transportLatentMort;/**< Latent mortality for post-Bonneville
                               * calculations (transported) */

    /** Differential return for post-Bonneville calculations */
    float differentialReturn;

    /** This equation is used to estimate latent mortality of inriver fish
     *  and is used in the S3 vs WTT post-Bonneville algorithm */
    Equation inriverLatentMortEqn;

    /* any other parameters of the model that might vary by species */
};


#endif // C_SPECIES_H
