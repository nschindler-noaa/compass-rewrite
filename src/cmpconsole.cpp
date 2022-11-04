#include "cmpconsole.h"
#include "cmpsettings.h"
#include "cmpfile.h"
#include "cmpriversystem.h"

cmpConsole::cmpConsole(QObject *parent) : QObject(parent)
{
    outfile = nullptr;
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

    // read in river description file
    cmpFile *rDesc = new cmpFile(settings.getCommandSettings()->getRivDesc());
    if (rDesc->open(QIODevice::ReadOnly))
    {
        rDesc->readHeader();
        system.parseDesc(rDesc);
    }
    else
        emit done(1);

    // read in data files

    // run the scenario

    // write any output files

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
