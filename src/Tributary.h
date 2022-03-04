#ifndef C_TRIBUTARY_H
#define C_TRIBUTARY_H

#include <QString>
#include <QList>


#include "RiverPoint.h"

class Tributary
{
public:
    Tributary();
    ~Tributary();

    QString getName() const;
    void setName(const QString &value);

    QList<RiverPoint *> getCourse() const;
    void setCourse(const QList<RiverPoint *> &value);

    RiverPoint * getPoint(int index = 0) const;
    void setPoint (int index, RiverPoint * point);

    int getFlowCount() {return flow.count();}
    float getFlow(int index);
    void setFlow(int index, float flw);

protected:
    QString name;
    QList<RiverPoint *> course;
    QList<float> flow;
};

#endif // C_TRIBUTARY_H
