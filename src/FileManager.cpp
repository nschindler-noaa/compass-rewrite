#include <QFileInfo>
#include <QTime>
#include <QDate>

#include "FileManager.h"
#include "parseUtil.h"
#include "Log.h"
#include "settings.h"
#include "writeUtil.h"


FileManager::FileManager(QObject *parent) :
    QObject(parent)
{
}

bool FileManager::parseHeader (QString filename, file_info *fi)
{
    bool ret = true;

    return ret;
}

bool FileManager::parseFileinfo (QString filename, file_info *fi)
{
    bool ret = true;

    return ret;
}

bool FileManager::readRiverDescFile (Scenario *scn, Settings *set)//QString filename)
{
    bool okay = true;

    QString path ("");
    QString filename (set->getRiverDesc());

    if (findFile (filename, path, set))//filename, path, compassSettings))//fileExists (filename, path))
    {
        okay = readRiverDescFile (scn->river, filename);
    }
    else
    {
        QString msg (QString ("River description file {%1} not found.").arg (filename));
//        Log::outlog->add (Log::Error, msg);
        okay = false;
    }
    return okay;
}

bool FileManager::readRiverDescFile (RiverSystem *rs, QString filename)
{
    bool okay = true;

    emit readingFile ();

    CompassFile *infile = new CompassFile (filename);
    okay = infile->open (QIODevice::ReadOnly);

    // read the header information if any
    if (okay) okay = infile->readHeader ();

    // read the file information (at least data version)
    if (okay) infile->readInfo (); // to read older files, okay not set

    // read the data
    okay = parseRiverDesc (infile, rs);

    infile->close ();

    if (okay)
        files.append(infile);

//    delete infile;

    emit fileRead ();

    return okay;
}

bool FileManager::readFile (RiverSystem *rs, QString filename)
{
    bool okay = true;
    TYPE type = getType(filename);

    switch (type)
    {
    case DESC:
        okay = readRiverDescFile (rs, filename);
        filenames.append(filename);
        break;

    case NONE:
        Log::outlog->add(Log::Error, QString("Could not determine type of file to read: %1").arg(filename));
        break;

    default:
        CompassFile *cfile = new CompassFile(filename);
        if (cfile->open(QIODevice::ReadOnly))
        {
            filenames.append(filename);
            files.append(cfile);
            emit readingFile ();
            cfile->readHeader();
            cfile->readInfo();
            okay = rs->parse (cfile);
            emit fileRead ();
        }
        else
        {
            Log::outlog->add(Log::Error, QString("Could not open file to read: %1").arg(filename));
            cfile->printError ("could not open file ...");
            okay = false;
        }
        break;
    }

    return okay;
}
/*
bool FileManager::readFile (QString filename)
{
    bool okay = true;

    if (isRivDescFile(filename))
    {
        okay = readRiverDescFile (filename);
    }
    else if (isCompassFile (filename))
    {
        emit readingFile ();
        okay = parseCompassFile (filename);
        emit fileRead ();
    }
    else
    {
        Log::outlog->add(Log::Error, "could not determine type of file to read.");
        Log::outlog->add(Log::Error, filename);
    }

    return okay;
}

bool FileManager::readFiles ()
{
    return true;
}*/

bool FileManager::readFiles (Scenario *scn, Settings *set)
{
    bool okay = true;
//    QString filename (set->riv_data);

    if (!set->getInitialData().isEmpty())
    {
        okay = findFile (set->getInitialData(), set->getApplicationDir(), set);
        okay = readFile (scn->river, set->getInitialData());
    }
    if (!set->getRiverData().isEmpty())
    {
        okay = readFile (scn->river, set->getRiverData());
    }
    if (!set->getDamOpsData().isEmpty())
    {
        okay = readFile (scn->river, set->getDamOpsData());
    }
    if (!set->getCalibData().isEmpty())
    {
        okay = readFile (scn->river, set->getCalibData());
    }
    if (!set->getPostbnData().isEmpty())
    {
        okay = readFile (scn->river, set->getPostbnData());
    }
    if (!set->getEtcData().isEmpty())
    {
        okay = readFile(scn->river, set->getEtcData());
    }
    if (!set->getReleaseData().isEmpty())
    {
        okay = readFile (scn->river, set->getReleaseData());
    }

    return okay;
}

bool FileManager::readMVFiles (RiverSystem *rs, Settings *settings)
{
    bool okay = true;
    return okay;
}

bool FileManager::readMultiFiles (RiverSystem *rs, Settings *settings)
{
    bool okay = true;
    return okay;
}

bool FileManager::readRTFiles (RiverSystem *rs, Settings *settings)
{
    bool okay = true;
    return okay;
}

bool FileManager::writeSummary(Results *result, QString filename)
{
    return true;
}

bool FileManager::writeRiverDescFile (RiverSystem *rs, Settings *settings, QString filename)
{
    bool okay = true;
    CompassFile *descfile = files.at(0);
    CompassFile *outFile;

    if (filename.isEmpty()) {
        outFile = descfile;
    }
    else {
        outFile = new CompassFile(filename);
    }
    okay = outFile->open(QIODevice::WriteOnly);
    if (okay)
    {
        writeFileComments(outFile, descfile->header);
        outFile->writeNewline();
        for (int i = 0; i < rs->speciesNames->count(); i++)
            outFile->writeString(0, "species", rs->speciesNames->at(i));
        outFile->writeNewline();
        for (int i = 0; i < rs->stockNames->count(); i++)
            outFile->writeString(0, "stock", rs->stockNames->at(i));
        outFile->writeNewline();
        for (int i = 0; i < rs->releaseSites->count(); i++)
        {
            outFile->writeString(0, "release_site", *rs->releaseSites->at(i)->getName());
            outFile->writeString(1, "latlon", rs->releaseSites->at(i)->getLatlon()->getLatLon());
            outputEnd(outFile, 0, "release_site");
//            newFile->writeString(0, "end", "(release_site)");
            outFile->writeNewline();
        }
        for (int i = 0; i < rs->rivers->count(); i++)
        {
            outputRiver(rs->rivers->at(i), outFile);
        }
    }
    outFile->close();

    return okay;
}

bool FileManager::outputRiver(River *river, CompassFile *outfile)
{
    bool okay = true;

    outfile->writeString(0, "river", river->getName());
    outfile->writeString(0, "flow_max", QString::number(river->getFlowMax()));
    outfile->writeString(0, "flow_min", QString::number(river->getFlowMin()));
    outfile->writeNewline();

    for (int i = 0; i < river->getNumSegments(); i++)
    {
        outputSegment (river->getSegment(i), outfile);
        outfile->writeNewline();
    }
    outputEnd(outfile, 0, "river", river->getName());
    outfile->writeNewline();

    return okay;
}

bool FileManager::outputSegment(RiverSegment *rseg, CompassFile *outfile)
{
    bool okay = true;
    QString tempString;
    float tempFloat;
    float defaultFloat = 0.0;
    int defaultInt = 0;
    switch (rseg->getType()) {
    case RiverSegment::Dam:
    {
        Dam *dam = static_cast<Dam*>(rseg);
        outfile->writeString(0, "dam", *dam->getName());
        if (dam->getNumPowerhouses() > 0)
        {
            outfile->writeString(0, "abbrev", *dam->getAbbrev());
            outfile->writeValue(0, "powerhouse_capacity", dam->getPowerhouses().at(0)->getCapacity(), &defaultFloat);
            for (int i = 1; i < dam->getNumPowerhouses(); i++)
            {
                tempString = QString("powerhouse_%1_capacity").arg(QString::number(i+1));
                outfile->writeValue(0, tempString, dam->getPowerhouses().at(i)->getCapacity(), &defaultFloat);
            }
            if (dam->getBasin() != nullptr)
                outfile->writeString(0, "storage_basin",
                                     QString::number(dam->getBasin()->getVolumeMin()),
                                     QString::number(dam->getBasin()->getVolumeMax()));
            outfile->writeValue(0, "floor_elevation", dam->getElevBase(), &defaultFloat);
            outfile->writeValue(0, "forebay_elevation", dam->getElevForebay(), &defaultFloat);
            outfile->writeValue(0, "tailrace_elevation", dam->getElevTailrace(), &defaultFloat);
            if (dam->getHeightBypass() != 0.0)
                outfile->writeValue(0, "bypass_elevation", dam->getHeightBypass() + dam->getElevBase(), &defaultFloat);
            outfile->writeValue(0, "spillway_width", dam->getSpillway()->getWidth(), &defaultFloat);
            outfile->writeString(0, "spill_side", dam->getSpillSideText());
            outfile->writeValue(0, "pergate", dam->getSpillway()->getPerGate(), &defaultFloat);
            outfile->writeValue(0, "ngates", dam->getSpillway()->getNumGates(), &defaultInt);
            outfile->writeValue(0, "gate_width", dam->getSpillway()->getGateWidth(), &defaultFloat);
            outfile->writeValue(0, "basin_length", dam->getLengthBasin(), &defaultFloat);
            outfile->writeValue(0, "sgr", dam->getSpecGrav(), &defaultFloat);
            if (dam->getFishway() != nullptr) {
                outfile->writeString(1, "fishway");
                outfile->writeString(2, "type", dam->getFishway()->getTypeString());
                outfile->writeValue(2, "length", dam->getFishway()->getLength());
                outfile->writeValue(2, "capacity", dam->getFishway()->getCapacity());
                outfile->writeValue(2, "velocity", dam->getFishway()->getVelocity());
                outfile->writeString(1, "end", "fishway");
            }
            outfile->writeString(1, "latlon", dam->getCourse().at(0)->getLatLon());
        }
        outputEnd(outfile, 0, "dam", *dam->getName());
        break;
    }
    case RiverSegment::Reach:
    {
        Reach *rch = static_cast<Reach*>(rseg);
        outfile->writeString(0, "reach", *rch->getName());
        outfile->writeValue(0, "width", rch->getWidthAve(), &defaultFloat);
        outfile->writeValue(0, "lower_depth", rch->getDepthLower(), &defaultFloat);
        outfile->writeValue(0, "upper_depth", rch->getDepthUpper(), &defaultFloat);
        outfile->writeValue(0, "slope", rch->getSlope(), &defaultFloat);
        outfile->writeValue(0, "lower_elev", rch->getElevLower(), &defaultFloat);
        for (int i = 0; i < rch->getCourse().count(); i++)
        {
            outfile->writeString(1, "latlon", rch->getCourse().at(i)->getLatLon());
        }
        outputEnd(outfile, 0, "reach", *rseg->getName());
        break;
    }
    case RiverSegment::Headwater:
    {
        outfile->writeString(0, "headwater", *rseg->getName());
        outputEnd(outfile, 0, "headwater", *rseg->getName());
        break;
    }
    default:
    {
        outfile->writeString(0, "null_segment", *rseg->getName());
        outputEnd(outfile, 0, "null_segment", *rseg->getName());
    }
    }


    return okay;
}

bool FileManager::writeFile (Scenario *scn, QString filename)
{
    return true;
}
bool FileManager::writeFiles (Scenario *scn, Settings *settings)
{
    return true;
}

bool FileManager::writeFileHeader (QFile *outfile, Settings *settings)
{
    bool okay = true;
    QString time, date;

    TYPE type = getType(outfile->fileName());
    if (type == NONE)
    {
        Log::outlog->add(Log::Error, QString("Incorrect file extension: %1").arg(outfile->fileName()));
        okay = false;
    }
    if (outfile->isOpen() || outfile->open (QIODevice::WriteOnly))
    {
        QString line;
        writeSeparator (outfile);

        switch (type) {
        case OPS:
            line.append ("# COMPASS Operations File                                                    #\n");
            break;

        case CTRL:
            line.append ("# COMPASS Control File                                                       #\n");
            break;

        case RIV:
            line.append ("# COMPASS River Environment File                                             #\n");
            break;

        case TRANS:
            line.append ("# COMPASS Transport File                                                     #\n");
            break;

        case DAM:
            line.append ("# COMPASS Dam Parameters File                                                #\n");
            break;

        case RLS:
            line.append ("# COMPASS Release File                                                       #\n");
            break;

        case CLB:
            line.append ("# COMPASS Calibration File                                                   #\n");
            break;

        default:
            line.append ("# COMPASS Data File                                                          #\n");
        }

        outfile->write (line.toUtf8());
        outfile->write ("#                                                                            #\n");


        /* get the time and date */
        time = QTime::currentTime().toString();
        date = QDate::currentDate().toString (QString ("ddd MMM dd, yyyy,"));


        if (settings->getCurrentUser().isEmpty())
        {
            line = QString ("# Written");
        }
        else
        {
            line = QString ("# Written by user %1").arg(settings->getCurrentUser());
        }
        line.append (QString (" on %1 at %2").arg (date, time));
        addSpaces(line, 78);
        line.append(QString("#\n"));

        outfile->write (line.toUtf8());

        writeSeparator(outfile);

    }
    else {
        okay = false;
        qInfo("%s", QString("Could not open file to read: %1").arg(outfile->fileName()).toUtf8().data());
//        Log::outlog->add(Log::Error, QString("Could not open file to read: %1").arg(outfile->fileName()));
    }

    return okay;
}

void FileManager::writeSeparator(QFile *outfile)
{
    outfile->write ("#============================================================================#\n");
}

bool FileManager::outputText (QString text)
{
    return true;
}

bool FileManager::saveData (Scenario *scn, QString filename)
{
    return true;
}

bool FileManager::fileExists (QString &filename, QString &path)
{
    bool exist = true;
    QFileInfo finfo (filename);
    QString sep ('/');
    QString nameonly ("");

    if (filename.contains ('\\'))
        filename.replace ('\\', '/');

    finfo.setFile (filename);
    nameonly = QString (filename.section ('/', -1));

    exist = finfo.exists();
    if (exist)
    {
        path = finfo.absolutePath();
    }
    else
    {
        if (!path.isEmpty ())
        {
            filename = QString (path + sep + nameonly);
            finfo.setFile (filename);
            exist = finfo.exists ();
        }
        else
        {
            finfo.setFile(nameonly);
            exist = finfo.exists();
            if (exist)
                path = finfo.absolutePath();
        }
    }
    return exist;
}

bool FileManager::findFile (QString filename, QString path, Settings *&set)
{
    bool exist = false;
//    QFileInfo finfo (filename);
//    QString nameonly (filename.section ('/', -1));

    if (filename.contains ('\\'))
        filename.replace ('\\', '/');

//    exist = finfo.exists ();
    if (!exist)
    {
        exist = fileExists (filename, path);
    }
    if (!exist)
    {
        path = set->getApplicationDir();
        exist = fileExists (filename, path);
    }
    if (!exist)
    {
        path = set->getUserHomeDir();
        exist = fileExists (filename, path);
    }
    if (!exist)
    {
        path = set->getUserCurrentDir();
        exist = fileExists (filename, path);
    }

    if (!exist)
    {
        QString msg (QString ("File {%1} not found.").arg (filename));
        qWarning("%s", msg.toUtf8().data());
//        Log::outlog->add (Log::Error, msg);
    }

    return exist;
}

bool FileManager::isRivDescFile(QString filename)
{
    bool rivdesc = false;
    if (filename.endsWith(".desc", Qt::CaseInsensitive))
        rivdesc = true;

    return rivdesc;
}

bool FileManager::isCompassFile(QString filename)
{
    bool compass = false;

    if (filename.endsWith(".riv", Qt::CaseInsensitive) ||
            filename.endsWith(".ops", Qt::CaseInsensitive) ||
            filename.endsWith(".clb", Qt::CaseInsensitive) ||
            filename.endsWith(".rls", Qt::CaseInsensitive) ||
            filename.endsWith(".pbn", Qt::CaseInsensitive) ||
            filename.endsWith(".scn", Qt::CaseInsensitive) ||
            filename.endsWith(".dat", Qt::CaseInsensitive))
        compass = true;

    return compass;
}

TYPE FileManager::getType(QString filename)
{
    TYPE type;
    if (filename.endsWith(".desc"))
        type = DESC;
    else if (filename.endsWith(".riv", Qt::CaseInsensitive))
        type = RIV;
    else if (filename.endsWith(".ops", Qt::CaseInsensitive))
        type = OPS;
    else if (filename.endsWith(".clb", Qt::CaseInsensitive))
        type = CLB;
    else if (filename.endsWith(".rls", Qt::CaseInsensitive))
        type = RLS;
    else if (filename.endsWith(".pbn", Qt::CaseInsensitive))
        type = PBN;
    else if (filename.endsWith(".scn", Qt::CaseInsensitive))
        type = SCN;
    else if (filename.endsWith(".dat", Qt::CaseInsensitive))
        type = DAT;
    else if (filename.endsWith(".etc", Qt::CaseInsensitive))
        type = ETC;
    else if (filename.endsWith(".ctrl", Qt::CaseInsensitive))
        type = CTRL;
    else if (filename.endsWith(".spll", Qt::CaseInsensitive))
        type = SPLL;
    else if (filename.endsWith(".spill", Qt::CaseInsensitive))
        type = SPILL;
    else if (filename.endsWith(".flow", Qt::CaseInsensitive))
        type = FLOW;
    else if (filename.endsWith(".trans", Qt::CaseInsensitive))
        type = TRANS;
    else
        type = NONE;

    return type;
}

void FileManager::addSpaces(QString &line, int col)
{
    while (line.count() < col)
        line.append(' ');
}
