#ifndef CMPSCHEDULE_H
#define CMPSCHEDULE_H

#include "cmpfile.h"

#include <QObject>
#include <QStringList>

#define TOTALHOURS 24

class cmpDaySchedule
{
public:
    cmpDaySchedule();

    bool isOn(int hour);
    void setOn(int start, int end = 0);
    void setHour(int hour, bool on = true);
    void setHours(int start, int end, bool on = true);
    bool getHour(int hour);
    int getStartHour();
    int getStopHour();
    bool getHours(int &start, int &stop);

private:
    bool hours[24];
};

class cmpSchedule
{
public:
    cmpSchedule();
    ~cmpSchedule();

    void allocate (int numdays);
    bool parseData (cmpFile *infile, QString prompt = QString());
    void writeData (cmpFile *outfile, int indent, QString name, bool outputAll);

    int getScheduleLength();

    cmpDaySchedule getDaySchedule(int day);


private:
    QList<cmpDaySchedule> schedule;
    cmpDaySchedule current;
};

#endif // CMPSCHEDULE_H
