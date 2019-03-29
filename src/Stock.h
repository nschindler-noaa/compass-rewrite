#ifndef C_STOCK_H
#define C_STOCK_H
/** \file stock.h
 *  \brief Defines the data structure for stocks (groups of fish from a common
 *         release point with similar characteristics).
 */
#include <QString>
#include <QList>

#include "definitions.h"
#include "Equation.h"
//#include "var.h"

/**
 * \class Stock
 * The general design is that there are a number of custom stocks,
 * designated in the river description file, the data for which are
 * gathered in the global stockInfo struct below.  in addition, each
 * species contains one Stock object, which is "generic" data for that
 * species.

 * The key usage is that each release keeps a handle to one Stock
 * object, either one of the custom globals or the generic from the
 * appropriate species for that release.
 *
 * equations and floats are dimensioned by number of
 * \ref reach_classes. */



/** Default values for Stock are kept in Species defaults. These disable as many features as possible
 * and when an input file is written out, only values that differ from
 * these defaults are written out. Thus, old parameters that are
 * generally disabled are hidden. */

class Stock
{
public:
    Stock(QString &nm);
    ~Stock();

    void allocate (int num);
    void deleteAll ();

    QString &getName() const;
    void setName(QString &value);

    Equation * getMigrationEqn(int index) const;
    void setMigrationEqn(int index, Equation *&value);

    float getMvcoef(int index) const;
    void setMvcoef(int index, const float &value);

    float getDistanceCoeff(int index) const;
    void setDistanceCoeff(int index, const float &value);

    float getTimeCoeff(int index) const;
    void setTimeCoeff(int index, const float &value);

    float getSigmaD(int index) const;
    void setSigmaD(int index, const float &value);

    float getMigrB1Factor(int index, int step) const;
    void setMigrB1Factor(int index, int step, const float &value);

    float getVvar(int index) const;
    void setVvar(int index, const float &value);

    Equation * getCustomSurvivalEqn(int index) const;
    void setCustomSurvivalEqn(int index, Equation *value);

    Equation * getPredTempResponseEqn(int index) const;
    void setPredTempResponseEqn(int index, Equation *value);

    Equation * getInriver_return_eqn() const;
    void setInriver_return_eqn(Equation *value);

    Equation * getTransportReturnEqn() const;
    void setTransportReturnEqn(Equation *value);

    float getReachSurvivalCoef(int index) const;
    void setReachSurvivalCoef(int index, const float &value);

protected:
    QString  *name;      /**< Name of the stock */
    QList<Equation *>  migrationEqn;/**< list of migration equations for reach classes [num_reach_classes]*/
    QList<float> mvcoef;		/**< migration variance [num_reach_classes]*/
    QList<float> distanceCoeff;  /**< "a" in "sqrt( a * x^2 + b * t^2 )" [num_reach_classes]*/
    QList<float> timeCoeff;      /**< "b" in "sqrt( a * x^2 + b * t^2 )" [num_reach_classes]*/

    QList<float> sigmaD;	/**< reach survival error parameter [num_reach_classes]*/

    QList<QList<float>> migrB1Factor; /**< constant for each step computed first and held here [num_reach_classes][STEPS_IN_SEASON] */
    QList<float> vvar;   /**< Herterogeneity of species - travel time distribution [num_reach_classes]*/

    /** This equation is used with the CUSTOM_CLASS mortality class so that
     * additional X-T-Theta models may be implemented and used easily. [num_reach_classes]*/
    QList<Equation *>  customSurvivalEqn;

    QList<Equation *>  predTempResponseEqn;

    /* These equations are used to estimate a return rate for adults based on
     * arrival timing at the transport destination (i.e. below Bonneville) */
    Equation * inriverReturnEqn; /**< return rate for inriver fish */
    Equation * transportReturnEqn;/**< return rate for transported fish */

    /* COMPASS reservoir survival model stuff */
    /** Reach survival coefficient (alpha) [num_reach_classes]*/
    QList<float> reachSurvivalCoef;
    /* Reach survival time coefficient (bee) */
    /* float	time_coef[10]; */ /* Already in species for XT model */
    /* Predator capture distance equation (gee of turbidity)
    equation  pred_capture_dist_eqn[10]; */

};

//extern Stock stock_defaults;
int float_arrs_equal (float *flptr1, float *flptr2, int num);

#endif // C_STOCK_H
