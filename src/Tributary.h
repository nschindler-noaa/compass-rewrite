#ifndef C_TRIBUTARY_H
#define C_TRIBUTARY_H

#include <QString>
#include <QList>


#include "RiverPoint.h"

class Tributary
{
public:
    Tributary();

    QString name;
    QList<RiverPoint *> course;
};

#endif // C_TRIBUTARY_H
