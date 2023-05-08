#include "cmpboollist.h"

cmpHourList::cmpHourList(int st, int end)
{
    setHours(st, end);
}

void cmpHourList::getHours(int &stHour, int &endHour)
{
    stHour = hours[0];
    endHour = hours[1];
}

void cmpHourList::setHours(int st, int end)
{
    hours[0] = st;
    hours[1] = end;
}

//void cmpHourList::operator=(cmpHourList &rhl)
//{
//    int stHr, endHr;
//    rhl.getHours(stHr, endHr);
//    setHours(stHr, endHr);
//}


cmpDayHourList::cmpDayHourList()
{
    days = 366;
    allocate(days);
}

cmpDayHourList::~cmpDayHourList()
{
    cbl.clear();
}

void cmpDayHourList::reset()
{
    if (cbl.isEmpty())
    {
        allocate(days);
    }
    else
    {
        for (int i = 0; i < days; i++)
        {
            for (int j = 0; j < hours; j++)
                cbl[i][j] = true;
        }
    }
}

void cmpDayHourList::allocate(int newDays)
{
    days = newDays;

    if (!cbl.isEmpty())
        cbl.clear();
    for (int i = 0; i < days; i++)
    {
//        cmpHourList *chl = new cmpHourList(0, 24);
        cbl.append(cmpHourList());
    }
}

bool cmpDayHourList::parseData(cmpFile *infile, QString prompt)
{
    bool okay = true, end = false;
    QString token ("");
    QStringList indays, inhours;
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
            indays = token.split(':');
            startday = indays[0].toInt(&okay);
            if (okay)
                endday = indays[1].toInt(&okay);
            if (okay)
            {
                cmpHourList chl;
                token = infile->popToken();
                inhours = token.split(':');
                if (inhours.count() == 2)
                {
                    inhours[0].remove('(');
                    inhours[1].remove(')');
                    starthr = inhours[0].toInt(&okay);
                    endhr = inhours[1].toInt(&okay);
                    if (okay)
                        chl.setHours(starthr, endhr);
                }
                else
                {
                    inhours[0].remove('(');
                    inhours[0].remove(')');
                    starthr = inhours[0].toInt(&okay);
                    if (okay)
                        chl.setHours(starthr, 24);
                }
                for (int i = startday; i <= endday; i++)
                {
                    cbl[i] = chl;
                }
            }
        }
        else if (infile->isInt(token))
        {
            startday = token.toInt(&okay);
            if (okay)
            {
                token = infile->popToken();
                inhours = token.split(':');
                cmpHourList chl;
                if (inhours.count() == 2)
                {
                    inhours[0].remove('(');
                    inhours[1].remove(')');
                    starthr = inhours[0].toInt(&okay);
                    endhr = inhours[1].toInt(&okay);
                    if (okay)
                    {
                        chl.setHours(starthr, endhr);
                        cbl[startday] = chl;
                    }
                }
            }
        }
        else
        {
            infile->pushToken(token);
            end = true;
        }
        if (endday >= days || startday >= days)
            end = true;
    }
    return okay;
}

void cmpDayHourList::writeData(cmpFile *outfile, int indent, QString title, bool outputAll)
{
    bool writeAll = outputAll;
    QString line;

    if (!outputAll)
    {
        for (int i = 0; i < days; i++)
            for (int j = 0; j < hours; j++)
                if (cbl[i][j] == false)
                    writeAll = true;
    }
    if (writeAll)
    {
        if (outfile->isOpen())
        {
            int indent2 = indent + 1;
            int numOnLine = 0;
            bool value = true, oldvalue = true;
            bool hoursChanged = false;

            int stDay = 0, endDay = 0, stHr = 0, endHr = 0;
            int firstHour = 0, lastHour = 24;
            QString days, hours;

            outfile->writeStringNR(indent, title);

            cbl[stDay].getHours(stHr, endHr);
            firstHour = stHr;
            lastHour = endHr;

            for (int i = 1; i < days; i++)
            {
                days = QString::number(stDay);
                hours = QString("(%1:%2)").arg(stHr).arg(endHr);
                endDay = i;

                cbl[i].getHours(firstHour, lastHour);

                if (firstHour != stHr || lastHour != endHr)
                {
                    hoursChanged = true;
                    if ((endDay - stDay) > 1)
                    {
                        days.append(QString(":%1").arg(endDay));
                        stDay = i + 1;
                    }
                    else
                    {
                        stDay = i;
                    }
                    days.prepend(" ");
                    outfile->writeStringNR(0, days, hours);
                    stHr = firstHour;
                    endHr = lastHour;
                    numOnLine += 2;
                }
                if (numOnLine >= 4)
                {
                    outfile->writeNewline();
                    outfile->writeIndent(indent);
                    numOnLine = 0;
                }

            }
            if (stDay == 0)
            {
                outfile->writeSpace();
                outfile->writeStringNR(0, QString("%1:%2").arg(stDay).arg(endDay), hours);
                numOnLine = 2;
            }
            if (numOnLine > 0)
                outfile->writeNewline();
        }
    }
}


bool cmpDayHourList::getValue(int day, int hour)
{
    bool isOn = false;
    if (day >= 0 && day < days)
    {
        int stHr, endHr;
        getHours(day, stHr, endHr);
        if (hour >= stHr && hour <= endHr)
            isOn = true;
    }
    return isOn;
}

void cmpDayHourList::getHours(int day, int &stHour, int &endHour)
{
    cbl[day].getHours(stHour, endHour);
}


