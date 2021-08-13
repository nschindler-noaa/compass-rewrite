#ifndef CMPEQUATION_H
#define CMPEQUATION_H

#include <QStringList>

/** \class parameter \brief A class that holds equation parameter values. */
class cmpEqnParameter
{
public:
    cmpEqnParameter();
    cmpEqnParameter(int id, double value, double min, double max, QString name = QString());

    const QString &getName() const;
    void setName(const QString &newName);

    double getValue() const;
    void setValue(double newValue);

    double getMin() const;
    void setMin(double newMin);

    double getMax() const;
    void setMax(double newMax);

private:
    QString name;   /**< name of the parameter or default. */
    int    id;      /**< id of the parameter. */
    double value;   /**< mean value of the parameter. */
    double min;     /**< min value, used in monte carlo methods */
    double max;     /**< max value, used in monte carlo methods */
};
/** /class cmpEquation /brief A class that holds parameters and formula for
      a particular computation. */
class cmpEquation
{
public:
    cmpEquation();
    cmpEquation(const cmpEquation &rhs);
    virtual ~cmpEquation();

    /** Add a parameter */
    void addParameter (cmpEqnParameter *param);
    /** Add a parameter with optional minimum and maximum values */
    void addParameter (int num, double val, double mn = 0, double mx = 100, QString nm = QString());

    /** Set a parameter value by name with optional minimum and maximum values */
    void setParameter (QString name, double val, double mn = -1, double mx = -1);
    /** Set a parameter value by id with optional minimum and maximum values */
    void setParameter (int num, double val, double mn = -1, double mx = -1, QString name = QString());

    /** Get a parameter by equation parameter number (0 to numEqnParams-1) */
    cmpEqnParameter *getParameter (int num) const;
    /** Get a parameter by name */
    cmpEqnParameter *getParameter (QString name) const;

    /** return the string representation */
    const QString getFormula() const;
    /** set the string representation of the equation */
    void setFormula(const QString &newFormula);
    /** return the result of the equation */
    virtual double value (double xval = 0);

    const QString &getName() const;
    void setName(const QString &newName);

    int getId() const;
    void setId(int newId);

    int getNumEqnParams() const;
    void setNumEqnParams(int newNumEqnParams);

    int getNumValParams() const;
    void setNumValParams(int newNumValParams);

private:
    QString name;       /**< equation name */
    int id;             /**< equation id number  */
    QString formula;    /**< equation string representation */

    /** copy an existing equation */
    cmpEquation *copy(const cmpEquation &rhs);

    int numEqnParams;   /**< number of parameters for this equation */
    int numValParams;   /**< number of value parameters needed to calculate
                             the general curve for a graphic display. */

    QList<cmpEqnParameter *> parameters; /**< list of parameters: this includes
                              equation parameters and value parameters. */
};

#endif // CMPEQUATION_H
