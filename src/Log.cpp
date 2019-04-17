#include <iostream>

#include "Log.h"
#define DEFAULT_LOGNAME  "COMPASSXXXX.outlog"

Log *Log::outlog = nullptr;

Log *Log::instance()
{
    if (outlog == nullptr)
        outlog = new Log(DEFAULT_LOGNAME);
    return outlog;
}


Log::Log (QString filename, QObject *parent)
{
    logFile = nullptr;
    setLogFile (filename);
    setup ();
}

Log::Log (QObject *parent)
{
    logFile = nullptr;
    setLogFile ();
    setup ();
}

void Log::setup ()
{
    prefix.append ("--ERROR: ");
    prefix.append ("WARNING: ");
    prefix.append ("  ");
    prefix.append ("RAWDUMP: ");
    prefix.append ("--DEBUG: ");
    prefix.append ("!FATAL!  ");
    prefix.append ("");

    lastLine = QString ("");
    repCount = 0;

    logFileName = logFile->fileName ();
    logFile->open ();
}

void Log::setLogFile (QString filename)
{
    if (filename.isEmpty())
    {
        filename.append (DEFAULT_LOGNAME);
    }
    deleteLogFile ();
    logFile = new QTemporaryFile (filename);
    logFileName = logFile->fileName ();
    logFile->open ();
}

Log::~Log()
{
    deleteLogFile ();
//    outlog = nullptr;
}


void Log::deleteLogFile ()
{
    if (logFile != nullptr)
    {
        logFile->close ();
        delete logFile;
    }
}

void Log::add (MessageType id, QString msg)
{
    bool addtolog = true;
    switch (id)
    {
    case Warning:
        addtolog = warning;
        break;
    case Message:
        addtolog = message;
        break;
    case RawDump:
        addtolog = rawdump;
        break;
    case Debug:
        addtolog = debug;
        break;
    default:
        addtolog = true;
    }

    if (addtolog)
    {
        // add to the display
        addToDisplay ((prefix[id] + msg).toUtf8());
    }
    // always add to the temp file
    logFile->write ((prefix[id] + msg).toUtf8());
}

void Log::addToDisplay (QString text)
{
    if (lastLine.compare (text) == 0)
    {
        repCount++;
    }
    else
    {
        if (repCount > 0)
        {
            if (repCount > 1)
            {
                display (QString("       last line repeated %1 times.")
                         .arg (QString::number (repCount)));
            }
            else
            {
                display ("       last line repeated 1 time.");
            }
            repCount = 0;
        }
        display (text);
        lastLine = text;
    }
}

void Log::display (QString text)
{
    std::cout << text.toUtf8 ().data () << std::endl;
}

void Log::setMessage (bool flag)
{
    message = flag;
}

void Log::setWarning (bool flag)
{
    warning = flag;
}

void Log::setRawDump (bool flag)
{
    rawdump = flag;
}

void Log::setDebug (bool flag)
{
    debug = flag;
}

void Log::clear ()
{
    logFile->close ();
    logFile->open ();
}
