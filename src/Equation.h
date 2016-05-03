#ifndef C_EQUATION_H
#define C_EQUATION_H

#include <QString>

#define MAX_PARAMETERS (15)

/** \class A class that hold info for a particular equation.
 *  Maximum number of parameters is set to 15.               */
class Equation
{
public:
    Equation ();
    Equation (const Equation &other);
    ~Equation ();

    QString name;       /**< equation name */
    int id;             /**< equation id number  */
    QString formula;    /**< equation string representation */

    /** Add a parameter value by name with optional minimum and maximum values */
    void addParameter (QString name, double val, double mn = 0, double mx = 100);
    /** Add a parameter value by id with optional minimum and maximum values */
    void addParameter (int num, double val, double mn = 0, double mx = 100);

    /** Set a parameter value by name with optional minimum and maximum values */
    void setParameter (QString name, double val, double mn = -1, double mx = -1);
    /** Set a parameter value by id with optional minimum and maximum values */
    void setParameter (int num, double val, double mn = -1, double mx = -1);

    /** copy an existing equation */
    Equation copy (const Equation &rhs);
    /** return the string representation */
    QString getFormula ();
    /** return the result of the equation */
    double value (double xval);

    int validParameters; /**< number of valid parameters for this equation */
    /** \struct parameter \brief A struct that holds equation parameter values */
    typedef struct parameter {
        QString name;    /**< parameter name */
        double value;    /**< parameter value */
        double min;      /**< parameter minimum value */
        double max;      /**< parameter maximum value */
    }Parameter;

    /** Get a parameter by equation parameter number (0 to validParameters-1) */
    Parameter *getParameter (int num);
    /** Get a parameter by name */
    Parameter *getParameter (QString name);

    Parameter parameters[MAX_PARAMETERS]; /**< list of parameters */
};

#endif // C_EQUATION_H
