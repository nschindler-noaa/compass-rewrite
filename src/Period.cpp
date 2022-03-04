#include "Period.h"


template <class T>
Period<T>::Period () :start(0), stop(0), value(0)
{}

template <class T>
Period<T>::Period (const Period<T> &period)
    :start(period.getStart()), stop(period.getStop()), value(period.getValue())
{}

template <class T>
Period<T>::~Period()
{}

template <class T>
void Period<T>::setStart(int index)
{    start = index; }

template <class T>
int Period<T>::getStart()
{    return start;  }

template <class T>
void Period<T>::setStop(int index)
{   stop = index;   }

template <class T>
int Period<T>::getStop()
{   return stop;    }

template <class T>
void Period<T>::setValue(T val)
{   value = val;    }

template <class T>
T Period<T>::getValue()
{   return value;   }


BoolPeriod::BoolPeriod()
{
    start = stop = 0;
    value = false;
}
BoolPeriod::BoolPeriod(const BoolPeriod&period)
{
    start = period.getStart();
    stop = period.getStop();
    value = period.getValue();
}
BoolPeriod::~BoolPeriod()
{
}

void BoolPeriod::setStart (int index)
{
    start = index;
}
int BoolPeriod::getStart () const
{
    return start;
}
void BoolPeriod::setStop (int index)
{
    stop = index;
}
int BoolPeriod::getStop () const
{
    return stop;
}

void BoolPeriod::setValue (bool val)
{
    value = val;
}
bool BoolPeriod::getValue () const
{
    return value;
}


