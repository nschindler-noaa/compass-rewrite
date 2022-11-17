#include "cmplog.h"

#include <iostream>

#define DEFAULT_LOGNAME  "COMPASSXXXX.outlog"

cmpLog *cmpLog::outlog = nullptr;

cmpLog *cmpLog::instance()
{
    if (outlog == nullptr)
        outlog = new cmpLog(DEFAULT_LOGNAME);
    return outlog;
}


cmpLog::cmpLog (QString filename)
{
    logFile = nullptr;
    setLogFile (filename);
    setup ();
}

void cmpLog::setup ()
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

void cmpLog::setLogFile (QString filename)
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

cmpLog::~cmpLog()
{
    deleteLogFile ();
//    outlog = nullptr;
}


void cmpLog::deleteLogFile ()
{
    if (logFile != nullptr)
    {
        logFile->close ();
        delete logFile;
    }
}

void cmpLog::add (MessageType id, QString msg)
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

void cmpLog::addToDisplay (QString text)
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

void cmpLog::display (QString text)
{
    std::cout << text.toUtf8 ().data () << std::endl;
}

void cmpLog::setMessage (bool flag)
{
    message = flag;
}

void cmpLog::setWarning (bool flag)
{
    warning = flag;
}

void cmpLog::setRawDump (bool flag)
{
    rawdump = flag;
}

void cmpLog::setDebug (bool flag)
{
    debug = flag;
}

void cmpLog::clear ()
{
    logFile->close ();
    logFile->open ();
}
