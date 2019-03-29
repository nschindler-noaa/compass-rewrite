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
            okay = read_float (token, &river->flowMax, "Flow max");
        }
        else if (token.compare ("flow_min") == 0)
        {
            okay = read_float (cfile->popToken (), &river->flowMin, QString("Flow min"));
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
                reach->setRiverName(river->name);
                okay = parse_reach (cfile, reach);
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
                dam->setRiverName(river->name);
                okay = parse_dam (cfile, dam);
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
                head->setRiverName(river->name);
                okay = parse_headwater (cfile, head);
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
    return okay;
}

bool parse_dam (CompassFile *cfile, Dam *dam)
{
    bool okay = true, end = false;
    bool tempBool = false;
    float tempFloat = 0;
    int tempInt = 0;
    QString token (""), val ("");
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
            if (okay) dam->getCourse().append (pt);
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
            okay = read_float (val, &dam->width, QString("Width"));
        }
        else if (token.compare ("floor_elevation", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &dam->elevBase, QString("Floor elevation"));
        }
        else if (token.compare ("forebay_elevation", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &dam->elevForebay, QString("Forebay elevation"));
        }
        else if (token.compare ("bypass_elevation", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &temp, QString("Forebay elevation"));
            dam->heightBypass = temp - dam->elevBase;
            if (dam->heightBypass < 0.0)
                dam->heightBypass = 0.0;
        }
        else if (token.compare ("tailrace_elevation", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &dam->elevTailrace,
                           QString("Tailrace elevation"));
        }
        else if (token.compare ("spillway_width", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &dam->widthSpillway,
                           QString("Spillway width"));
        }
        else if (token.compare ("spill_side", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken ();
            if (val.compare ("left", Qt::CaseInsensitive) == 0)
                dam->spillSide = Dam::Left;
            else
                dam->spillSide = Dam::Right;
        }
        else if (token.compare ("ngates", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_int (val, &dam->numGates,
                    QString("Number of spillway gates"));
        }
        else if (token.compare ("gate_width", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &dam->widthGates,
                    QString("Spillway gate width"));
        }
        else if (token.compare ("pergate", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &dam->spillPerGate,
                    QString("Spillway per gate"));
        }
        else if (token.contains ("powerhouse", Qt::CaseInsensitive))
        {
            val = cfile->popToken ();
            read_float (val, &temp, QString ("powerhouse"));
            PowerHouse *phouse = nullptr;
            if (token.contains ("2"))
                index = 1;
            else
                index = 0;

            Log::outlog->add(Log::Debug,QString ("powerhouse index %1").arg(
                                     QString::number(index)));
            while (dam->powerhouses.count () < (index + 1))
            {
                dam->powerhouses.append (new PowerHouse (index));
            }

            phouse = (PowerHouse *) dam->powerhouses[index];
            phouse->setCapacity(temp);
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
            val = cfile->popToken();
            dam->basin = new Basin ();
            okay = read_float (val, &dam->basin->min_volume,
                       QString("Dam basin minimum volume"));
            val = cfile->popToken();
            okay = read_float (val, &dam->basin->max_volume,
                       QString("Dam basin maximum volume"));
        }
        else if (token.compare ("basin_length", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &dam->lengthBasin,
                       QString("Stilling basin length"));
        }
        else if (token.compare ("sgr", Qt::CaseInsensitive) == 0)
        {
            val = cfile->popToken();
            okay = read_float (val, &dam->specGrav,
                       QString("Specific gravity"));
        }
        else if (token.contains ("end"))
        {
//            cfile->checkEnd (*dam->name);
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
    okay = check_course (dam);
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
            if (okay) rch->getCourse().append (pt);
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
            okay = read_float (cfile->popToken (), &rch->depth,
                               QString("Depth"));
        }
        else if (token.compare ("lower_depth", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &rch->lower_depth,
                               QString("Lower depth"));
        }
        else if (token.compare ("upper_depth", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &rch->upper_depth,
                               QString("Upper depth"));
        }
        else if (token.compare ("lower_elev", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &rch->lower_elev,
                               QString("Lower elevation"));
        }
        else if (token.compare ("slope", Qt::CaseInsensitive) == 0)
        {
            okay = read_float (cfile->popToken (), &rch->slope
                               ,
                               QString("Slope"));
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
        else if (token.contains ("EOF", Qt::CaseInsensitive))
        {
            okay = false;
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
            okay = parse_latlon (cfile, relsite->latlon);
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
    }
    return okay;
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
