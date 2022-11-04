#include "cmpriver.h"

#include "cmpriversystem.h"

cmpRiver::cmpRiver(cmpRiverSystem *parent) : QObject (parent)
{
    setup(QString(), parent);
//    name = QString ("");
//    flowMax = 0.0;
//    flowMin = 0.0;
}

cmpRiver::cmpRiver(QString rname, cmpRiverSystem *parent) :
    QObject (parent)
{
    setup(QString(), parent);
//    name = QString (rname);
//    flowMax = 0.0;
//    flowMin = 0.0;
}

void cmpRiver::setup(QString name, cmpRiverSystem *parent)
{
    rs = parent;
    flowMax = 1.0;
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
    bool okay = true, end = false;
    QString token (""), val ("");
    float tempFloat = 0;
    int tempInt = 0;


    while (okay && !end)
    {
        token = descfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            descfile->printEOF ("River description");
            okay = false;
        }
        else if (token.compare ("flow_max") == 0)
        {
            okay = descfile->readFloatOrNa(token, tempFloat);
            setFlowMax(tempFloat);
        }
        else if (token.compare ("flow_min") == 0)
        {
            okay = descfile->readFloatOrNa(token, tempFloat);
            setFlowMin(tempFloat);
        }
        else if (token.compare ("reach") == 0)
        {
            QString reachName ("");
            okay = descfile->readString (reachName);
            if (okay)
            {
                cmpReach *reach = new cmpReach (this);
                reach->setName(reachName);
                rs->reaches.append (reachName);
                rs->segments.append ((cmpRiverSegment *) reach);
                reach->setRiverName(getName());
                okay = reach->parseDesc(descfile);
                addSegment(reach);
            }
        }
        else if (token.compare ("dam") == 0)
        {
            QString damName ("");
            okay = descfile->readString (damName);
            if (okay)
            {
                cmpDam *dam = new cmpDam (damName);
                rs->dams.append (damName);
                rs->segments.append ((cmpRiverSegment *) dam);
                dam->setRiverName(getName());
                okay = dam->parseDesc(descfile);
                addSegment(dam);
            }
        }
        else if (token.compare ("headwater") == 0)
        {
            QString hwName ("");
            okay = descfile->readString (hwName);
            if (okay)
            {
                cmpHeadwater *head = new cmpHeadwater (hwName);
                rs->headwaters.append (hwName);
                rs->segments.append ((cmpRiverSegment *) head);
                head->setRiverName(getName());
                okay = head->parseDesc(descfile);
                addSegment(head);
            }
        }
        else if (token.contains ("end"))
        {
            end = descfile->checkEnd("river", name);
        }
        else
        {
            descfile->unknownToken(token, name);
            descfile->skipLine ();
        }
    }
    // we have ended, but is it at a headwater?
    // create headwater of this river, if it doesn't exist
    cmpRiverSegment *cur = rs->segments.last();
    if (okay && cur->getType() != cmpRiverSegment::Headwater)
    {
        QString hname (getName());
        hname.append(" Headwater");
        cur->setUpperSegment(new cmpHeadwater (hname, this));
        cur->getUpperSegment()->setLowerSegment(cur);
        rs->segments.append (cur->getUpperSegment());
        rs->headwaters.append(hname);
        descfile->printMessage(QString("Adding headwater %1").arg(hname));
//        cmpLog::outlog->add(cmpLog::Debug, QString (
//                  QString("adding headwater %1").arg(hname)));
    }
    return okay;
}

bool cmpRiver::outputDesc(cmpFile *descfile)
{
    bool okay = true, end = false;
    QString token, val;

    if (descfile->open(QIODevice::WriteOnly))
    {
        // output river values
        descfile->writeString(0, QString("river"), name);
        descfile->writeValue(0, QString("flow_max"), flowMax);
        descfile->writeValue(0, QString("flow_min"), flowMin);
        descfile->writeNewline();

        // output river segments
        for (int i = 0; i < segments.count(); i++)
            segments.at(i)->outputDesc(descfile);
        descfile->writeNewline();

        // output end statement
        descfile->writeEnd(0, QString("river"), name);
        descfile->writeNewline();
    }
    else
    {

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

