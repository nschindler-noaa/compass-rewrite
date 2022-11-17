#include "cmpconsole.h"
#include "cmpsettings.h"
#include "cmpfile.h"
#include "cmpriversystem.h"

cmpConsole::cmpConsole(QObject *parent) : QObject(parent)
{
    outfile = nullptr;
    scenario = new cmpScenario(this);

    connect (scenario, SIGNAL(done()), SIGNAL(done(0)));
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
    settings.parseCommandArguments(args);

    // create list of input files
    scenario->setSettings(settings);

    // read in river description file
    scenario->readDescriptionFile();

    // read in data files
    scenario->readDataFiles();

    // run the scenario
    scenario->run();

    // write any output files
    scenario->outputData();

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
