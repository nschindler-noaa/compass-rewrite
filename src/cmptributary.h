#ifndef CMPTRIBUTARY_H
#define CMPTRIBUTARY_H

#include "cmpriverpoint.h"

#include <QStringList>

class cmpTributary
{
public:
    cmpTributary();

    QString getName() const;
    void setName(const QString &value);

    QList<cmpRiverPoint *> getCourse() const;
    void setCourse(const QList<cmpRiverPoint *> &value);

    cmpRiverPoint * getPoint(int index = 0) const;
    void setPoint (int index, cmpRiverPoint * point);

    int getFlowCount() {return flow.count();}
    float getFlow(int index);
    void setFlow(int index, float flw);

protected:
    QString name;
    QList<cmpRiverPoint *> course;
    QList<float> flow;
};

#endif // CMPTRIBUTARY_H
