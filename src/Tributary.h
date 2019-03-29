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

protected:
    QString name;
    QList<RiverPoint *> course;
};

#endif // C_TRIBUTARY_H
