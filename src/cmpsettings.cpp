
#include "cmpsettings.h"

#include <qlogging.h>

cmpSettings::cmpSettings()
{
    numReachClasses = 2;
    message = false;
    warn = false;
    debug = false;
    rawData = false;
    batch = true;
    single = true;
    monte = false;
    covar = false;
    iterations = -1;
    realtime = false;
    stepwise = false;
    summary = true;
    gui = false;
    ttCalib = false;
    tspd4_print4 = 0;
    survCalib = false;
    outputAllData = false;

    rivDesc = QString("columbia.desc");
    calibFile = QString();
    relFile = QString();
    rivYearFile = QString();
    damOpsFile = QString();
    postRivFile = QString();
    configFile = QString("base.etc");
    outControlFile = QString();
    scenarioFile = QString();
    outputFile = QString();
    initialData = QString("base.dat");
}

void cmpSettings::parseArguments(QStringList &argList)
{
    QString arg;
    bool okay = true;
    bool verboseFlag = false, helpFlag = false;

    for (int i = 1; i < argList.count(); i++)
    {
        arg = argList.at (i);
        if (arg.startsWith ("--"))
        {
            if (arg.contains ("version", Qt::CaseInsensitive) ||
                arg.contains ("ver", Qt::CaseInsensitive))
                verboseFlag = true;
            else if (arg.contains ("help", Qt::CaseInsensitive))
                helpFlag = true;
        }
        else if (arg.startsWith ('-'))
        {
            // Logging options
            if (arg.contains ("-l"))
            {
                if (arg.contains ('w'))
                    warn = true;
                if (arg.contains ('m'))
                    message = true;
                if (arg.contains ('r'))
                    rawData = true;
                if (arg.contains ('d'))
                    debug = true;
            }
            // help
            else if (arg.contains ("-h") || arg.contains ("-?")) // Help?
            {
                helpFlag = true;
            }
            // verbose help
            else if (arg.contains ("-v")) // Verbose output on/off
            {
                verboseFlag = true;
            }
            else if (arg.contains ("-o")) // Output file name
            {
                outputFile = argList.at (++i);
                if (arg.contains ('a'))
                    outputAllData = true;
            }
            else
            {
                arg = arg.remove (0, 1);
                // running modes
                if (arg.contains ('b'))
                {
                    batch = true;
                    arg = arg.remove ('b');
                }
                if (arg.contains ('s')) // Scenario mode
                {
                    single = true;
                    arg = arg.remove ('s');
                }
                if (arg.contains ('m')) // Monte Carlo mode
                {
                    monte = true;
                    arg = arg.remove ('m');
                }
                if (arg.contains ('i')) // "Real-time" mode
                {
                    realtime = true;
                    arg = arg.remove ('i');
                    if (arg.contains ('v'))
                        stepwise = true;
                }
                if (arg.contains ('u')) // Create summary
                {
                    summary = true;
                    arg = arg.remove ('u');
                }
                if (arg.contains ('g')) // Full GUI mode
                {
                    gui = true;
                    arg = arg.remove ('g');
                }
                if (arg.contains ('t')) // Travel time calibration mode
                {
                    ttCalib = true;
                    tspd4_print4 = 1;
                    arg = arg.remove ('t');
                }
                if (arg.contains ('a')) // Survival calibration mode
                {
                    survCalib = true;
                    arg = arg.remove ('a');
                }
                if (arg.contains ('M'))
                {
                    covar = true;
                    iterations = 0;
                    arg = arg.remove ('M');
                    iterations = arg.toInt (&okay);
                }
                // file names
                if (arg.contains ('r')) // River description file name (*.desc)
                {
                    rivDesc = argList.at (++i);
                }
                if (arg.contains ('k')) // Calibration file name (*.clb)
                {
                    calibFile = argList.at (++i);
                }
                if (arg.contains ('z')) // Release file name (*.rls)
                {
                    relFile = argList.at (++i);
                }
                if (arg.contains ('e')) // River environment file name (*.riv)
                {
                    rivYearFile = argList.at (++i);
                }
                if (arg.contains ('d')) // Dam operations file name (*.ops)
                {
                    damOpsFile = argList.at (++i);
                }
                if (arg.contains ('p')) // Post-river file name (*.pbn)
                {
                    postRivFile = argList.at (++i);
                }
                if (arg.contains ('x')) // Extra input file name (*.etc or *.cnf)
                {
                    configFile = argList.at (++i);
                }
                if (arg.contains ('c')) // Output control file name
                {
                    outControlFile = argList.at (++i);
                }
                if (arg.contains ('f')) // Scenario file name (*.scn, *.dat, or *.ctrl)
                {
                    scenarioFile = argList.at (++i);
                }
            }
        }
        else
        {
            initialData = arg;
        }
    }
    if (covar)
    {
        if (iterations <= 0)
        {
            qCritical("You must specify number of times to run the new Monte Carlo mode.\n");
        }
    }

    if (helpFlag)
    {
        if (!batch)
            batch = true;
//        print_ver ();
//        print_help (verboseFlag);
    }
    else if (verboseFlag)
    {
        if (!batch)
            batch = true;
//        print_ver();
    }
    else
    {
        /* Fixup options after scanning command line.
         */
        if (initialData.endsWith (".desc", Qt::CaseInsensitive))
        {
            rivDesc = QString (initialData);
            initialData = QString ("base.dat");
        }

        if (!single && !monte && !covar)
        {
            if (realtime)
            {
                monte = true;
                qWarning("No run mode specified, running in realtime Monte Carlo mode.\n");
            }
            if (batch)
            {
                monte = true;
                qWarning("No run mode specified, running in batch Monte Carlo mode.\n");
            }
            else
            {
                single = true;
            }
        }

//TODO: not sure if following code is needed
        if (scenarioFile.isEmpty() &&
                (rivYearFile.isEmpty() &&
                damOpsFile.isEmpty() &&
                calibFile.isEmpty() &&
                postRivFile.isEmpty()))
        {
            if ((batch && realtime) && !covar)
            {
                qWarning("Using BASE.DAT for initial data file.\n");
            }
        }
    }
}

int cmpSettings::getNumReachClasses()
{
    return numReachClasses;
}

void cmpSettings::setNumReachClasses(int newNumReachClasses)
{
    numReachClasses = newNumReachClasses;
}

bool cmpSettings::getMessage() const
{
    return message;
}

void cmpSettings::setMessage(bool newMessage)
{
    message = newMessage;
}

bool cmpSettings::getWarn() const
{
    return warn;
}

void cmpSettings::setWarn(bool newWarn)
{
    warn = newWarn;
}

bool cmpSettings::getDebug() const
{
    return debug;
}

void cmpSettings::setDebug(bool newDebug)
{
    debug = newDebug;
}

bool cmpSettings::getRawData() const
{
    return rawData;
}

void cmpSettings::setRawData(bool newRawData)
{
    rawData = newRawData;
}

bool cmpSettings::getBatch() const
{
    return batch;
}

void cmpSettings::setBatch(bool newBatch)
{
    batch = newBatch;
}

bool cmpSettings::getSingle() const
{
    return single;
}

void cmpSettings::setSingle(bool newSingle)
{
    single = newSingle;
}

bool cmpSettings::getMonte() const
{
    return monte;
}

void cmpSettings::setMonte(bool newMonte)
{
    monte = newMonte;
}

bool cmpSettings::getRealtime() const
{
    return realtime;
}

void cmpSettings::setRealtime(bool newRealtime)
{
    realtime = newRealtime;
}

bool cmpSettings::getSummary() const
{
    return summary;
}

void cmpSettings::setSummary(bool newSummary)
{
    summary = newSummary;
}

bool cmpSettings::getGui() const
{
    return gui;
}

void cmpSettings::setGui(bool newGui)
{
    gui = newGui;
}

bool cmpSettings::getCalib() const
{
    return ttCalib;
}

void cmpSettings::setCalib(bool newCalib)
{
    ttCalib = newCalib;
}

const QString &cmpSettings::getRivDesc() const
{
    return rivDesc;
}

void cmpSettings::setRivDesc(const QString &newRivDesc)
{
    rivDesc = newRivDesc;
}

const QString &cmpSettings::getCalibFile() const
{
    return calibFile;
}

void cmpSettings::setCalibFile(const QString &newCalibFile)
{
    calibFile = newCalibFile;
}

const QString &cmpSettings::getRelFile() const
{
    return relFile;
}

void cmpSettings::setRelFile(const QString &newRelFile)
{
    relFile = newRelFile;
}

const QString &cmpSettings::getRivYearFile() const
{
    return rivYearFile;
}

void cmpSettings::setRivYearFile(const QString &newRivYearFile)
{
    rivYearFile = newRivYearFile;
}

const QString &cmpSettings::getDamOpsFile() const
{
    return damOpsFile;
}

void cmpSettings::setDamOpsFile(const QString &newDamOpsFile)
{
    damOpsFile = newDamOpsFile;
}

const QString &cmpSettings::getPostRivFile() const
{
    return postRivFile;
}

void cmpSettings::setPostRivFile(const QString &newPostRivFile)
{
    postRivFile = newPostRivFile;
}

const QString &cmpSettings::getConfigFile() const
{
    return configFile;
}

void cmpSettings::setConfigFile(const QString &newConfigFile)
{
    configFile = newConfigFile;
}

const QString &cmpSettings::getOutControlFile() const
{
    return outControlFile;
}

void cmpSettings::setOutControlFile(const QString &newOutControlFile)
{
    outControlFile = newOutControlFile;
}

const QString &cmpSettings::getScenarioFile() const
{
    return scenarioFile;
}

void cmpSettings::setScenarioFile(const QString &newScenarioFile)
{
    scenarioFile = newScenarioFile;
}

const QString &cmpSettings::getOutputFile() const
{
    return outputFile;
}

void cmpSettings::setOutputFile(const QString &newOutputFile)
{
    outputFile = newOutputFile;
}

bool cmpSettings::getOutputAllData() const
{
    return outputAllData;
}

void cmpSettings::setOutputAllData(bool newOutputAllData)
{
    outputAllData = newOutputAllData;
}

bool cmpSettings::getStepwise() const
{
    return stepwise;
}

void cmpSettings::setStepwise(bool newStepwise)
{
    stepwise = newStepwise;
}

bool cmpSettings::getSurvCalib() const
{
    return survCalib;
}

void cmpSettings::setSurvCalib(bool newSurvCalib)
{
    survCalib = newSurvCalib;
}

int cmpSettings::getTspd4_print4() const
{
    return tspd4_print4;
}

void cmpSettings::setTspd4_print4(int newTspd4_print4)
{
    tspd4_print4 = newTspd4_print4;
}

bool cmpSettings::getCovar() const
{
    return covar;
}

void cmpSettings::setCovar(bool newCovar)
{
    covar = newCovar;
}

int cmpSettings::getIterations() const
{
    return iterations;
}

void cmpSettings::setIterations(int newIterations)
{
    iterations = newIterations;
}
