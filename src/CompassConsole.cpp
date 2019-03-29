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


    out = Log::instance();//new Log (this);
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
    bool okay = compassSettings->getBatch();//

    if (okay)
    {
        Scenario scenario (this);
        Results results (compassSettings);

        // read in river description file
        okay = fManager->readRiverDescFile (&scenario, compassSettings);
        // put all the segments together - create headwaters if needed, etc.
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
    if (!okay)
        out->add(Log::Error, "Error in running COMPASS console application.");

//    delete compassSettings;

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

