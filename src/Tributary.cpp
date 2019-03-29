#include "Tributary.h"

Tributary::Tributary()
{
}

Tributary::~Tributary()
{
    while (course.count() > 0)
    {
        RiverPoint *pt = course.takeFirst();
        delete pt;
    }
}

QString Tributary::getName() const
{
    return name;
}

void Tributary::setName(const QString &value)
{
    name = value;
}

QList<RiverPoint *> Tributary::getCourse() const
{
    return course;
}

void Tributary::setCourse(const QList<RiverPoint *> &value)
{
    course = value;
}

RiverPoint * Tributary::getPoint(int index) const
{
    return course.at(index);
}

void Tributary::setPoint(int index, RiverPoint *point)
{
    RiverPoint *pt = course.takeAt(index);
    delete pt;
    course.insert(index, point);
}
