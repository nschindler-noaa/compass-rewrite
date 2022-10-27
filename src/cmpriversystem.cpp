#include "cmpriversystem.h"

#include "cmpheadwater.h"

cmpRiverSystem::cmpRiverSystem(QObject *parent) :
    QObject(parent)
{
    setup ();
//    cmpRiverSystem = this;
}

cmpRiverSystem::cmpRiverSystem(QString riverFile, QObject *parent) :
    QObject(parent)
{
    cmpFile cf(riverFile);
    setup();
//    cmpRiverSystem = this;
    parseDesc (&cf);
}


void cmpRiverSystem::setup ()
{
}

cmpRiverSystem::~cmpRiverSystem ()
{
    deleteAll ();
}

void cmpRiverSystem::deleteAll()
{
//    int i;
    while (rivers.count())
        delete rivers.takeLast();

    while (species.count())
        delete species.takeLast();

    while (stocks.count())
        delete stocks.takeLast();
//    while (releases->count())
//        delete releases->takeLast();

    while (releaseSites.count())
        delete (releaseSites.takeLast());
}

bool cmpRiverSystem::parseDesc(cmpFile *descfile)
{
    bool okay = true, end = false;
    QString token (""), val ("");
    QString name;
    cmpReleaseSite *newrelsite = nullptr;
    cmpRiver *river = nullptr;

    while (okay && !end)
    {
        token = descfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            descfile->printMessage("Found EOF in Compass file.");
            end = true;
        }
        else if (token.compare ("species") == 0)
        {
            okay = descfile->readString(name);
            speciesNames.append(name);
        }
        else if (token.compare ("stock") == 0)
        {
            okay = descfile->readString(name);
            stockNames.append(name);
        }
        else if (token.compare ("release_site") == 0)
        {
            okay = descfile->readString(name);
            newrelsite = new cmpReleaseSite(name);
            newrelsite->parseDesc(descfile);
            releaseSites.append(newrelsite);
        }
        else if (token.compare ("river") == 0)
        {
            river = new cmpRiver(this);
            okay = descfile->readString(name);
            river->setName(name);
            okay = river->parseDesc(descfile);

            rivers.append(river);
        }
    }
    return okay;
}

bool cmpRiverSystem::parse(cmpFile *rfile)
{
    bool okay = true, end = false;
    QString token (""), val ("");
    QString name;
    cmpRiver *river = nullptr;

    while (okay && !end)
    {
        token = rfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            rfile->printMessage("Found EOF in Compass file.");
            end = true;
        }
        else if (token.compare ("species") == 0)
        {
            okay = rfile->readString(name);
            speciesNames.append(name);
        }
        else if (token.compare ("stock") == 0)
        {
            okay = rfile->readString(name);
            stockNames.append(name);
        }
        else if (token.compare ("release_site") == 0)
        {
            okay = rfile->readString(name);
            cmpReleaseSite *newrelsite = new cmpReleaseSite(name);
            newrelsite->parseDesc(rfile);
            releaseSites.append(newrelsite);
        }
        else if (token.compare ("river") == 0)
        {
            token = rfile->popToken ();
            okay = rfile->readString(name);
            river = new cmpRiver(name);
            okay = river->parseDesc(rfile);

            rivers.append(river);
        }
        else if (token.compare ("reach") == 0)
        {
            QString reachName ("");
            okay = rfile->readString (reachName);
            if (okay)
            {
                if (reaches.contains(reachName))
                {
//                    cmpReach *reach = static_cast<cmpReach *>(findSegment(reachName));
//                    okay = reach->parse (rfile);
                }
                else
                {
//                    Log::outlog->add(Log::Error, "No reach by this name");
//                    Log::outlog->add(Log::Error, reachName);
                }
            }
        }
        else if (token.compare ("dam") == 0)
        {
            QString damName ("");
            okay = rfile->readString (damName);
            if (okay)
            {
                if (dams.contains(damName))
                {
//                    cmpDam *dam = static_cast<cmpDam *>(findSegment(damName));
//                    okay = dam->parse (rfile);
                }
                else
                {
//                    Log::outlog->add(Log::Error, "No dam by this name");
//                    Log::outlog->add(Log::Error, damName);
                }
            }
        }
        else if (token.compare ("headwater") == 0)
        {
            QString hwName ("");
            okay = rfile->readString (hwName);
            if (okay)
            {
                if (headwaters.contains(hwName))
                {
                    cmpHeadwater *hwtr = static_cast<cmpHeadwater *>(findSegment(hwName));
                    okay = hwtr->parse (rfile);
                }
                else
                {
 //                   Log::outlog->add(Log::Error, "No headwater by this name");
 //                   Log::outlog->add(Log::Error, hwName);
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
//            rfile->handle_unknown_token (token);
            rfile->skipLine ();
        }
    }
    return okay;
}

bool cmpRiverSystem::parseReleaseSite(cmpFile *cfile, cmpReleaseSite *relsite)
{
    bool okay = true;
    QString line;
    cmpRiverPoint *rivpt = new cmpRiverPoint();
    okay = cfile->readString(line);
    rivpt->parse(line);
    relsite->setLatlon(rivpt);
    releaseSites.append(relsite);
    okay = cfile->checkEnd ("release_site", relsite->getName());
    rivpt = nullptr;
    return true;
}

bool cmpRiverSystem::outputDesc(cmpFile *descfile)
{
    bool okay = true, end = false;
    QString token (""), val ("");

    if (descfile->open(QIODevice::WriteOnly))
    {
        // output river system values
        for (int i = 0; i < speciesNames.count(); i++)
            descfile->writeString(0, "species", speciesNames.at(i));
        descfile->writeNewline();
        for (int i = 0; i < stockNames.count(); i++)
            descfile->writeString(0, "stock", stockNames.at(i));
        descfile->writeNewline();
        for (int i = 0; i < releaseSites.count(); i++)
            releaseSites.at(i)->outputDesc(descfile);

        // output rivers
        for (int i = 0; i < rivers.count(); i++)
        {
            rivers.at(i)->outputDesc(descfile);
//            outputRiver(rivers->at(i), descfile);
            descfile->writeNewline();
        }
    }
    else
    {
        okay = false;
    }
    return okay;
}

bool cmpRiverSystem::output(cmpFile *cfile)
{
    bool okay = true;
    if (cfile->open(QIODevice::WriteOnly))
    {
        for (int i = 0; i < rivers.count(); i++)
        {
            rivers.at(i)->output(cfile);
        }
    }
    else {
        okay = false;
    }
    return okay;
}

bool cmpRiverSystem::construct()
{
    bool okay = true;
    cmpRiver *riv = nullptr;
    cmpRiverSegment *cur = nullptr;
    cmpRiverSegment *prev = nullptr;
    QString curRiver ("");

    if (segments.count() < 2)
        okay = false;

    if (okay) {
        prev = static_cast<cmpRiverSegment *> (segments.at (0));
        curRiver = QString (prev->getRiverName());
        for (int i = 1; okay && i < segments.count(); i++)
        {
            cur = static_cast<cmpRiverSegment *> (segments.at(i));
            if (curRiver.compare(cur->getRiverName()) != 0)
            {
                // create headwater of previous river, if it doesn't exist
                if (prev->getType() != cmpRiverSegment::Headwater)
                {
                    QString hname (curRiver);
                    hname.append(" Headwater");
                    prev->setUpperSegment(new cmpHeadwater (hname, curRiver));
                    prev->getUpperSegment()->setLowerSegment(prev);
                    segments.insert (i, prev->getUpperSegment());
                    i++;
                    headwaters.append(hname);
//                    Log::outlog->add(Log::Debug, QString (
//                              QString("adding headwater %1").arg(hname)));
                }
                // change rivers, if exist
                curRiver = QString (cur->getRiverName());
                riv = findRiver(curRiver);
                if (riv == nullptr)
                {
//                    Log::outlog->add (Log::Error, QString (
//                               QString("cmpRiver name %1 not found when constructing system").arg (
//                                   curRiver)));
                    okay = false;
                }
                else
                {
                    // find previous segment by top latlon
                    prev = findSegment (cur->getBottomPoint());
                    if (prev == nullptr)
                    {
                        okay = false;
//                        Log::outlog->add(Log::Error, QString ("faulty cmpRiver construction."));
                    }
                    else
                    {
                        prev->setForkSegment(cur);
                        cur->setLowerSegment(prev);
                    }
                }
            }
            else
            {
                prev->setUpperSegment(cur);
                cur->setLowerSegment(prev);
            }
            prev = cur;
        }
    }
    return okay;
}

bool cmpRiverSystem::initialize()
{
    bool okay = true;

    return okay;
}

cmpRiver * cmpRiverSystem::findRiver(QString name)
{
    cmpRiver *riv = nullptr;
    for (int i = 0; i < rivers.count(); i++)
    {
        riv = rivers.at (i);
        if (riv->getName().compare(name) == 0)
            break;
    }
    return riv;
}

cmpRiverSegment * cmpRiverSystem::findSegment(QString name)
{
    cmpRiverSegment *seg = nullptr;
    for (int i = 0; i < segments.count(); i++)
    {
        seg = segments.at (i);
        if (seg->getName().compare(name) == 0)
            break;
    }
    return seg;
}

cmpRiverSegment * cmpRiverSystem::findSegment(cmpRiverPoint *pt)
{
    cmpRiverSegment *seg = nullptr;
    bool found = false;
    for (int i = 0; !found && i < segments.count(); i++)
    {
        seg = segments.at (i);
        for (int j = 0; !found && j < seg->getCourse().count(); j++)
            if (seg->getCourse().at (j)->equals(*pt))
                found = true;
    }
    return seg;
}

cmpSpecies * cmpRiverSystem::findSpecies(QString name)
{
    cmpSpecies *spec = nullptr;
    for (int i = 0; i < species.count(); i++)
    {
        spec = species.at (i);
        if (spec->getName().compare(name) == 0)
            break;
    }
    return spec;
}

cmpStock * cmpRiverSystem::findStock(QString name)
{
    cmpStock *st = nullptr;
    for (int i = 0; i < stocks.count(); i++)
    {
        st = stocks.at (i);
        if (st->getName()->compare(name) == 0)
            break;
    }
    return st;
}

cmpTransport * cmpRiverSystem::findTransport(QString name)
{
//    cmpDam *dm = static_cast<cmpDam *> (findSegment(name));

    return nullptr; //dm->getTransport();
}

cmpRelease * cmpRiverSystem::findRelease(QString name)
{
    cmpRelease *rel = nullptr;
    for (int i = 0; i < releases.count(); i++)
    {
        rel = releases.at (i);
        if (rel->getName().compare(name) == 0)
            break;
    }
    return rel;
}

cmpReleaseSite * cmpRiverSystem::findReleaseSite(QString name)
{
    cmpReleaseSite *site = nullptr;
    for (int i = 0, total = releaseSites.count(); i < total; ++i)
    {
        site = releaseSites.at (i);
        if (site->getName().compare(name) == 0)
            break;
    }
    return site;
}

void cmpRiverSystem::computeFlows ()
{
    markRegulationPts ();
    fillHeadwaters ();
    computeSegFlow (getMouth ());
}
void cmpRiverSystem::markRegulationPts()
{
    for (int i = 0, total = dams.count(); i < total; ++i)
    {
        QString damname = dams.at (i);
        cmpDam *dam = static_cast <cmpDam *> (findSegment(damname));
        dam->setIsRegPoint(true);
    }
}

/** Upstream flow propogation (to headwaters) */

void cmpRiverSystem::fillHeadwaters ()
{
    QString hwtrname ("");
    cmpHeadwater *hwtr;

    for (int i = 0, total = headwaters.count(); i < total; ++i)
    {
        hwtrname = headwaters.at (i);
        hwtr = static_cast <cmpHeadwater *> (findSegment (hwtrname));
        hwtr->fillRegulated();
    }
    for (int i = 0, total = headwaters.count(); i < total; ++i)
    {
        hwtrname = headwaters.at (i);
        hwtr = static_cast <cmpHeadwater *> (findSegment (hwtrname));
        hwtr->fillUnRegulated();
    }
}

/** Downstream flow propogation. Headwaters are always done first, then
 *  the downstream segments.  */

void cmpRiverSystem::computeSegFlow (cmpRiverSegment *seg)
{
/*    if (seg->getUpperSegment() != nullptr)
    {
            computeSegFlow(seg->getUpperSegment());
        if (seg->getForkSegment() != nullptr)
            computeSegFlow(seg->getForkSegment());

        if (seg->getType() == cmpRiverSegment::cmpDam)
            static_cast <cmpDam *> (seg)->calculateFlow();
        else if (seg->getType() == cmpRiverSegment::cmpReach)
            static_cast <cmpReach *> (seg)->calculateFlow();
    }
    else if (seg->getType() == cmpRiverSegment::Headwater)
        static_cast <cmpHeadwater *> (seg)->calculateFlow();
    else
    {
        QString msg (QString ("Segment %1 is not headwater and has no upstream segment.").arg (*seg->getName()));
        Log::outlog->add(Log::Error, msg);
        return;
    }*/
}

/** Downstream temperature propogation. Headwaters are always done first, then
 *  the downstream segments. */

void cmpRiverSystem::computeTemps ()
{
    getMouth()->calculateTemps();
}

void cmpRiverSystem::computeSegTemp (cmpRiverSegment *seg)
{
/*    if (seg->getUpperSegment() != nullptr)
    {
        computeSegTemp(seg->getUpperSegment());
        if (seg->getForkSegment() != nullptr)
            computeSegTemp(seg->getForkSegment());

        if (!seg->getReadTemps())
        {
            if (seg->getType() == cmpRiverSegment::cmpDam)
                static_cast <cmpDam *> (seg)->calculateTemp ();
            else if (seg->getType() == cmpRiverSegment::cmpReach)
                static_cast <cmpReach *> (seg)->calculateTemp ();
        }
    }
    else if (seg->getType() == cmpRiverSegment::Headwater)
    {
        if (!seg->getReadTemps())
            static_cast <Headwater *> (seg)->calculateTemp ();
    }
    else
    {
        QString msg (QString ("Segment %1 is not headwater and has no upstream segment.").arg (*seg->name));
        Log::outlog->add(Log::Error, msg);
    }*/
}

void cmpRiverSystem::computeSpill ()
{
//    computeSegSpill (getMouth());
    for (int i = 0, total = segments.count(); i < total; ++i)
    if (segments.at (i)->getType() == cmpRiverSegment::Dam)
    {
        static_cast <cmpDam *> (segments.at (i))->calculateSpill ();
    }
}

void cmpRiverSystem::computeSegSpill(cmpRiverSegment *seg)
{
    if (seg->getType() == cmpRiverSegment::Dam)
    {
        static_cast <cmpDam *> (seg)->calculateSpill ();
    }
}

void cmpRiverSystem::deleteReleases ()
{

}

void cmpRiverSystem::deleteSpill ()
{
    for (int i = 0, total = segments.count(); i < total; ++i)
    if (segments.at (i)->getType() == cmpRiverSegment::Dam)
    {
        static_cast <cmpDam *> (segments.at (i))->deleteSpill ();
    }
 }

