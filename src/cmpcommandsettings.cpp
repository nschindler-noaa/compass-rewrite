
#include "cmpcommandsettings.h"

#include <qlogging.h>

cmpCommandSettings::cmpCommandSettings()
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

    rivDesc = QString("columbia.desc"); // historical default
    calibFile = QString();
    relFile = QString();
    rivYearFile = QString();
    damOpsFile = QString();
    postRivFile = QString();
    configFile = QString();            // "base.etc"
    outControlFile = QString();
    scenarioFile = QString();
    outputFile = QString();
    initialData = QString("base.dat"); // basic "base.dat"
}

cmpCommandSettings::~cmpCommandSettings()
{}

void cmpCommandSettings::parseArguments(QStringList &argList)
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
                if (arg.contains("v"))
                    verboseFlag = true; // if combined with help flag
            }
            // verbose help
            else if (arg.contains ("-v")) // Verbose output on/off
            {
                verboseFlag = true;
            }
            else if (arg.contains ("-o")) // Output data file name
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
                initialData = QString("base.dat");
            }
        }

        // create list of files

        if (!scenarioFile.isEmpty())
            fileNames.append(scenarioFile);
        if (!rivYearFile.isEmpty())
            fileNames.append(rivYearFile);
        if (!damOpsFile.isEmpty())
            fileNames.append(damOpsFile);
        if (!postRivFile.isEmpty())
            fileNames.append(postRivFile);
        if (fileNames.isEmpty() && !initialData.isEmpty())
            fileNames.append(initialData);
        if (!calibFile.isEmpty())
            fileNames.append(calibFile);
        if (!configFile.isEmpty())
            fileNames.append(configFile);
        if (!outControlFile.isEmpty())
            fileNames.append(outControlFile);
        if (!relFile.isEmpty())
            fileNames.append(relFile);
    }
}

int cmpCommandSettings::getNumReachClasses()
{
    return numReachClasses;
}

void cmpCommandSettings::setNumReachClasses(int newNumReachClasses)
{
    numReachClasses = newNumReachClasses;
}

bool cmpCommandSettings::getMessage() const
{
    return message;
}

void cmpCommandSettings::setMessage(bool newMessage)
{
    message = newMessage;
}

bool cmpCommandSettings::getWarn() const
{
    return warn;
}

void cmpCommandSettings::setWarn(bool newWarn)
{
    warn = newWarn;
}

bool cmpCommandSettings::getDebug() const
{
    return debug;
}

void cmpCommandSettings::setDebug(bool newDebug)
{
    debug = newDebug;
}

bool cmpCommandSettings::getRawData() const
{
    return rawData;
}

void cmpCommandSettings::setRawData(bool newRawData)
{
    rawData = newRawData;
}

bool cmpCommandSettings::getBatch() const
{
    return batch;
}

void cmpCommandSettings::setBatch(bool newBatch)
{
    batch = newBatch;
}

bool cmpCommandSettings::getSingle() const
{
    return single;
}

void cmpCommandSettings::setSingle(bool newSingle)
{
    single = newSingle;
}

bool cmpCommandSettings::getMonte() const
{
    return monte;
}

void cmpCommandSettings::setMonte(bool newMonte)
{
    monte = newMonte;
}

bool cmpCommandSettings::getRealtime() const
{
    return realtime;
}

void cmpCommandSettings::setRealtime(bool newRealtime)
{
    realtime = newRealtime;
}

bool cmpCommandSettings::getSummary() const
{
    return summary;
}

void cmpCommandSettings::setSummary(bool newSummary)
{
    summary = newSummary;
}

bool cmpCommandSettings::getGui() const
{
    return gui;
}

void cmpCommandSettings::setGui(bool newGui)
{
    gui = newGui;
}

bool cmpCommandSettings::getCalib() const
{
    return ttCalib;
}

void cmpCommandSettings::setCalib(bool newCalib)
{
    ttCalib = newCalib;
}

const QString &cmpCommandSettings::getRivDesc() const
{
    return rivDesc;
}

void cmpCommandSettings::setRivDesc(const QString &newRivDesc)
{
    rivDesc = newRivDesc;
}

const QStringList &cmpCommandSettings::getFileNameList() const
{
    return fileNames;
}

const QString &cmpCommandSettings::getInitialData() const
{
    return initialData;
}

const QString &cmpCommandSettings::getCalibFile() const
{
    return calibFile;
}

void cmpCommandSettings::setCalibFile(const QString &newCalibFile)
{
    calibFile = newCalibFile;
}

const QString &cmpCommandSettings::getRelFile() const
{
    return relFile;
}

void cmpCommandSettings::setRelFile(const QString &newRelFile)
{
    relFile = newRelFile;
}

const QString &cmpCommandSettings::getRivYearFile() const
{
    return rivYearFile;
}

void cmpCommandSettings::setRivYearFile(const QString &newRivYearFile)
{
    rivYearFile = newRivYearFile;
}

const QString &cmpCommandSettings::getDamOpsFile() const
{
    return damOpsFile;
}

void cmpCommandSettings::setDamOpsFile(const QString &newDamOpsFile)
{
    damOpsFile = newDamOpsFile;
}

const QString &cmpCommandSettings::getPostRivFile() const
{
    return postRivFile;
}

void cmpCommandSettings::setPostRivFile(const QString &newPostRivFile)
{
    postRivFile = newPostRivFile;
}

const QString &cmpCommandSettings::getConfigFile() const
{
    return configFile;
}

void cmpCommandSettings::setConfigFile(const QString &newConfigFile)
{
    configFile = newConfigFile;
}

const QString &cmpCommandSettings::getOutControlFile() const
{
    return outControlFile;
}

void cmpCommandSettings::setOutControlFile(const QString &newOutControlFile)
{
    outControlFile = newOutControlFile;
}

const QString &cmpCommandSettings::getScenarioFile() const
{
    return scenarioFile;
}

void cmpCommandSettings::setScenarioFile(const QString &newScenarioFile)
{
    scenarioFile = newScenarioFile;
}

const QString &cmpCommandSettings::getOutputFile() const
{
    return outputFile;
}

void cmpCommandSettings::setOutputFile(const QString &newOutputFile)
{
    outputFile = newOutputFile;
}

bool cmpCommandSettings::getOutputAllData() const
{
    return outputAllData;
}

void cmpCommandSettings::setOutputAllData(bool newOutputAllData)
{
    outputAllData = newOutputAllData;
}

bool cmpCommandSettings::getStepwise() const
{
    return stepwise;
}

void cmpCommandSettings::setStepwise(bool newStepwise)
{
    stepwise = newStepwise;
}

bool cmpCommandSettings::getSurvCalib() const
{
    return survCalib;
}

void cmpCommandSettings::setSurvCalib(bool newSurvCalib)
{
    survCalib = newSurvCalib;
}

int cmpCommandSettings::getTspd4_print4() const
{
    return tspd4_print4;
}

void cmpCommandSettings::setTspd4_print4(int newTspd4_print4)
{
    tspd4_print4 = newTspd4_print4;
}

bool cmpCommandSettings::getCovar() const
{
    return covar;
}

void cmpCommandSettings::setCovar(bool newCovar)
{
    covar = newCovar;
}

int cmpCommandSettings::getIterations() const
{
    return iterations;
}

void cmpCommandSettings::setIterations(int newIterations)
{
    iterations = newIterations;
}
