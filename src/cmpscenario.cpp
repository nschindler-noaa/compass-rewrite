#include "cmpscenario.h"

cmpScenario::cmpScenario(QObject *parent) : QObject(parent)
{
    settings = nullptr;
    system = nullptr;
    postRivMethods = new QStringList();
    postRivMethodNames = new QStringList();
}

cmpScenario::cmpScenario(cmpSettings *settings, QObject *parent) : QObject(parent)
{
    setSettings(settings);
    system = nullptr;
    postRivMethods = new QStringList();
    postRivMethodNames = new QStringList();
}

cmpScenario::~cmpScenario()
{
    delete postRivMethods;
    delete postRivMethodNames;
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
    QStringList files = settings->getCommandSettings()->getFileNameList();
    cmpFile datafile;
    system->resetData();
    for (int i = 0, total = files.count(); i < total; i++)
    {
        datafile.setFileName(files.at(i));
        if (datafile.open(QIODevice::ReadOnly))
        {
            datafile.readHeader();
            datafile.readInfo();
            system->parseData(&datafile);
        }
    }
}

void refresh_data(){

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

QStringList *cmpScenario::getPostRivMethods() const
{
    return postRivMethods;
}

void cmpScenario::setPostRivMethods()
{
    postRivMethods->append ("sar_vs_date");
    postRivMethods->append ("latent_mortality");
    postRivMethods->append ("constant_d");
    postRivMethods->append ("s3_vs_wtt");
    postRivMethodNames->append ("SAR vs. Date");
    postRivMethodNames->append ("Latent Mortality");
    postRivMethodNames->append ("Constant D");
    postRivMethodNames->append ("S3 vs Water Travel Time");
}
