#include <iostream>
using namespace std;

#include <QCoreApplication>
#include <QStringList>

#include "CompassConsole.h"
#include "Log.h"
#include "RiverSystem.h"
#include "Scenario.h"
#include "Results.h"

CompassConsole::CompassConsole(QObject *parent) :
    QObject(parent)
{
    compassSettings = new Settings();
    compassSettings->restore();
    compassSettings->readArguments(qApp->arguments ());
    compassSettings->getCurrentUserData();


    out = Log::instance();
    fManager = new FileManager (this);
//    sManager = new ScenarioManager (this);

}

CompassConsole::~CompassConsole()
{
//    delete sManager;
    delete fManager;
    delete out;
}

void CompassConsole::run ()
{
    bool okay = compassSettings->getRunConsole();//

    if (okay)
    {
        Scenario scenario (this);
        Results results (compassSettings);

        // read in river description file
        okay = fManager->readRiverDescFile (&scenario, compassSettings);

#ifdef DEBUG
        if (okay) {
            QString fn(QString("%1River.check").arg(scenario.river->rivers->at(0)->getName()));
            okay = fManager->writeRiverDescFile (scenario.river, compassSettings, fn); //QString("columbiaRiver.check"));
        }
#endif
        // put all the segments together - create headwaters if needed, etc.
        if (okay)
            okay = scenario.constructRiver (compassSettings);
        // read in scenario information file(s)
        if (okay)
        {
            okay = fManager->readFiles (&scenario, compassSettings);
        }

        if (okay)
        {
            // check, fill in missing, and initialize data elements (flow, spill, etc.)
            scenario.initialize ();
            // run the scenario
            okay = scenario.run (&results, compassSettings);
        }
        // output specified data file(s), if any
        if (okay)
            okay = fManager->writeFiles (&scenario, compassSettings);
        // output results data, if requested
        if (okay && compassSettings->getSummary())
            okay = fManager->writeSummary (&results, compassSettings->getOutputData());

    }
    else
    {
        qFatal("Trying to run console without being in console mode.");
    }
    if (!okay)
    {
        out->add(Log::Error, "Error in running COMPASS console application.");
        qWarning("Error in running COMPASS console application.");
    }

    emit done(okay);
}


bool consoleMode (int argc, char *argv[])
{
    bool cmode = false;
    char *ch;
    for (int i = 1; cmode == false && i < argc; i++)
    {
        ch = argv[i];
        if (*ch == '-')
        {
            while (*(++ch) != '\0')
            {
                if (*ch == 'b')
                {
                    cmode = true;
                    break;
                }
            }
        }
    }
    return cmode;
}

// Installs a message handler for QtDebugMsg, QtInfoMsg, QtWarningMsg
// QtCriticalMsg, QtFatalMsg, and QtSystemMsg
// used by qDebug(), qInfo(), qWarning(), qCritical(), qFatal() - similar to fprint()
// TODO:
// We'll see if this works.
void  consoleOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stdout, "Info:  %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stdout, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    }
}


