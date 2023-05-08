#ifndef CMPPERIODLIST_H
#define CMPPERIODLIST_H

#include "cmpfile.h"

#include <QObject>
#include <QStringList>


class cmpFloatPeriod
{
public:
    cmpFloatPeriod();
    cmpFloatPeriod(int i, int j, float val);
    cmpFloatPeriod(const cmpFloatPeriod &period) noexcept ;

    void setStart (int index);
    int getStart() const;
    void setStop (int index);
    int getStop () const;

    void setValue (float val);
    float getValue () const;

    bool isEqual(cmpFloatPeriod &period);

private:
    int start;
    int stop;
    float value;
};


class cmpFloatPeriodList
{
public:
    cmpFloatPeriodList();
    ~cmpFloatPeriodList();

    void allocate(int periods);
    bool parseData(cmpFile *infile, QString prompt);
    void writeData(cmpFile *outfile, int indent, QString name);

    const QList<cmpFloatPeriod> &getList() const;
    void setList(const QList<cmpFloatPeriod> &newList);

private:
    QList<cmpFloatPeriod> list;

};


#endif // CMPPERIODLIST_H
