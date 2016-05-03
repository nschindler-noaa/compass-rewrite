#include "RiverSystem.h"
#include "Headwater.h"
#include "Reach.h"
#include "Dam.h"
#include "Log.h"
#include "parseUtil.h"
#include "writeUtil.h"

RiverSystem *riverSystem = NULL;

River::River (QObject *parent) :
    QObject (parent)
{
    name = new QString ("");
    flowMax = 0.0;
    flowMin = 0.0;
}

River::River (QString rname, QObject *parent) :
    QObject (parent)
{
    name = new QString (rname);
    flowMax = 0.0;
    flowMin = 0.0;
}

River::~River ()
{
    delete name;
}

RiverSystem::RiverSystem(QObject *parent) :
    QObject(parent)
{
    setup ();
    riverSystem = this;
}

RiverSystem::RiverSystem(QString riverFile, QObject *parent) :
    QObject(parent)
{
    setup();
    riverSystem = this;
}

void RiverSystem::setup ()
{
    rivers = new QList<River *>();
    segments = new QList <RiverSegment *>();
    species = new QList <Species *>();
    stocks = new QList <Stock *>();
    transports = new QList <Transport *>();
    releases = new QList <Release *>();
    releaseSites = new QList <ReleaseSite *>();

    speciesNames = new QStringList();
    stockNames = new QStringList();
    powerhouses = new QStringList();
    dams = new QStringList();
    reaches = new QStringList();
    headwaters = new QStringList();
    basins = new QStringList();
}

RiverSystem::~RiverSystem ()
{
    deleteAll ();
}

void RiverSystem::deleteAll()
{
    int i;
    while (rivers->count())
        delete rivers->takeLast();
    delete rivers;

    while (species->count())
        delete species->takeLast();
    delete species;

    while (stocks->count())
        delete stocks->takeLast();
    delete stocks;
    while (releases->count())
        delete releases->takeLast();
    delete releases;
    while (releaseSites->count())
        delete (releaseSites->takeLast());
    delete releaseSites;

    delete speciesNames;
    delete stockNames;
    delete powerhouses;
    delete dams;
    delete reaches;
    delete headwaters;
    delete basins;

}

bool RiverSystem::parseDesc(CompassFile *rfile)
{
    bool okay = true;
    if (rfile->isReadable())
        parse (rfile);

    return okay;
}

bool RiverSystem::parse(CompassFile *rfile)
{
    bool okay = true, end = false;
    QString token (""), val ("");

//    River *river = new River (rivname);
//    compassInfo->rivers.append (river);

    while (okay && !end)
    {
        token = rfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            rfile->printMessage("Found EOF in Compass file.");
            end = true;
        }
        else if (token.compare ("flow_max") == 0)
        {
            token = rfile->popToken ();
//            okay = read_float (token, &river->flowMax, "Flow max");
        }
        else if (token.compare ("reach") == 0)
        {
            QString reachName ("");
            okay = rfile->readString (reachName);
            if (okay)
            {
                if (reaches->contains(reachName))
                {
                    Reach *reach = (Reach *)findSegment(reachName);
                    okay = reach->parse (rfile);
                }
                else
                {
                    Log::instance()->add(Log::Error, "No reach by this name");
                    Log::instance()->add(Log::Error, reachName);
                }
            }
        }
        else if (token.compare ("dam") == 0)
        {
            QString damName ("");
            okay = rfile->readString (damName);
            if (okay)
            {
                if (dams->contains(damName))
                {
                    Dam *dam = (Dam *)findSegment(damName);
                    okay = dam->parse (rfile);
                }
                else
                {
                    Log::instance()->add(Log::Error, "No dam by this name");
                    Log::instance()->add(Log::Error, damName);
                }
            }
        }
        else if (token.compare ("headwater") == 0)
        {
            QString hwName ("");
            okay = rfile->readString (hwName);
            if (okay)
            {
                if (headwaters->contains(hwName))
                {
                    Headwater *hwtr = (Headwater *)findSegment(hwName);
                    okay = hwtr->parse (rfile);
                }
                else
                {
                    Log::instance()->add(Log::Error, "No headwater by this name");
                    Log::instance()->add(Log::Error, hwName);
                }
            }
        }
        else if (token.contains ("end"))
        {
            rfile->skipLine ();
            end = true;
//            cfile->checkEnd (*river->name);
        }
        else
        {
            handle_unknown_token (token);
            rfile->skipLine ();
        }
    }
    return okay;
}

bool RiverSystem::construct()
{
    bool okay = true;
    River *riv = NULL;
    RiverSegment *cur = NULL;
    RiverSegment *prev = NULL;
    QString curRiver ("");

    prev = (RiverSegment *) segments->at (0);
    curRiver = QString (*prev->riverName);
    for (int i = 1; okay && i < segments->count(); i++)
    {
        cur = (RiverSegment *) segments->at(i);
        if (curRiver.compare(*cur->riverName) != 0)
        {
            // create headwater of previous river, if it doesn't exist
            if (prev->type != RiverSegment::Headwater)
            {
                QString hname (curRiver);
                hname.append(" Headwater");
                prev->up = new Headwater (hname, curRiver);
                prev->up->down = prev;
                segments->insert (i, prev->up);
                i++;
                headwaters->append(hname);
                Log::instance()->add(Log::Debug, QString (
                          QString("adding headwater %1").arg(hname)));
            }
            // change rivers, if exist
            curRiver = QString (*cur->riverName);
            riv = findRiver(curRiver);
            if (riv == NULL)
            {
                Log::instance ()->add (Log::Error, QString (
                           QString("River name %1 not found when constructing system").arg (
                               curRiver)));
                okay = false;
            }
            else
            {
                // find previous segment by top latlon
                prev = findSegment (cur->bottomPoint());
                if (prev == NULL)
                {
                    okay = false;
                    Log::instance()->add(Log::Error, QString ("faulty River construction."));
                }
                else
                {
                    prev->fork = cur;
                    cur->down = prev;
                }
            }
        }
        else
        {
            prev->up = cur;
            cur->down = prev;
        }
        prev = cur;
    }
    // we have ended, but is it at a headwater?
    // create headwater of this river, if it doesn't exist
    if (cur->type != RiverSegment::Headwater)
    {
        QString hname (curRiver);
        hname.append(" Headwater");
        cur->up = new Headwater (hname, curRiver);
        cur->up->down = cur;
        segments->append (cur->up);
        headwaters->append(hname);
        Log::instance()->add(Log::Debug, QString (
                  QString("adding headwater %1").arg(hname)));
    }

    return okay;
}

bool RiverSystem::initialize()
{
    bool okay = true;

    return okay;
}

River * RiverSystem::findRiver(QString name)
{
    River *riv = NULL;
    for (int i = 0; i < rivers->count(); i++)
    {
        riv = rivers->at (i);
        if (riv->name->compare(name) == 0)
            break;
    }
    return riv;
}

RiverSegment * RiverSystem::findSegment(QString name)
{
    RiverSegment *seg = NULL;
    for (int i = 0; i < segments->count(); i++)
    {
        seg = segments->at (i);
        if (seg->name->compare(name) == 0)
            break;
    }
    return seg;
}

RiverSegment * RiverSystem::findSegment(RiverPoint *pt)
{
    RiverSegment *seg = NULL;
    bool found = false;
    for (int i = 0; !found && i < segments->count(); i++)
    {
        seg = segments->at (i);
        for (int j = 0; !found && j < seg->course.count(); j++)
            if (seg->course.at (j)->equals(*pt))
                found = true;
    }
    return seg;
}

Species * RiverSystem::findSpecies(QString name)
{
    Species *spec = NULL;
    for (int i = 0; i < species->count(); i++)
    {
        spec = species->at (i);
        if (spec->name->compare(name) == 0)
            break;
    }
    return spec;
}

Stock * RiverSystem::findStock(QString name)
{
    Stock *st = NULL;
    for (int i = 0; i < stocks->count(); i++)
    {
        st = stocks->at (i);
        if (st->name->compare(name) == 0)
            break;
    }
    return st;
}

Transport * RiverSystem::findTransport(QString name)
{
    Dam *dm = (Dam *)findSegment(name);

    return dm->transport;
}

Release * RiverSystem::findRelease(QString name)
{
    Release *rel = NULL;
    for (int i = 0; i < releases->count(); i++)
    {
        rel = releases->at (i);
        if (rel->name->compare(name) == 0)
            break;
    }
    return rel;
}

ReleaseSite * RiverSystem::findReleaseSite(QString name)
{
    ReleaseSite *site = NULL;
    for (int i = 0; i < releaseSites->count(); i++)
    {
        site = releaseSites->at (i);
        if (site->name->compare(name) == 0)
            break;
    }
    return site;
}

void RiverSystem::computeFlows ()
{
    markRegulationPts ();
    fillHeadwaters ();
    computeSegFlow (mouth ());
}
void RiverSystem::markRegulationPts()
{
    for (int i = 0; i < dams->count(); i++)\
    {
        QString damname = (QString)dams->at (i);
        Dam *dam = static_cast <Dam *> (findSegment(damname));
        dam->regPoint = true;
    }
}

/** Upstream flow propogation (to headwaters) */

void RiverSystem::fillHeadwaters ()
{
    QString hwtrname ("");
    Headwater *hwtr;

    for (int i = 0; i < headwaters->count(); i++)
    {
        hwtrname = (QString)(headwaters->at (i));
        hwtr = static_cast <Headwater *> (findSegment (hwtrname));
        hwtr->fillRegulated();
    }
    for (int i = 0; i < headwaters->count(); i++)
    {
        hwtrname = (QString)(headwaters->at (i));
        hwtr = static_cast <Headwater *> (findSegment (hwtrname));
        hwtr->fillUnRegulated();
    }
}

/** Downstream flow propogation. Headwaters are always done first, then
 *  the downstream segments.  */

void RiverSystem::computeSegFlow (RiverSegment *seg)
{
    if (seg->up != NULL)
    {
            computeSegFlow(seg->up);
        if (seg->fork != NULL)
            computeSegFlow(seg->fork);

        if (seg->type == RiverSegment::Dam)
            static_cast <Dam *> (seg)->calculateFlow();
        else if (seg->type == RiverSegment::Reach)
            static_cast <Reach *> (seg)->calculateFlow();
    }
    else if (seg->type == RiverSegment::Headwater)
        static_cast <Headwater *> (seg)->calculateFlow();
    else
    {
        QString msg (QString ("Segment %1 is not headwater and has no upstream segment.").arg (*seg->name));
        Log::instance()->add(Log::Error, msg);
        return;
    }
}

/** Downstream temperature propogation. Headwaters are always done first, then
 *  the downstream segments. */

void RiverSystem::computeTemps ()
{
    mouth()->calculateTemps();
}

void RiverSystem::computeSegTemp (RiverSegment *seg)
{
    if (seg->up != NULL)
    {
        computeSegTemp(seg->up);
        if (seg->fork != NULL)
            computeSegTemp(seg->fork);

        if (!seg->read_temps)
        {
            if (seg->type == RiverSegment::Dam)
                static_cast <Dam *> (seg)->calculateTemp ();
            else if (seg->type == RiverSegment::Reach)
                static_cast <Reach *> (seg)->calculateTemp ();
        }
    }
    else if (seg->type == RiverSegment::Headwater)
    {
        if (!seg->read_temps)
            static_cast <Headwater *> (seg)->calculateTemp ();
    }
    else
    {
        QString msg (QString ("Segment %1 is not headwater and has no upstream segment.").arg (*seg->name));
        Log::instance()->add(Log::Error, msg);
    }
}

void RiverSystem::computeSpill ()
{
    computeSegSpill (mouth());
}

void RiverSystem::computeSegSpill(RiverSegment *seg)
{
    for (int i = 0; i < segments->count(); i++)
    if (segments->at (i)->type == RiverSegment::Dam)
    {
        static_cast <Dam *> (segments->at (i))->calculateSpill ();
    }
}

void RiverSystem::deleteReleases ()
{

}

void RiverSystem::deleteSpill ()
{
    for (int i = 0; i < segments->count(); i++)
    if (segments->at (i)->type == RiverSegment::Dam)
    {
        static_cast <Dam *> (segments->at (i))->deleteSpill ();
    }
 }
