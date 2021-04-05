#include "parseUtil.h"
#include "information.h"
#include "Basin.h"


/** The main entry point for reading any file. This will call the
 *  appropriate parsing functions. */

Information * compassInfo = newInfo ();

bool parseCompassFile (QString filename)
{
    bool okay = true;

    CompassFile *infile = new CompassFile (filename);
    infile->open(QIODevice::ReadOnly);

    okay = infile->readHeader ();
    okay = infile->readInfo ();
    okay = parseCompassData (infile);
    if (!okay)
        Log::outlog->add(Log::Error, QString ("Error in reading file %1").arg (filename));

//    delete infile;

    return okay;
}

bool parseCompassData (CompassFile *cfile)
{
    bool okay = true, end = false;
    QString token (""), val ("");

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            okay = false;
            cfile->printEOF ("Compass data.");
        }

    }
    return okay;
}

bool parse_river (CompassFile *cfile, RiverSystem *rs, River *river)
{
    bool okay = true, end = false;
    QString token (""), val ("");
    float tempFloat = 0;
    int tempInt = 0;

//    River *river = new River (rivname);
//    compassInfo->rivers.append (river);

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            cfile->printEOF ("river data");
            okay = false;
        }
        else if (token.compare ("flow_max") == 0)
        {
            token = cfile->popToken ();
            okay = read_float (token, &tempFloat, "Flow max");
            river->setFlowMax(tempFloat);
        }
        else if (token.compare ("flow_min") == 0)
        {
            okay = read_float (cfile->popToken (), &tempFloat, QString("Flow min"));
            river->setFlowMin(tempFloat);
        }
        else if (token.compare ("reach") == 0)
        {
            QString reachName ("");
            okay = cfile->readString (reachName);
            if (okay)
            {
                Reach *reach = new Reach (reachName);
                rs->reaches->append (reachName);
                rs->segments->append ((RiverSegment *) reach);
                reach->setRiverName(&river->getName());
                okay = parse_reach (cfile, reach);
                river->addSegment(reach);
            }
        }
        else if (token.compare ("dam") == 0)
        {
            QString damName ("");
            okay = cfile->readString (damName);
            if (okay)
            {
                Dam *dam = new Dam (damName);
                rs->dams->append (damName);
                rs->segments->append ((RiverSegment *) dam);
                dam->setRiverName(&river->getName());
                okay = parse_dam (cfile, dam);
                river->addSegment(dam);
            }
        }
        else if (token.compare ("headwater") == 0)
        {
            QString hwName ("");
            okay = cfile->readString (hwName);
            if (okay)
            {
                Headwater *head = new Headwater (hwName);
                rs->headwaters->append (hwName);
                rs->segments->append ((RiverSegment *) head);
                head->setRiverName(&river->getName());
                okay = parse_headwater (cfile, head);
                river->addSegment(head);
            }
        }
        else if (token.contains ("end"))
        {
            cfile->skipLine ();
            end = true;
//            cfile->checkEnd (*river->name);
        }
        else
        {
            handle_unknown_token (token);
            cfile->skipLine ();
        }
    }
    // we have ended, but is it at a headwater?
    // create headwater of this river, if it doesn't exist
    RiverSegment *cur = rs->segments->last();
    if (okay && cur->getType() != RiverSegment::Headwater)
    {
        QString hname (river->getName());
        hname.append(" Headwater");
        cur->setUpperSegment(new Headwater (hname, river->getName()));
        cur->getUpperSegment()->setLowerSegment(cur);
        rs->segments->append (cur->getUpperSegment());
        rs->headwaters->append(hname);
        Log::outlog->add(Log::Debug, QString (
                  QString("adding headwater %1").arg(hname)));
    }
    return okay;
}

bool parse_dam (CompassFile *cfile, Dam *dam)
{
    bool okay = true, end = false;
    bool tempBool = false;
    float tempFloat = 0;
    int tempInt = 0;
    QString token (""), val ("");
    QStringList tokenlist;
    float temp = 0.0;
    int index = 0;

    Log::outlog->add (Log::Debug, QString("parsing ......... %1").arg (*dam->getName()));

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.contains ("EOF"))
        {
            okay = false;
        }
        else if (token.compare ("latlon", Qt::CaseInsensitive) == 0)
        {
            RiverPoint *pt = new RiverPoint ();
            okay = parse_latlon (cfile, pt);
            if (okay)
                dam->addCoursePoint(pt);
        }
        else if (token.compare ("abbrev", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            dam->setAbbrev(new QString (val));
            cfile->skipLine ();
        }
        else if (token.compare ("width", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat, QString("Spillway width"));
            dam->getSpillway()->setWidth(tempFloat);
        }
        else if (token.compare ("floor_elevation", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat, QString("Floor elevation"));
            dam->setElevBase(tempFloat);
        }
        else if (token.compare ("forebay_elevation", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat, QString("Forebay elevation"));
            dam->setElevForebay(tempFloat);
        }
        else if (token.compare ("bypass_elevation", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat, QString("Forebay elevation"));
            dam->setHeightBypass(tempFloat - dam->getElevBase());
            if (tempFloat < 0.0)
                dam->setHeightBypass(0.0);
        }
        else if (token.compare ("tailrace_elevation", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat,
                           QString("Tailrace elevation"));
            dam->setElevTailrace(tempFloat);
        }
        else if (token.compare ("spillway_width", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat,
                           QString("Spillway width"));
            dam->getSpillway()->setWidth(tempFloat);
        }
        else if (token.compare ("spill_side", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken ();
            dam->setSpillSideText(val);
        }
        else if (token.compare ("ngates", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_int (val, &tempInt,
                    QString("Number of spillway gates"));
            dam->getSpillway()->setNumGates(tempInt);
        }
        else if (token.compare ("gate_width", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat,
                    QString("Spillway gate width"));
            dam->getSpillway()->setGateWidth(tempFloat);
        }
        else if (token.compare ("pergate", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat,
                    QString("Spillway per gate"));
            dam->getSpillway()->setPerGate(tempFloat);
        }
        else if (token.contains ("powerhouse", Qt::CaseInsensitive))
        {
            tempInt = 0;
            tokenlist = token.split('_');
            if (tokenlist.count() > 2)
                tempInt = tokenlist.at(1).toInt() - 1;
            val = cfile->popToken ();
            read_float (val, &tempFloat, QString ("powerhouse capacity"));
            PowerHouse *phouse = new PowerHouse(tempInt);

            index = dam->setPowerhouse(phouse, tempInt);
            Log::outlog->add(Log::Debug,QString ("powerhouse index %1").arg(
                                     QString::number(tempInt)));

            phouse->setCapacity(tempFloat);
/*            if (token.contains ("capacity"), Qt::CaseInsensitive)
            {
                phouse->capacity = temp;
                okay = read_float (val, &temp, QString("powerhouse capacity"));
                if (okay)
                {
                    Log::outlog->add(Log::Debug, QString(QString ("Powerhouse capacity %1").arg(
                                QString::number(phouse->capacity))));
                }
            }*/
        }
        else if (token.compare ("storage_basin", Qt::CaseInsensitive) == 0)
        {
            Basin *bsn = new Basin ();
            dam->setBasin(bsn);
            val = cfile->popToken();

            okay = read_float (val, &tempFloat,// &dam->basin->volumeMin,
                       QString("Dam basin minimum volume"));
            dam->getBasin()->setVolumeMin(tempFloat);
            val = cfile->popToken();
            okay = read_float (val, &tempFloat,//&dam->basin->volumeMax,
                       QString("Dam basin maximum volume"));
            dam->getBasin()->setVolumeMax(tempFloat);
        }
        else if (token.compare ("basin_length", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat,
                       QString("Stilling basin length"));
            dam->setLengthBasin(tempFloat);
        }
        else if (token.compare ("sgr", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &tempFloat,
                       QString("Specific gravity"));
            dam->setSpecGrav(tempFloat);
        }
        else if (token.compare ("fishway", Qt::CaseInsensitive) == 0)
        {
            parse_fishway (cfile, dam);
        }
        else if (token.contains ("end"))
        {
            if (cfile->checkEnd ("dam", *dam->getName())) {
                cfile->skipLine ();
                end = true;
            }
        }
        else if (token.startsWith('#'))
        {
            cfile->skipLine ();
        }
        else
        {
            okay = dam->parseToken(token, cfile);
            handle_unknown_token (token);
            cfile->skipLine ();
        }
    }
    okay = check_course (dam);
    return okay;
}

bool parse_fishway (CompassFile *cfile, Dam *dam)
{
    bool okay = true, end = false;
    QString token(""), val("");
    int temp_int = 0;

    while (okay && !end)
    {
        token = cfile->popToken();
        if (token.contains ("EOF"))
        {
            okay = false;
        }

        else if (token.contains ("type", Qt::CaseInsensitive))
        {
            temp_int = cfile->popToken().toInt(&okay);
            if (temp_int >= dam->getNumFishways())
                dam->setNumFishways(temp_int + 1);

            token = cfile->popToken();
            dam->getFishway(temp_int)->setType(token);
        }
        else if (token.contains ("length", Qt::CaseInsensitive))
        {
            temp_int = cfile->popToken().toInt(&okay);
            if (temp_int >= dam->getNumFishways())
                dam->setNumFishways(temp_int + 1);

            token = cfile->popToken();
            dam->getFishway(temp_int)->setLength(token.toFloat());
        }
        else if (token.contains ("capacity", Qt::CaseInsensitive))
        {
            temp_int = cfile->popToken().toInt(&okay);
            if (temp_int >= dam->getNumFishways())
                dam->setNumFishways(temp_int + 1);

            token = cfile->popToken();
            dam->getFishway(temp_int)->setCapacity(token.toFloat());
        }
        else if (token.contains ("velocity", Qt::CaseInsensitive))
        {
            temp_int = cfile->popToken().toInt(&okay);
            if (temp_int >= dam->getNumFishways())
                dam->setNumFishways(temp_int + 1);

            token = cfile->popToken();
            dam->getFishway(temp_int)->setVelocity(token.toFloat());
        }
        else if (token.contains ("end"))
        {
            if (cfile->checkEnd ("fishway")) {
                cfile->skipLine ();
                end = true;
            }
        }
        else if (token.startsWith('#'))
        {
            cfile->skipLine ();
        }
        else
        {
            handle_unknown_token (token);
            cfile->skipLine ();
        }
    }
    return okay;
}

bool parse_reach (CompassFile *cfile, Reach *rch)
{
    bool okay = true, end = false;
    bool tempBool = false;
    float tempFloat = 0;
    int tempInt = 0;
    QString token (""), val ("");

    Log::outlog->add (Log::Debug, QString("parsing ......... %1").arg (*rch->getName()));

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.contains ("EOF"))
        {
            okay = false;
        }
        else if (token.compare ("latlon", Qt::CaseInsensitive) == 0)
        {
            RiverPoint *pt = new RiverPoint ();
            okay = parse_latlon (cfile, pt);
            if (okay)
                okay = rch->addCoursePoint(pt);
        }
        else if (token.compare ("abbrev", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            rch->setAbbrev(new QString (val));
            cfile->skipLine ();
        }
        else if (token.compare ("width", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &tempFloat,
                               QString("Width"));
            rch->setWidthAve(tempFloat);
        }
        else if (token.compare ("depth", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &tempFloat,
                               QString("Depth"));
            rch->setDepth(tempFloat);
        }
        else if (token.compare ("lower_depth", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &tempFloat,
                               QString("Lower depth"));
            rch->setDepthLower(tempFloat);
        }
        else if (token.compare ("upper_depth", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &tempFloat,
                               QString("Upper depth"));
            rch->setDepthUpper(tempFloat);
        }
        else if (token.compare ("lower_elev", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &tempFloat,
                               QString("Lower elevation"));
            rch->setElevLower(tempFloat);
        }
        else if (token.compare ("slope", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &tempFloat,
                               QString("Slope"));
            rch->setSlope(tempFloat);
        }
        else if (token.contains ("end"))
        {
//            cfile->checkEnd (*rch->name);
            cfile->skipLine ();
            end = true;
        }
        else if (token.startsWith('#'))
        {
            cfile->skipLine ();
        }
        else
        {
            handle_unknown_token (token);
            cfile->skipLine ();
        }
    }
    okay = check_course (rch);
    if (!end) okay = false;
    return okay;
}

bool parse_headwater (CompassFile *cfile, Headwater *hdw)
{
    bool okay = true, end = false;
    bool tempBool = false;
    float tempFloat = 0;
    int tempInt = 0;
    QString token (""), val ("");

    Log::outlog->add (Log::Debug, QString("parsing ......... %1").arg (*hdw->getName()));

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.contains ("EOF"))
        {
            okay = false;
        }
        else if (token.compare ("latlon", Qt::CaseInsensitive) == 0)
        {
            RiverPoint *pt = new RiverPoint ();
            okay = parse_latlon (cfile, pt);
            if (okay) hdw->getCourse().append (pt);
        }
        else if (token.compare ("width",Qt::CaseInsensitive))
        {
            okay = read_float (cfile->popToken (), &tempFloat, QString("Width"));
            hdw->setWidthAve(tempFloat);
        }
        else if (token.contains ("end"))
        {
//            cfile->checkEnd (*hdw->name);
            cfile->skipLine ();
            end = true;
        }
        else if (token.startsWith('#'))
        {
            cfile->skipLine ();
        }
        else
        {
            handle_unknown_token (token);
        }
    }
    okay = check_course (hdw);
    return okay;
}

bool parse_powerhouse (CompassFile *infile, PowerHouse *ph)
{
    bool ret = true;

    return ret;
}

bool parse_species (CompassFile *infile, Species *spc)
{
    bool ret = true;

    return ret;
}

bool parse_stock (CompassFile *infile, Stock *stk)
{
    bool ret = true;

    return ret;
}

bool parse_release (CompassFile *infile, Release *rel)
{
    bool ret = true;

    return ret;
}



bool find_file (QString filename, QString &path)
{
    bool ret = true;

    return ret;
}



QString read_string (CompassFile *infile, bool ignore_spaces)
{
    QString tok ("");
    if (ignore_spaces)
        infile->readString (tok);
    else
        tok.append (infile->popToken ());

    return tok;
}

float read_float (CompassFile *infile)
{
    QString na ("");
    float fval;
    infile->readFloatOrNa (na, fval);

    return fval;
}

bool read_float_or_na (QString token, float *number, QString name, bool *was_na)
{
    bool okay = true;
    return okay;

}

bool read_float_or_na (CompassFile *infile, float *number, QString name, bool *was_na)
{
    QString na ("");
    bool okay = infile->readFloatOrNa (na, *number);
    if (!okay)
    {
        Log::outlog->add (Log::Error, QString("Reading float or na for %1.")
                               .arg (name));
    }

    if (na.compare ("na", Qt::CaseInsensitive) == 0)
        *was_na = true;
    else
        *was_na = false;

    return okay;
}

bool read_double (QString token, double *val, QString prompt)
{
    bool okay;
    *val = token.toDouble (&okay);
    if (!okay)
        Log::outlog->add (Log::Error, QString ("Looking for double value for %1, found %2.")
                               .arg (prompt, token));
    return okay;
}

bool read_float (QString token, float *val, QString prompt)
{
    bool okay;
    *val = token.toFloat (&okay);
    if (!okay)
        Log::outlog->add (Log::Error, QString ("Looking for float value for %1, found %2.")
                               .arg (prompt, token));
    return okay;
}

bool read_int (QString token, int *val, QString prompt)
{
    bool okay;
    *val = token.toInt (&okay);
    if (!okay)
        Log::outlog->add (Log::Error, QString ("Looking for float value for %1, found %2.")
                               .arg (prompt, token));
    return okay;
}

int read_int (QFile *infile)
{
    int ival = -1;
    return ival;
}


bool check_end (QString type, QString name)
{
    bool ret = true;

    return ret;
}

void handle_unknown_token (QString token)
{
    bool isFloat;
    token.toFloat (&isFloat);

    if (isFloat)
    {
        QString msg (QString (NUMERIC_TOKEN).arg (token));
        Log::outlog->add (Log::Error, msg);
    }
    else
    {
        QString msg (QString (UNKNOWN_TOKEN).arg (token));
        Log::outlog->add (Log::Error, msg);
    }
}
void handle_obsolete_token (QString obs_token, QString new_token)
{
        QString message ("obsolete token {");
        message.append (obs_token);
        message.append ("}. ");

        if (new_token.isEmpty())
        {
                message.append ("Data discarded.\n");
        }
        else
        {
                message.append ("Replaced with {");
                message.append (new_token);
                message.append ("}.\n");
        }

        Log::outlog->add (Log::Error, message);
}
bool is_float (QString token)
{
        bool okay;
        token.toDouble (&okay);
        return okay;
}

bool is_int (QString token)
{
        bool okay;
        token.toInt (&okay);
        return okay;
}

bool parseRiverDesc (CompassFile *cfile, RiverSystem *rs)
{
    bool okay = true, end = false;
    QString token (""), val ("");

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.isEmpty () || token.compare (" ") == 0)
            continue;
        if (token.startsWith("#"))
        {
            cfile->skipLine();
        }
        else if (token.contains ("EOF", Qt::CaseInsensitive))
        {
            end = true;
        }
        else if (token.contains ("species", Qt::CaseInsensitive))
        {
            okay = cfile->readString (val);
            if (okay)
                rs->speciesNames->append (val);//compassInfo->species.append (val);
        }
        else if (token.contains ("stock", Qt::CaseInsensitive))
        {
            okay = cfile->readString (val);
            if (okay)
                rs->stockNames->append (val);//compassInfo->stocks.append (val);
        }
        else if (token.contains ("release_site"))
        {
            okay = cfile->readString (val);
            if (okay)
            {
                ReleaseSite *relsite = new ReleaseSite (val);
                rs->releaseSites->append (relsite);
                okay = parse_release_site (cfile, relsite);
            }
        }
        else if (token.contains ("river"))
        {
            QString riverName ("");
            okay = cfile->readString (riverName);
            if (okay)
            {
                River *riv = new River (riverName);
                rs->rivers->append (riv);
                okay = parse_river (cfile, rs, riv);
            }
        }
        else
        {
            handle_unknown_token (token);
        }
    }
    return okay;
}

bool parse_release_site (CompassFile *cfile, ReleaseSite *relsite)
{
    bool okay = true, end = false;
    QString token ("");
 //   ReleaseSite *relsite = new ReleaseSite();

/*    okay = cfile->readString (site);
    if (okay)
    {
        relsite->name = new QString (site);
    }
*/
    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("EOF") == 0)
        {
            cfile->printEOF ("release site data");
            okay = false;
        }
        else if (token.compare ("latlon") == 0)
        {
            okay = parse_latlon (cfile, relsite->getLatlon());
        }
        else if (token.compare ("end") == 0)\
        {
            cfile->skipLine ();
            //okay = cfile->checkEnd ("release");
            end = true;
        }
        else
        {
            handle_unknown_token (token);
            cfile->printFileLine ();
        }
    }
//    if (okay && end)
  //      rivSystem->releaseSites.append (relsite);

    return okay;
}

bool parse_latlon (CompassFile *cfile, RiverPoint *pt)
{
    bool okay = true, end = false;
    QString ltln;
    okay = cfile->readString(ltln);
    if (okay)
        pt->setLatLon(ltln);
    return okay;
/*
    QString latd, latm, lats, latdir;
    QString lond, lonm, lons, londir;

    latd = cfile->popToken ();
    latm = cfile->popToken ();
    lats = cfile->popToken ();
    latdir = cfile->popToken ();
    if (latd.compare ("EOF", Qt::CaseInsensitive) == 0 ||
            latm.compare ("EOF", Qt::CaseInsensitive) == 0 ||
            lats.compare ("EOF", Qt::CaseInsensitive) == 0 ||
            latdir.compare ("EOF", Qt::CaseInsensitive) == 0)
    {
        okay = false;
        cfile->printError ("Found EOF looking for lattitude data.");
    }
    else
    {
        pt->setLat (latd, latm, lats);
        pt->setLatDir (latdir);

        lond = cfile->popToken ();
        lonm = cfile->popToken ();
        lons = cfile->popToken ();
        londir = cfile->popToken ();
    }
    if (lond.compare ("EOF", Qt::CaseInsensitive) == 0 ||
            lonm.compare ("EOF", Qt::CaseInsensitive) == 0 ||
            lons.compare ("EOF", Qt::CaseInsensitive) == 0 ||
            londir.compare ("EOF", Qt::CaseInsensitive) == 0)
    {
        okay = false;
        cfile->printError ("Found EOF looking for longitude data.");
    }
    else
    {
        end = true;
        pt->setLon (lond, lonm, lons);
        pt->setLonDir (londir);
    }
    pt->updateText();

    return okay;
    */
}

bool check_course (RiverSegment *seg)
{
    bool okay = true;
    if (seg->getCourse().isEmpty())
    {
        okay = false;
        Log::outlog->add(Log::Error, QString(QString("Segment %1 has no latlon points.").arg(
                                                     *seg->getName())));
    }
    return okay;
}
