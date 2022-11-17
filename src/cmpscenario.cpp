#include "cmpscenario.h"

cmpScenario::cmpScenario(QObject *parent) : QObject(parent)
{

}

cmpScenario::cmpScenario(cmpSettings &settings, QObject *parent) : QObject(parent)
{
    setSettings(settings);
}

cmpScenario::~cmpScenario()
{

}

void cmpScenario::setSettings(cmpSettings &sets)
{
    settings = sets;
}

cmpSettings &cmpScenario::getSettings()
{
    return settings;
}

void cmpScenario::readDescriptionFile()
{
    cmpFile *rDesc = new cmpFile(settings.getCommandSettings()->getRivDesc());
    if (rDesc->open(QIODevice::ReadOnly))
    {
        rDesc->readHeader();
        system.parseDesc(rDesc);
        system.construct();
    }
    else
    {

    }
}


// read in data files
void cmpScenario::readDataFiles()
{

}

// run the scenario
void cmpScenario::run()
{

}

// write any output files
void cmpScenario::outputData()
{

}
