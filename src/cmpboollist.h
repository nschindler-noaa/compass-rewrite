#ifndef CMPBOOLLIST_H
#define CMPBOOLLIST_H

#include "cmpfile.h"

#include <QStringList>
#include <QObject>

class cmpHourList : public QList<bool>
{
public:
    cmpHourList(int st = 0, int end = 24);

    void getHours(int &stHour, int &endHour);
    void setHours(int st, int end);

//    void operator =(cmpHourList &rhl);


private:
    int hours[2];
};


class cmpDayHourList
{
public:
    cmpDayHourList();
    ~cmpDayHourList();

    void reset();
    void allocate(int newDays = 366);
    bool parseData(cmpFile *infile, QString prompt);
    void writeData(cmpFile *outfile, int indent, QString title, bool outputAll);

    bool getValue(int day, int hour);
    void getHours(int day, int &stHour, int &endHour);

private:
    QList<cmpHourList> cbl;
    int days;
    int hours;

};

#endif // CMPBOOLLIST_H
