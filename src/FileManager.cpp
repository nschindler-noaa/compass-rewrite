#include "FileManager.h"
#include <QFileInfo>
#include "parseUtil.h"
#include "Log.h"
#include "settings.h"


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
        Log::outlog->add (Log::Error, msg);
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

//    delete infile;

    emit fileRead ();

    return okay;
}

bool FileManager::readFile (RiverSystem *rs, QString filename)
{
    bool okay = true;

    if (isRivDescFile(filename))
    {
        okay = readRiverDescFile (rs, filename);
        filenames.append(filename);
    }
    else if (isCompassFile (filename))
    {
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
            cfile->printError ("could not open file ...");
    }
    else
    {
        Log::outlog->add(Log::Error, "could not determine type of file to read.");
        Log::outlog->add(Log::Error, filename);
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

bool FileManager::writeFile (Scenario *scn, QString filename)
{
    return true;
}
bool FileManager::writeFiles (Scenario *scn, Settings *settings)
{
    return true;
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
        Log::outlog->add (Log::Error, msg);
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
