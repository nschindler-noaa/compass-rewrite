#include "cmpconsole.h"
#include "cmpsettings.h"
#include "cmpfile.h"
#include "cmpriversystem.h"

#include <QString>

cmpConsole::cmpConsole(QObject *parent) : QObject(parent)
{
    outfile = nullptr;
    settings = new cmpSettings();
    scenario = new cmpScenario(this);
    system = new cmpRiverSystem(this);

    connect (scenario, SIGNAL(done()), SIGNAL(done()));
    connect (scenario, SIGNAL(canceled()), SIGNAL(done(1)));
}

cmpConsole::~cmpConsole()
{
    while (!files.isEmpty())
    {
        cmpFile *tmp = files.takeLast();
        delete tmp;
    }
    if (outfile != nullptr)
    {
        delete outfile;
    }
}

int cmpConsole::run(QStringList args)
{
    // get command line arguments
    settings->parseCommandArguments(args);

    // share settings
    scenario->setSettings(settings);
    system->setSettings(settings);
    scenario->setSystem(system);

    // read in river description file
    scenario->readDescriptionFile();

    scenario->setPostRivMethods();

    // read in data files
//    scenario->resetData();
    scenario->readDataFiles();

    // propagate input dam data into the headwaters
    // this flow init must occur before the dam inits
    system->markRegulationPts();
    system->fillHeadwaters();
//    system->initialize();

    // run the scenario
    scenario->run();

    // write any output files
    scenario->outputData(settings->getCommandSettings()->getOutputFile(), settings->getCommandSettings()->getOutputAllData());

    emit done(0);
    return 0;
}

bool consoleMode(int argc, char *argv[])
{
    bool con = false;
    for (int i = 1; i < argc; i++)
    {
        QString arg(argv[i]);
        if (arg.startsWith("-b"))
        {
            con = true;
            break;
        }
    }
    return con;
}
