#ifndef C_STOCK_H
#define C_STOCK_H
/** \file stock.h
 *  \brief Defines the data structure for stocks (groups of fish from a common
 *         release point with similar characteristics).
 */
#include <QString>

#include "definitions.h"
#include "Equation.h"
//#include "var.h"

/**
 * \struct stock
 * The general design is that there are a number of custom stocks,
 * designated in the river description file, the data for which are
 * gathered in the global stock_info struct below.  in addition, each
 * species contains one Stock object, which is "generic" data for that
 * species.

 * The key usage is that each release keeps a handle to one Stock
 * object, either one of the custom globals or the generic from the
 * appropriate species for that release.
 *
 * equation []'s and float []'s are dimensioned by number of
 * \ref reach_classes. */



/** Default values for Stock. These disable as many features as possible
 * and when an input file is written out, only values that differ from
 * these defaults are written out. Thus, old parameters that are
 * generally disabled are hidden. */

class Stock
{
public:
    Stock();
    ~Stock();
    QString  *name;      /**< Name of the stock */
    Equation  migration_eqn[10];/**< migration equation */
    float    mvcoef[10];		/**< migration variance */
    float    distance_coeff[10];  /**< "a" in "sqrt( a * x^2 + b * t^2 )" */
    float    time_coeff[10];      /**< "b" in "sqrt( a * x^2 + b * t^2 )" */

    float    sigma_d[10];	/**< reach survival error parameter */

    float    *migr_b1fac[10]; /**< [num_reach_classes][STEPS_IN_SEASON] */
    float    vvar[10];   /**< Herterogeneity of species - travel time distribution */

    /** This equation is used with the CUSTOM_CLASS mortality class so that
     * additional X-T-Theta models may be implemented and used easily. */
    Equation  custom_survival_eqn[10];

    Equation  pred_temp_response_eqn[10];

    /* These equations are used to estimate a return rate for adults based on
     * arrival timing at the transport destination (i.e. below Bonneville) */
    Equation  inriver_return_eqn; /**< return rate for inriver fish */
    Equation  transport_return_eqn;/**< return rate for transported fish */

    /* COMPASS reservoir survival model stuff */
    /** Reach survival coefficient (alpha) */
    float    reach_survival_coef[10];
    /* Reach survival time coefficient (bee) */
    /* float	time_coef[10]; */ /* Already in species for XT model */
    /* Predator capture distance equation (gee of turbidity)
    equation  pred_capture_dist_eqn[10]; */

};

//extern Stock stock_defaults;
int float_arrs_equal (float *flptr1, float *flptr2, int num);

#endif // C_STOCK_H
