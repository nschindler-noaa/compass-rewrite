#include "cmpschedule.h"

cmpDaySchedule::cmpDaySchedule()
{
    for (int i = 0; i < TOTALHOURS; i++)
        hours[i] = false;
}

bool cmpDaySchedule::isOn(int hour)
{
    return (hours[hour] == true);
}

void cmpDaySchedule::setOn(int start, int end)
{
    for (int i = start; i <= end; i++)
        hours[i] = true;
}

void cmpDaySchedule::setHour(int hour, bool on)
{
    hours[hour] = on;
}

void cmpDaySchedule::setHours(int start, int end, bool on)
{
    for (int i = start; i <= end; i++)
        hours[i] = on;
}

bool cmpDaySchedule::getHour(int hour)
{
    return hours[hour];
}

int cmpDaySchedule::getStartHour()
{
    int start, stop;
    getHours(start, stop);
    return start;
}

int cmpDaySchedule::getStopHour()
{
    int start, stop;
    getHours(start, stop);
    return stop;
}

bool cmpDaySchedule::getHours(int &start, int &stop)
{
    int i = 0;
    bool anyhours = true;
    start = 0;
    stop = 23;
    while (!hours[i] && i < TOTALHOURS)
    {
        start = i;
        i++;
    }
    while (hours[i] && i < TOTALHOURS)
    {
        stop = i;
        i++;
    }
    if (start == stop)
        anyhours = false;
    return anyhours;
}



cmpSchedule::cmpSchedule()
{}

cmpSchedule::~cmpSchedule()
{
    if (!schedule.isEmpty())
        schedule.clear();
}

void cmpSchedule::allocate (int numdays)
{
    if (!schedule.isEmpty())
        schedule.clear();
    for (int i = 0; i < numdays; i++)
    {
        cmpDaySchedule day;
        schedule.append(day);
    }
}

bool cmpSchedule::parseData (cmpFile *infile, QString prompt)
{
    bool okay = true, end = false;
    QString token ("");
    QStringList days, hours;
    int startday = 0, endday = 0, starthr = 0, endhr = 0;

    while (okay && !end)
    {
        token = infile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            infile->printEOF(prompt);
            okay = false;
        }
        else if (token.contains(':'))
        {
            days = token.split(':');
            startday = days[0].toInt(&okay);
            if (okay)
                endday = days[1].toInt(&okay);
            if (okay)
            {
                token = infile->popToken();
                hours = token.split(':');
                if (hours.count() == 2)
                {
                    hours[0].remove('(');
                    hours[1].remove(')');
                    starthr = hours[0].toInt(&okay);
                    endhr = hours[1].toInt(&okay);
                    for (int i = startday; i <= endday; i++)
                        schedule[i].setHours(starthr, endhr, true);
                }
            }
        }
        else if (infile->isInt(token))
        {
            startday = token.toInt(&okay);
            if (okay)
            {
                token = infile->popToken();
                hours = token.split(':');
                if (hours.count() == 2)
                {
                    hours[0].remove('(');
                    hours[1].remove(')');
                    starthr = hours[0].toInt(&okay);
                    endhr = hours[1].toInt(&okay);
                    schedule[startday].setHours(starthr, endhr, true);
                }
            }
        }
        else
        {
//            infile->unknownToken(token, prompt);
            infile->pushToken(token);
            end = true;
        }
        if (endday >= schedule.count() || startday >= schedule.count())
            end = true;
    }
    return okay;
}

void cmpSchedule::writeData (cmpFile *outfile, int indent, QString name, bool outputAll)
{
    QString line;
    if (outfile->isOpen())
    {
        int numDays = schedule.count();
        int stDay = 0, endDay = 0, stHr = 0, endHr = 0;
        int firstHour = 0, lastHour = 24;
        int numOnLine = 0;
        bool hoursChanged = false;

        outfile->writeStringNR(indent, name);

        schedule[stDay].getHours(stHr, endHr);

        for (int i = 1; i < numDays; i++)
        {
            schedule[i].getHours(firstHour, lastHour);

            if (firstHour != stHr || lastHour != endHr)
            {
                hoursChanged = true;
                endDay = i - 1;
                if ((endDay - stDay) > 1)
                {
                    outfile->writeStringNR(0, QString(" %1:%2").arg(stDay).arg(endDay));
                }
                else
                {
                    outfile->writeStringNR(0, QString::number(stDay));
                }
                outfile->writeStringNR(0, QString(" (%1:%2)").arg(stHr).arg(endHr));
                stDay = i;
                stHr = firstHour;
                endHr = lastHour;
                numOnLine += 2;
            }
            if (numOnLine >= 4)
            {
                outfile->writeNewline();
                outfile->writeIndent(indent);
            }

        }
        if (numOnLine < 4)
            outfile->writeNewline();
    }
}

int cmpSchedule::getScheduleLength()
{
    return schedule.size();
}

cmpDaySchedule cmpSchedule::getDaySchedule(int day)
{
    if (day >= 0 && day < schedule.count())
        current = schedule[day];
    else
        current = schedule[0];
    return current;
}


