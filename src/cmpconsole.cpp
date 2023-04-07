#include "cmpconsole.h"
#include "cmpsettings.h"
#include "cmpfile.h"
#include "cmpriversystem.h"

#include <QString>


cmpConsole::cmpConsole(QObject *parent) : QObject(parent)
{
    outfile = nullptr;
    settings = new cmpSettings();
    system = new cmpRiverSystem(this);
    system->setSettings(settings);
    scenario = new cmpScenario(this);
    scenario->setSystem(system);
    scenario->setSettings(settings);

    connect (scenario, SIGNAL(done()), SLOT(complete()));
    connect (scenario, SIGNAL(canceled()), SLOT(canceled()));
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
#ifdef DEBUG_
    std::cout << "Get command line arguments" << std::endl;
#endif
    // get command line arguments
    settings->parseCommandArguments(args);

    // share settings
    scenario->setSettings(settings);
    system->setSettings(settings);
    scenario->setSystem(system);

#ifdef DEBUG_
    std::cout << "Read in river description file" << std::endl;
#endif
    // read in river description file
    scenario->readDescriptionFile();

    scenario->setPostRivMethods();

#ifdef DEBUG_
    std::cout << "Read in data files" << std::endl;
#endif
    // read in data files
//    scenario->resetData();
    scenario->readDataFiles();

#ifdef DEBUG_
    std::cout << "Set flow in segments" << std::endl;
#endif
    // propagate input dam data into the headwaters
    // this flow init must occur before the dam inits
    system->markRegulationPts();
    system->fillHeadwaters();
//    system->initialize();

#ifdef DEBUG_
    std::cout << "Run the scenario" << std::endl;
#endif
    // run the scenario
    scenario->run();

#ifdef DEBUG_
    std::cout << "Write any output files" << std::endl;
#endif
    // write any output files
    scenario->outputData(settings->getCommandSettings()->getOutputFile(),
                         settings->getCommandSettings()->getOutputAllData());

    complete();
    return 0;
}

void cmpConsole::complete()
{
    emit done(0);
}

void cmpConsole::canceled()
{
    emit done(1);
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
