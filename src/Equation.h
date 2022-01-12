#ifndef C_EQUATION_H
#define C_EQUATION_H

#include <QStringList>

#define MAX_PARAMETERS (15)
/** \class parameter \brief A class that holds equation parameter values */
class Parameter {
public:
    Parameter();
    ~Parameter();

    const QString &getName() const;
    void setName(const QString &newName);

    double getValue() const;
    void setValue(double newValue);

    double getMin() const;
    void setMin(double newMin);

    double getMax() const;
    void setMax(double newMax);

private:
    QString name;    /**< parameter name */
    double value;    /**< parameter value */
    double min;      /**< parameter minimum value */
    double max;      /**< parameter maximum value */
};


/** \class A class that hold equation data.
 *  Maximum number of parameters is not restricted.               */
class Equation
{
public:
    Equation ();
    Equation (const Equation &other);
    ~Equation ();

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
    /** return the string representation of the formula */
    QString getFormula ();
    /** return the result of the equation at a particular x value. */
    virtual double value (double xval);

    void setNumParameters(int num);
    int getNumParameters() const;
    void setNumCoefficients(int num);
    int getNumCoefficients() const;

    /** Get a parameter by equation parameter number (0 to validParameters-1) */
    Parameter *getParameter (int num);
    /** Get a parameter by name */
    Parameter *getParameter (QString name);

    /** Get a coefficient by equation parameter number (0 to validParameters-1) */
    Parameter *getCoefficient (int num);
    /** Get a coefficient by name */
    Parameter *getCoefficient (QString name);

    const QString &getName() const;
    void setName(const QString &newName);

    int getId() const;
    void setId(int newId);

    void setFormula(const QString &newFormula);

private:
    QString name;       /**< equation name */
    int id;             /**< equation id number  */
    QString formula;    /**< equation string representation */

    int validParameters; /**< number of valid parameters for this equation */
    QList<Parameter *> parameters; /**< list of parameters */
    QList<Parameter *> coefficients; /**< list of default coefficients */
};

#endif // C_EQUATION_H
