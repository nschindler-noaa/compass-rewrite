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
    if (index < course.count())
        return course.at(index);
    else
        return nullptr;
}

void Tributary::setPoint(int index, RiverPoint *point)
{
    while (index > course.count())
        course.append(nullptr);
    RiverPoint *pt = course.takeAt(index);
    if (pt != nullptr)
        delete pt;
    course.insert(index, point);
}

