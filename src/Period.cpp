#include "Period.h"


template <class T>
Period<T>::Period ()
{    start = 0; stop = 0; value = 0;    }

template <class T>
Period<T>::~Period()
{}

template <class T>
Period<T>::setStart(int index)
{    start = index; }

template <class T>
Period<T>::getStart()
{    return start;  }

template <class T>
Period<T>::setStop(int index)
{   stop = index;   }

template <class T>
Period<T>::getStop()
{   return stop;    }

template <class T>
Period<T>::setValue(T val)
{   value = val;    }

template <class T>
Period<T>::getValue()
{   return value;   }


template <class T>
void PeriodSubPeriod::setSubPeriod(int index, Period<T> *period)
{
    if (index < subPeriods.count())
    {
        subPeriods.takeAt(index);
    }
    subPeriods.insert(index, period);

}

template <class T>
Period<T>* PeriodSubPeriod::getSubPeriod(int index)
{
    Period<T> *period;
    if (index < subPeriods.count())
        period = subPeriods.at(index);

    return period;
}

template <class T>
PeriodList<T>::PeriodList (int stt, int stp, T val, bool pct)
{
    Period prd;
    prd.startday = stt;
    prd.stopday = stp;
    prd.value = val;
    percent = pct;

    periods.append (prd);
}

template <class T>
PeriodList<T>::PeriodList (bool pct)
{
    percent = pct;
}

template <class T>
PeriodList<T>::~PeriodList ()
{
    clear();
}

template <class T>
void PeriodList<T>::append (int stt, int stp, T val)
{
    Period prd;
    prd.startday = stt;
    prd.stopday = stp;
    prd.value = val;

    periods.append (prd);
}

template <class T>
void PeriodList<T>::clear ()
{
    if (!periods.isEmpty())
        periods.clear ();
}


template <class T>
T * PeriodList<T>::toArray (PeriodList<T> plist, int &size)
{
    int i, j;
    T array[size];
    Period prd = plist.periods.at[0];

    for (i = 0; i < plist.periods.count(); i++)
    {
       // Period prd = schedule.at [i];
        if (plist.periods[i]->stopday > size)
        {
            size = plist.periods[i]->stopday;
            realloc (array, size * sizeof(T));
        }
        for (j = plist.periods[i].startday; j <= plist.periods[i].stopday; j++)
        {
            array[j] = plist.periods.at[i].value;
        }
    }
//    if (j < STEPS_IN_RUN)
//        realloc (array, j * sizeof(T));

    return array;
}

template <class T>
PeriodList<T> PeriodList<T>::fromArray (T array[], int size)
{
    int i;
    PeriodList<T> plist = PeriodList<T>();
    Period prd;
    prd.startday = 0;
    prd.value = new T;
    *(prd.value) = array[0];
    prd.stopday = 0;
    prd.percent = false;

    for (i = 1; i < size; i++)
    {
        if (*prd.value != array[i])
        {
            prd.stopday = i - 1;
            plist.periods.append (prd);
            prd.startday = i;
            prd.value = array[i];
            prd.stopday = i;
        }
    }
    prd.stopday = i - 1;
    plist.periods.append (prd);

    return plist;
}


template <class T>
PeriodList<T> PeriodList<T>::copy (PeriodList<T> rhplist)
{
    int num_periods = rhs.periods.count();
    clear();
    for (int i = 0; i < num_periods; i++)
    {
        Period<T>* period = rhplist.getPeriod(i);
        periods.append(period);
    }
    return this;
}

template <class T>
PeriodList<T> PeriodList<T>::operator = (PeriodList<T> rhplist)
{
    return copy (rhplist);
}

template <class T>
bool PeriodList<T>::operator == (PeriodList<T> rhplist)
{
    bool same = true;
    int i, num_periods = periods.count();
    if (num_periods != rhplist.getNumPeriods())
        same = false;
    for (i = 0; i < num_periods && same; i++)
    {
        if (periods.at (i).start_day != rhplist.periods.at (i).start_day ||
                periods.at (i).stop_day != rhplist.periods.at (i).stop_day ||
                periods.at (i).value != rhplist.periods.at (i).value ||
                periods.at (i).percent != rhplist.periods.at (i).percent)
            same = false;
    }
    return same;
}

template <class T>
void PeriodList<T>::trimTrailingZeros()
{
    int i = periods.count() - 1;
    while (periods.at(i)->getValue() == 0)
    {
        periods.takeAt(i);
        i--;
    }
}

template <class T>
void PeriodList<T>::trimLeadingZeros()
{
    int i = 0;
    while (periods.at(i)->getValue() == 0)
    {
        periods.takeAt(i);
        i++;
    }
}



