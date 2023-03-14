#ifndef CMPEQUATION_H
#define CMPEQUATION_H

#include "cmpfile.h"

#include <QStringList>

/** \class parameter \brief A class that holds equation parameter values. */
class cmpEqnParameter
{
public:
    cmpEqnParameter();
    cmpEqnParameter(const cmpEqnParameter &rhs);
    cmpEqnParameter(int id, double value, QString name = QString());
    cmpEqnParameter(int id, double value, double min, double max, QString name = QString());
    ~cmpEqnParameter();
    void clear();

    bool isEqual(const cmpEqnParameter &rhs);

    const QString &getName() const;
    void setName(const QString &newName);

    double getValue() const;
    void setValue(double newValue);

    double getMin() const;
    void setMin(double newMin);

    double getMax() const;
    void setMax(double newMax);

    int getId() const;
    void setId(int newId);

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
    cmpEquation(int eqid);
    cmpEquation(QString eqname);
    cmpEquation(const cmpEquation &rhs);
    virtual ~cmpEquation();
    cmpEquation &operator = (const cmpEquation &rhs);

    /** copy an existing equation */
    cmpEquation *copy(const cmpEquation &rhs);

    /** Equality */
    bool isEqual(const cmpEquation &rhs);
    bool operator == (const cmpEquation &rhs);
    bool operator == (cmpEquation &rhs);

    bool parseData(cmpFile *cfile, QString type = QString(), QString name = QString());
    void writeParameters(cmpFile *outfile, int indent, bool outputAll);

    /** Set up all aspects of equation for its id: parameters, name, formula, etc. */
    void setupEquation();
    /** Set number of total parameters */
    void setNumParameters(int newNum);
    /** Add a parameter */
    void addParameter (cmpEqnParameter *param);
    /** Add a parameter with optional minimum and maximum values */
    void addParameter (int num, double val, double mn = 0, double mx = 100, QString nm = QString());

    /** Copy an existing parameter */
    void setParameter (int num, cmpEqnParameter *param);
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

    const QString &getDescription() const;
    void setDescription(const QString &newDesc);

    int getId() const;
    void setId(int newId);

    int getNumEqnParams() const;
    void setNumEqnParams(int newNumEqnParams);

    int getNumValParams() const;
    void setNumValParams(int newNumValParams);

private:
    QString name;       /**< equation name */
    int id;             /**< equation id number  */
    QString description;/**< description of the equation [optional] */
    QString formula;    /**< equation string representation */

    int numEqnParams;   /**< number of parameters for this equation */
    int numValParams;   /**< number of value parameters needed to calculate
                             the general curve for a graphic display. */

    QList<cmpEqnParameter *> parameters; /**< list of parameters: this includes
                              equation parameters and value parameters. */

};

enum cmpEqnNum {
    EQ_LIN0       =  0,
    EQ_LIN1       =  1,
    EQ_LIN2       =  2,
    EQ_LIN3       =  3,
    EQ_EXP0       =  4,
    EQ_EXP1       =  5,
    EQ_EXP2       =  6,
    EQ_EXP3       =  7,
    EQ_POW0       =  8,
    EQ_POW1       =  9,
    EQ_POW2        = 10,
    EQ_POW3        = 11,
    EQ_NSAT0       = 12,
    EQ_FL_ACT      = 13,
    EQ_DELAY0      = 14,
    EQ_NSAT3       = 15,
    EQ_LOG0        = 16,
    EQ_NSAT1       = 17,
    EQ_NSAT2       = 18,
    EQ_GMORT0      = 19,
    EQ_DRIFT        = 20,
    EQ_DELAY1       = 21,
    EQ_FGE0         = 22,
    EQ_NSAT4        = 23,
    EQ_MIGR         = 24,
    EQ_GMORT1       = 25,
    EQ_GMORT2       = 26,
    EQ_FDENS        = 27,
    EQ_TRANS        = 28,
    EQ_NSAT5        = 29,
    EQ_NSAT6         = 30,
    EQ_GAS_DISP      = 31,
    EQ_GAS_DISP_RIGHT = 32,  // Unused
    EQ_MIGR2          = 33,
    EQ_GMORT3         = 34,
    EQ_CUB0           = 35,
    EQ_CUB1           = 36,
    EQ_PRED_TEMP1     = 37,
    EQ_PRED_TEMP2     = 38,
    EQ_MIGR1          = 39,
    EQ_MIGR3           = 40,
    EQ_MIGR4           = 41,
    EQ_MIGR5           = 42,
    EQ_MIGR6           = 43,
    EQ_SPILL_EFFICIENCY = 44,
    EQ_FLUSHTRANS       = 45,  // Unused
    EQ_DELAY3           = 46,
    EQ_MIGR7            = 47,
    EQ_MIGR8            = 48,
    EQ_MIGR9            = 49,
    EQ_MIGR10            = 50,
    EQ_RETURN_LOG        = 51,
    EQ_MIGR11            = 52,
    EQ_SIMPLEMORT5       = 53,  // Unused
    EQ_FLUSHTRANS5       = 54,  // Unused
    EQ_RSW               = 55,
    EQ_SURVIVAL_LINEAR   = 56,
    EQ_SURVIVAL_Z15      = 57,
    EQ_LATENT_MORT_WTT   = 58,
    EQ_MIGR12            = 59,
    EQ_MIGR13             = 60,
    EQ_FGE1               = 61,
    EQ_NSAT7              = 62,
    EQ_SURVIVAL_Z15_PRED_1 = 63,
    EQ_SPILL_EFF_PROBIT    = 64,
    EQ_FGE2                = 65,
    EQ_SURVIVAL_Z15_PRED_2 = 66,
    EQ_SURVIVAL_Z15_PRED_3 = 67,
    EQ_SWITCH0             = 68,
    EQ_MIGR14              = 69,
    EQ_SURVIVAL_XT_1       = 70,
    NUM_EQNS
};



#endif // CMPEQUATION_H
