#include "cmpperiodlist.h"

template <class T>
cmpPeriod<T>::cmpPeriod () :start(0), stop(0), value(0)
{}

template <class T>
cmpPeriod<T>::cmpPeriod (const cmpPeriod<T> &period)
{
    start = period.getStart();
    stop = period.getStop();
    value = period.getValue();
}

template <class T>
void cmpPeriod<T>::setStart(int index)
{    start = index; }

template <class T>
int cmpPeriod<T>::getStart()
{    return start;  }

template <class T>
void cmpPeriod<T>::setStop(int index)
{   stop = index;   }

template <class T>
int cmpPeriod<T>::getStop()
{   return stop;    }

template <class T>
void cmpPeriod<T>::setValue(T val)
{   value = val;    }

template <class T>
T cmpPeriod<T>::getValue()
{   return value;   }

template <class T>
bool cmpPeriod<T>::isEqual(cmpPeriod<T> &period)
{
    bool equal = true;
    if (period.getStart() != start)
        equal = false;
    else if (period.getStop() != stop)
        equal = false;
    else if (period.getValue() != value)
        equal = false;
    return equal;
}

cmpFloatPeriod::cmpFloatPeriod () :start(0), stop(0), value(0)
{}

cmpFloatPeriod::cmpFloatPeriod (const cmpFloatPeriod &period) noexcept
{
    start = period.getStart();
    stop = period.getStop();
    value = period.getValue();
}

void cmpFloatPeriod::setStart(int index)
{    start = index; }

int cmpFloatPeriod::getStart() const
{    return start;  }

void cmpFloatPeriod::setStop(int index)
{   stop = index;   }

int cmpFloatPeriod::getStop() const
{   return stop;    }

void cmpFloatPeriod::setValue(float val)
{   value = val;    }

float cmpFloatPeriod::getValue() const
{   return value;   }

bool cmpFloatPeriod::isEqual(cmpFloatPeriod &period)
{
    bool equal = true;
    if (period.getStart() != start)
        equal = false;
    else if (period.getStop() != stop)
        equal = false;
    else if (period.getValue() != value)
        equal = false;
    return equal;
}


cmpFloatPeriodList::cmpFloatPeriodList()
{}

cmpFloatPeriodList::~cmpFloatPeriodList()
{
    if (!list.isEmpty())
        list.clear();
}

void cmpFloatPeriodList::allocate(int periods)
{
    if (!list.isEmpty())
        list.clear();
    for (int i = 0; i < periods; i++)
    {
        cmpFloatPeriod period;
        list.append(period);
    }
}

bool cmpFloatPeriodList::parseData(cmpFile *infile, QString prompt)
{
    bool okay = true, end = false;
    QString token;
    QStringList periods, hours;
    int start = 0, stop = 0;
    float value;
    cmpFloatPeriod cfp;

    if(!list.isEmpty())
        list.clear();

    while (okay && !end)
    {
        token = infile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            infile->printEOF(prompt);
            okay = false;
        }
        else if (token.contains('['))
        {
            token.remove('[');
            token.remove(']');
            periods = token.split(':');
            start = periods[0].toInt(&okay);
            if (okay)
                stop = periods[1].toInt(&okay);
            if (okay)
            {
                token = infile->popToken();
                value = token.toFloat(&okay);
                if (okay)
                {
                    cfp.setStart(start);
                    cfp.setStop(stop);
                    cfp.setValue(value);
                    list.append(cfp);
                }
            }
            else
            {
                infile->printError(QString("error in parsing float list for %1").arg(prompt));
            }
        }
        else
        {
            infile->unknownToken(token, prompt);
            infile->pushToken(token);
            end = true;
        }
    }
    return okay;
}

void cmpFloatPeriodList::writeData(cmpFile *outfile, int indent, QString name)
{
    int total = list.count();
    int start = 0, stop = total - 1;
    int numOnLine = 0;
    cmpFloatPeriod cfp, nextcfp;
    QString valuestr;

    outfile->writeStringNR(indent, name);
    for (int i = 0; i < total; i++)
    {
        cfp = list.at(i);
        nextcfp = list.at(i+1);
        if (!cfp.isEqual(nextcfp))
        {
            stop = i;
            valuestr = QString::number(cfp.getValue(), 'f', 2);
            // write the stuff
            outfile->writeStringNR(0, QString(" [%1:%2]").arg(start).arg(stop));
            outfile->writeStringNR(0, QString(" %1").arg(valuestr));
            numOnLine += 2;
            start = i + 1;
        }
        if (numOnLine >= 4)
        {
            outfile->writeNewline();
            outfile->writeIndent(indent);
            numOnLine = 0;
        }
    }
    if (numOnLine > 1)
    {
        outfile->writeNewline();
    }
}

const QList<cmpFloatPeriod> &cmpFloatPeriodList::getList() const
{
    return list;
}

void cmpFloatPeriodList::setList(const QList<cmpFloatPeriod> &newList)
{
    list = newList;
}


