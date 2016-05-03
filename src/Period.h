#ifndef C_PERIOD_H
#define C_PERIOD_H

#include <QList>

/*template <class T>
struct period{
    int startday;
    int stopday;
    T value;
};
template <class T>
typedef struct period<T> Period<T>;*/

template <class T>
class Period
{
public:
    Period();
    ~Period();

    void setStart (int index);
    int getStart ();
    void setStop (int index);
    int getStop ();

    void setValue (T val);
    T getValue ();

private:
    int start;
    int stop;
    T value;
};

template <class T>
class PeriodSubPeriod : public Period<T>
{
public:
    PeriodSubPeriod();
    ~PeriodSubPeriod();

    void setSubPeriod (int index, Period<T> *period);
    Period<T> *getSubPeriod(int index);

private:
    QList <Period<T> *> subPeriods;
};

template <class T>
class PeriodList
{
public:
    PeriodList (int stt = 0, int stp = 0, T val = 0, bool pct = false);
    PeriodList (bool pct);
    ~PeriodList ();

    void append (int stt, int stp, T val);
    void clear ();

    T *toArray (PeriodList<T>, int &count);
    PeriodList<T> fromArray (T *array, int count);

    PeriodList<T> copy (PeriodList<T> rhplist);
    PeriodList<T> operator = (PeriodList<T> rhplist);
    bool operator == (PeriodList<T>);

    void setPercent (bool pct);
    bool isPercent () ;

    int getNumPeriods () {return periods.count();}
    void setPeriod (int index, Period<T> *per);
    Period<T>* getPeriod (int index);

    void trimTrailingZeros ();
    void trimLeadingZeros ();


private:
    QList <Period<T> *> periods;

    bool percent;


};
/*
class FloatPeriodList
{
public:
    FloatPeriodList (int stt = 0, int stp = 0, float val = 0, bool pct = false);
    FloatPeriodList (bool pct = false);
    ~FloatPeriodList ();

    QList<Period *> periods;

    bool percent;

    void append (int stt, int stp, float val);
    void clear ();

    float *toArray (FloatPeriodList, int &count);
    FloatPeriodList fromArray (float *array, int count);

    FloatPeriodList operator = (FloatPeriodList);
    bool operator == (FloatPeriodList);
};
*/
typedef PeriodList<float> FloatPeriodList;
typedef PeriodList<int>   IntPeriodList;

#endif // C_PERIOD_H
