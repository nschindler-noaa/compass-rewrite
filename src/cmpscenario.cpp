#include "cmpscenario.h"

cmpScenario::cmpScenario(QObject *parent) : QObject(parent)
{
    settings = nullptr;
    system = nullptr;
}

cmpScenario::cmpScenario(cmpSettings *settings, QObject *parent) : QObject(parent)
{
    setSettings(settings);
    system = nullptr;
}

cmpScenario::~cmpScenario()
{

}

void cmpScenario::setSettings(cmpSettings *sets)
{
    settings = sets;
}

cmpSettings *cmpScenario::getSettings()
{
    return settings;
}

void cmpScenario::setSystem(cmpRiverSystem *sys)
{
    system = sys;
}

cmpRiverSystem *cmpScenario::getSystem()
{
    return system;
}

void cmpScenario::readDescriptionFile()
{
    cmpFile *rDesc = new cmpFile(settings->getCommandSettings()->getRivDesc());
    if (rDesc->open(QIODevice::ReadOnly))
    {
        rDesc->readHeader();
        system->parseDesc(rDesc);
        system->construct();
    }
    else
    {
        emit canceled();
    }
}


// read in data files
void cmpScenario::readDataFiles()
{
    QString filename = settings->getCommandSettings()->getInitialData();
    cmpFile datafile;
    if (!filename.isEmpty())
    {
        datafile.setFileName(filename);
        system->parseData(&datafile);
    }
}

// release stuff
int cmpScenario::getNumberReleases()
{
    return releases.count();
}

cmpRelease *cmpScenario::getRelease(int index)
{
    return releases.at(index);
}

void cmpScenario::deleteReleases()
{
    while (releases.count() > 1)
    {
        cmpRelease *rls = releases.takeLast();
        delete rls;
    }
}

// run the scenario
void cmpScenario::run()
{

}

void cmpScenario::runScenario()
{

}

void cmpScenario::runMonteCarlo()
{

}

void cmpScenario::runMonteCarloMV()
{

}

void cmpScenario::runRealTime()
{

}

// write any output files
void cmpScenario::outputData()
{

}
