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
    setup(rname, parent);
//    name = QString (rname);
//    flowMax = 0.0;
//    flowMin = 0.0;
}

void cmpRiver::setup(QString rname, cmpRiverSystem *parent)
{
    name = rname;
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
        segments.append(value);
//        segments[index] = value;
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

    std::cout << "     Parsing River description: " << name.toStdString() << std::endl;

    while (okay && !end)
    {
        token = descfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            descfile->printEOF ("River description");
            okay = false;
        }
        else if (token.compare ("flow_min") == 0)
        {
            okay = descfile->readFloatOrNa(token, tempFloat);
            setFlowMin(tempFloat);
        }
        else if (token.compare ("flow_max") == 0)
        {
            okay = descfile->readFloatOrNa(token, tempFloat);
            setFlowMax(tempFloat);
        }
        else if (token.compare ("reach") == 0)
        {
            QString reachName ("");
            okay = descfile->readString (reachName);
            if (okay)
            {
                reachName = reachName.simplified().replace(' ', '_');
                cmpReach *reach = new cmpReach (reachName, this);
//                reach->setName(reachName);
                rs->reaches.append (reachName);
                rs->segments.append (reach);
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
                damName = damName.simplified().replace(' ', '_');
                cmpDam *dam = new cmpDam (damName);
                rs->dams.append (damName);
                rs->segments.append (dam);
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
                hwName = hwName.simplified().replace(' ', '_');
                cmpHeadwater *head = new cmpHeadwater (hwName);
                rs->headwaters.append (hwName);
                rs->segments.append (head);
//                segments.append(head);
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
        hname.append("_Headwater");
        cur->setUpperSegment(new cmpHeadwater (hname, this));
        cur->getUpperSegment()->setLowerSegment(cur);
        rs->segments.append (cur->getUpperSegment());
        rs->headwaters.append(hname);
        descfile->printMessage(QString("Adding headwater %1").arg(hname));
        addSegment(cur->getUpperSegment());
//        cmpLog::outlog->add(cmpLog::Debug, QString (
//                  QString("adding headwater %1").arg(hname)));
    }
    return okay;
}

bool cmpRiver::outputDesc(cmpFile *descfile)
{
    bool okay = true, end = false;
    QString token, val;
    QString namestr = name;
    cmpDam *dam;
    cmpReach *reach;
    cmpHeadwater *headwtr;


    if (descfile->isOpen())
    {
        // output river values
        descfile->writeString(0, QString("river"), namestr.replace('_', ' '));
        descfile->writeValue(1, QString("flow_min"), flowMin, Data::Float);
        descfile->writeValue(1, QString("flow_max"), flowMax, Data::Float);
        descfile->writeNewline();

        // output river segments
        for (int i = 0; i < segments.count(); i++)
        {
            switch (segments.at(i)->getType())
            {
            case cmpRiverSegment::Dam:
                dam = static_cast<cmpDam *>(segments.at(i));
                dam->outputDesc(descfile);
                break;
            case cmpRiverSegment::Reach:
                reach = static_cast<cmpReach *>(segments.at(i));
                reach->outputDesc(descfile);
                break;
            case cmpRiverSegment::Headwater:
                headwtr = static_cast<cmpHeadwater *>(segments.at(i));
                headwtr->outputDesc(descfile);
                break;
            }
            descfile->writeNewline();
        }

        // output end statement
        descfile->writeEnd(0, QString("river"), namestr);
        descfile->writeNewline();
    }
    else
    {
        okay = false;
    }

    return okay;
}


bool cmpRiver::outputData(cmpFile *cfile, bool outputAll)
{
    bool okay = true;
    int total = segments.count();
    cmpDam *dam;
    cmpReach *reach;
    cmpHeadwater *hwater;
    cmpRiverSegment *seg;
    cmpRiverSegment::SegmentType type;
    if (cfile->isOpen())
    {
        for (int i = 0; i < total; i++)
        {
            seg = segments.at(i);
            switch (seg->getType())
            {
            case cmpRiverSegment::Dam:
                dam = static_cast<cmpDam *> (segments.at(i));
                dam->writeData(cfile, 1, outputAll);
                break;
            case cmpRiverSegment::Reach:
                reach = static_cast<cmpReach *> (segments.at(i));
                reach->writeData(cfile, 1, outputAll);
                break;
            case cmpRiverSegment::Headwater:
                hwater = static_cast<cmpHeadwater *>(segments.at(i));
                hwater->writeData(cfile, 1, outputAll);
            }
            cfile->writeNewline();
        }
    }
    else
    {
        okay = false;
    }

    return okay;
}

