#include "cmpriver.h"

cmpRiver::cmpRiver(QObject *parent) : QObject (parent)
{
    name = QString ("");
    flowMax = 0.0;
    flowMin = 0.0;
}

cmpRiver::cmpRiver(QString rname, QObject *parent) :
    QObject (parent)
{
    name = QString (rname);
    flowMax = 0.0;
    flowMin = 0.0;
}

cmpRiver::~cmpRiver()
{
    segments.clear();
}

QString &cmpRiver::getName()
{
    return name;
}

void cmpRiver::setName(QString &value)
{
    name = value;
}

float cmpRiver::getFlowMax() const
{
    return flowMax;
}

void cmpRiver::setFlowMax(float value)
{
    flowMax = value;
}

float cmpRiver::getFlowMin() const
{
    return flowMin;
}

void cmpRiver::setFlowMin(float value)
{
    flowMin = value;
}

QList<cmpRiverSegment *> cmpRiver::getSegments() const
{
    return segments;
}

void cmpRiver::setSegments(const QList<cmpRiverSegment *> &value)
{
    segments = value;
}

cmpRiverSegment *cmpRiver::getSegment(int index) const
{
    cmpRiverSegment *rseg = nullptr;
    if (segments.count() > index)
        rseg = segments.at(index);
    return rseg;
}

void cmpRiver::addSegment(cmpRiverSegment *value)
{
    if (value != nullptr)
    {
        int index = segments.count();
        segments.append(nullptr);
        segments[index] = value;
    }
}

int cmpRiver::getNumSegments()
{
    return segments.count();
}

bool cmpRiver::parseDesc(cmpFile *descfile)
{
    bool okay = true;
    if (descfile->isReadable())
        ;
    else {
        okay = false;
    }

    return okay;
}

bool cmpRiver::outputDesc(cmpFile *descfile)
{
    bool okay = true;
    if (descfile->isReadable())
        ;
    else {
        okay = false;
    }

    return okay;
}

bool cmpRiver::output(cmpFile *cfile)
{
    bool okay = true;
    if (cfile->isReadable())
        ;
    else {
        okay = false;
    }

    return okay;
}

