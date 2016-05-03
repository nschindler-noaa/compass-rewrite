#ifndef C_SPECIES_H
#define C_SPECIES_H

#include "Equation.h"
#include "Stock.h"

/** \brief Species parameters.
 *
 * Major species groups are:
 * - Chinook 0 (sub-yearlings)
 * - Chinook 1 (yearlings)
 * - Coho
 * - Sockeye
 * - Steelhead
 *//*
 * All float arrays are dimensioned by number of \ref reach_classes.
 */

class Species
{
public:
    Species (QString name = QString (""));
    ~Species ();

    QString *name;               /**< Name of species */

    Stock stock;             /**< Generic stock params this species */
/*    float reach_pcoef[10];    *< Reach predation coefficients */
/*    float pprime_a[10];       *< Used to calculate 'p' for growth alg. */
/*    float pprime_b[10];       *< Used to calculate 'p' for growth alg. */
/*    float tailrace_pcoef;     * Used in dam predation mortality. */
/*    float forebay_pcoef;      * Used in dam predation mortality. */
    /* even newer gmort stuff */
/*    equation gmort_eqn;       * Gas bubble disease mortality equation */
/*    equation fdens_eqn;       * Used in gas mortality calculations */

    float inriver_latent_mort;/**< Latent mortality for post-Bonneville
                               * calculations (inriver) */
    float transport_latent_mort;/**< Latent mortality for post-Bonneville
                               * calculations (transported) */

    /** Differential return for post-Bonneville calculations */
    float differential_return;

    /** This equation is used to estimate latent mortality of inriver fish
     *  and is used in the S3 vs WTT post-Bonneville algorithm */
    Equation inriver_latent_mort_eqn;

    /* any other parameters of the model that might vary by species */
};


#endif // C_SPECIES_H
