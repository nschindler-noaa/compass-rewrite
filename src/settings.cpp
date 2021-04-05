#include <QProcess>
#include <QDate>

#include "definitions.h"
#include "settings.h"
#include "help.h"
#include "Log.h"

/** Create a new settings structure */
Settings::Settings ()
{
    running = false;
    runConsole = false;
    montecarlo = false;
    multiscenario = false;
    realtime = false;
    calibration = false;
    breakComputation = false;
    summary = false;
 //   set->monte_data =  make_monte_data ();

    currentUser = QString ("");
    currentUserId = QString ("");
    userHomeDir = QString ("");
    hostName = QString ("");
    currentDate = QString ("");
    riverDesc = QString ("");
    initialData = QString ("");

    riverData = QString ("");
    damOpsData = QString ("");
    calibData = QString ("");
    releaseData = QString ("");
    postbnData = QString ("");
    etcData = QString ("");

    adultConfigFile = QString ("");
    outputControl = QString ("");
#ifdef REALTIME
    RTInputPath = QString ("");
#endif
    outputData = QString ("");
    fullGui = false;
    tspd4Print4Flag = 0;
}

/** Delete the settings structure */
Settings::~Settings()
{
//    free_monte_data (set->monte_data);

    /** Current user name (full) */
    currentUser.clear ();
    /** Current user system id */
    currentUserId.clear ();
    /** The user's home directory */
    userHomeDir.clear ();
    /** Host name (if any - not used in Windows) */
    hostName.clear ();
    /** Current date - get this once */
    currentDate.clear ();
    /** The river description file */
    riverDesc.clear ();
    /** The initial data file to load */
    initialData.clear ();

    /* These four files allow the user to select water year, operations,
     * calibration, release and post-Bonneville data */
    /** River environment (river year) file to load */
    riverData.clear ();
    /** Dam operations file to load */
    damOpsData.clear ();
    /** Calibration file to load */
    calibData.clear ();
    /** Release file to load */
    releaseData.clear ();
    /** Post-Bonneville file to load */
    postbnData.clear ();
    /** Miscellaneous override file */
    etcData.clear ();

    /** Adult config file */
    adultConfigFile.clear ();
    /** The output control file to write out */
    outputControl.clear ();
#ifdef REALTIME
    /** Realtime input path */
    RTInputPath.clear ();
#endif
    /** The input file to write out */
    outputData.clear ();

}

/*bool Settings::re

bool set_settings(QStringList argList, Settings *set)
{
    bool okay = true;
    // get command list arguments first
    okay = parse_arguments(argList, set);
    // get user data settings
    okay = get_user_data (set);

    return okay;
}*/

/** Parse the command line arguments into the settings structure */
bool Settings::readArguments(QStringList arguments)
{
    bool okay = true;
    Settings::RunMode mode_flag = Settings::None;
//    bool realtime_flag = false;
//    bool summary_flag = false;
    bool verbose_flag = false;
    bool help_flag = false;

//    int tt_calib_mode;
    QString arg ("");

    applicationDir.append(arguments.at (0).section ('/', 0, -2));

    for (int i = 1; i < arguments.count(); i++)
    {
        arg = QString (arguments.at(i));

        // command options (batch mode)
        if (arg.startsWith ('-'))
        {
            // check for help flags
            arg = arg.section ('-', -1);
            if (arg.compare ("h", Qt::CaseInsensitive) == 0)
                help_flag = true;
            else if (arg.compare ("v", Qt::CaseInsensitive) == 0)
                verbose_flag = true;
            // check for logging options
            else if (arg.startsWith ('l', Qt::CaseInsensitive))
            {
                if (arg.contains ('w'))
                    Log::outlog->setWarning (true);
                if (arg.contains ('r'))
                    Log::outlog->setRawDump (true);
                if (arg.contains ('m'))
                    Log::outlog->setMessage (true);
                if (arg.contains ('d'))
                    Log::outlog->setDebug (true);
            }
            // check for files
            // river description file
            else if (arg.compare ("r") == 0)
            {
                setRiverDesc (arguments.at (++i));
            }
            // calibration file
            else if (arg.compare ("k") == 0)
            {
                setCalibData (arguments.at (++i));
            }
            // release file
            else if (arg.compare ("z") == 0)
            {
                setReleaseData (arguments.at (++i));
            }
            // river year data file
            else if (arg.compare ("e") == 0)
            {
                setRiverData (arguments.at (++i));
            }
            // dam operations file
            else if (arg.compare ("d") == 0)
            {
                setDamOpsData (arguments.at (++i));
            }
            // post bonneville data (post hydrosystem)
            else if (arg.compare ("p") == 0)
            {
                setPostbnData (arguments.at (++i));
            }
            // any additional data
            else if (arg.compare ("x") == 0)
            {
                setEtcData (arguments.at (++i));
            }
            // output settings data
            else if (arg.compare ("c") == 0)
            {
                setOutputControl (arguments.at (++i));
            }
            // initial data (.scn or .dat file)
            else if (arg.compare ("f") == 0)
            {
                setInitialData (arguments.at (++i));
            }
            // output file
            else if (arg.compare ("o") == 0)
            {
                setOutputData (arguments.at (++i));
            }
            // set the mode flags
            else
            {
                if (arg.contains ('b'))
                    setBatch (true);
                if (arg.contains ('s'))
                    setMontecarlo (false);
//                    mode_flag = Settings::Scenario;
                if (arg.contains ('m'))
                    setMontecarlo (true);
//                    mode_flag = Settings::MonteCarlo;
                if (arg.contains ('i'))
                    setRealtime (true);
                if (arg.contains ('u'))
                    setSummary (true);
                if (arg.contains ('t'))
                    setCalibration (true);//tt_calib_mode = 1;
            }
        }
        // otherwise (gui mode), it's a river description file.
        else
        {
            setRiverDesc (arg);
        }
    }

    if (help_flag || !okay)
    {
        Log::outlog->add (Log::Force, version_string());
        Log::outlog->add (Log::Force, help_string(verbose_flag));
    }
    else
    {
        /* Fixup options after scanning command line.
         */
        if (riverDesc.isEmpty())
        {
            setRiverDesc(QString (DEFAULT_RIVER_DESC));
        }
        // default for batch mode is monte carlo, for gui is scenario
        if (mode_flag == Settings::None)
        {
            if (runConsole)
                mode_flag = Settings::MonteCarlo;
            else
                mode_flag = Settings::Scenario;
        }
        /* combine mode flags
        if (realtime)
        {
            if (mode_flag == Settings::Scenario)
                mode_flag = Settings::RTScenario;
            else
                mode_flag = Settings::RTMonteCarlo;
        }
        else if (calibration)
        {
            mode_flag = Settings::Calibration;
        }*/
        runmode = mode_flag;

        // default scenario data is in DEFAULT_DATAFILE
        if (initialData.isEmpty() &&
           (riverData.isEmpty() &&
            damOpsData.isEmpty() &&
            calibData.isEmpty() &&
            postbnData.isEmpty()))
        {
            if (runConsole)
            {
                setInitialData (QString (DEFAULT_DATAFILE));
                Log::outlog->add (Log::Message,
                      QString ("Using %1 for initial data file.")
                         .arg (DEFAULT_DATAFILE));
            }
        }
    }
    return okay;
}

/** Retrieve saved settings */
bool Settings::restore()
{
    bool okay = true;

    return okay;
}

/** Get user specific data from environment variables */
void Settings::getCurrentUserData()
{
    bool okay = true;
    int index = -1;
    char name[100];
    QString homepath ("");
    QStringList env (QProcess::systemEnvironment());

    // get the user's name
#ifndef WIN32
    index = env.indexOf (QRegExp("USER*", Qt::CaseSensitive, QRegExp::Wildcard));
#endif
    if (index > -1)
    {
        okay = true;
        currentUser = env.at(index).section ('=', -1);
    }
    else
    {
        okay = false;
        currentUser = QString (" ");
    }
    // get the user's system id
#ifdef WIN32
    index = env.indexOf (QRegExp("USERNAME*", Qt::CaseSensitive, QRegExp::Wildcard));
#else
    index = env.indexOf (QRegExp("user*", Qt::CaseSensitive, QRegExp::Wildcard));
#endif
    if (index > -1)
    {
        okay = true;
        currentUserId = env.at(index).section ('=', -1);
    }
    else
    {
        okay = false;
        currentUserId = QString ("");//okay = false;
    }
    // if no name, use the user's system id
    if (currentUser.isEmpty ())
        currentUser = currentUserId;

    // get the user's home directory
#ifdef WIN32
    index = env.indexOf (QRegExp("HOMEDRIVE*", Qt::CaseSensitive, QRegExp::Wildcard));
    if (index > -1)
    {
        okay = true;
        homepath = env.at(index).section ('=', -1);
        index = env.indexOf (QRegExp("HOMEPATH*", Qt::CaseSensitive, QRegExp::Wildcard));
        if (index > -1)
            homepath.append (env.at(index).section ('=', -1));
        else
            homepath.clear();
    }
    if (homepath.isEmpty())
    {
        index = env.indexOf (QRegExp("USERPROFILE*", Qt::CaseSensitive, QRegExp::Wildcard));
        if (index > -1)
        {
            okay = true;
            homepath = env.at(index).section ('=', -1);
        }
        else
        {
            okay = false;
        }
    }
#else
    index = env.indexOf (QRegExp("HOME*", Qt::CaseSensitive, QRegExp::Wildcard));
    if (index > -1)
    {
        okay = true;
        homepath = env.at(index).section ('=', -1);
    }
#endif
    userHomeDir = homepath;

    // get the host name - applies only to Linux systems
#ifdef WIN32
    name[0] = '\0';
#else
    /* get the host name */

    if (gethostname(name,100))
    {
        Log::outlog->add (Log::Warning,
                "Error in gethostname.  Called from write_header.\n");
        name[0] = '\0';
    }
#ifdef SYSV
    if ( !(host = gethostbyname(name)) )
        Log::outlog->add (Log::Warning,
                "Error getting full host name. Unknown_host_error.\n%s%s\n",
                "Host was: ",
                name);
#endif /* SYSV */
#endif /* WIN32 */
    hostName = QString (name);

    // get the current date
    currentDate = QDate::currentDate().toString (QString ("yyyy-MM-dd"));
}

/** Save settings */
void Settings::save()
{
    bool okay = true;


}

bool Settings::getLoading() const
{
    return loading;
}

void Settings::setLoading(bool value)
{
    loading = value;
}

bool Settings::getRunning() const
{
    return running;
}

void Settings::setRunning(bool value)
{
    running = value;
}

bool Settings::getRunConsole() const
{
    return runConsole;
}

void Settings::setBatch(bool value)
{
    runConsole = value;
}

Settings::RunMode Settings::getRunmode() const
{
    return runmode;
}

void Settings::setRunmode(const RunMode &value)
{
    runmode = value;
}

bool Settings::getMontecarlo() const
{
    return montecarlo;
}

void Settings::setMontecarlo(bool value)
{
    montecarlo = value;
    if (montecarlo)
    {
        setRunmode(Settings::MonteCarlo);
    }
    else
    {
        setRunmode(Settings::Scenario);
    }
}

bool Settings::getMultivariate() const
{
    return multivariate;
}

void Settings::setMultivariate(bool value)
{
    multivariate = value;
    if (multivariate)
        setRunmode(Settings::MonteCarloMV);
}

bool Settings::getMultiscenario() const
{
    return multiscenario;
}

void Settings::setMultiscenario(bool value)
{
    multiscenario = value;
    if (multiscenario)
        setRunmode(Settings::MultiScenario);
}

bool Settings::getRealtime() const
{
    return realtime;
}

void Settings::setRealtime(bool value)
{
    realtime = value;
}

bool Settings::getCalibration() const
{
    return calibration;
}

void Settings::setCalibration(bool value)
{
    calibration = value;
}

bool Settings::getSummary() const
{
    return summary;
}

void Settings::setSummary(bool value)
{
    summary = value;
}

bool Settings::getBreakComputation() const
{
    return breakComputation;
}

void Settings::setBreakComputation(bool value)
{
    breakComputation = value;
}

QString Settings::getOutputData() const
{
    return outputData;
}

void Settings::setOutputData(const QString &value)
{
    outputData = QString(value);
}
QString Settings::getCurrentUser() const
{
    return currentUser;
}

void Settings::setCurrentUser(const QString &value)
{
    currentUser = QString(value);
}
QString Settings::getCurrentUserId() const
{
    return currentUserId;
}

void Settings::setCurrentUserId(const QString &value)
{
    currentUserId = QString(value);
}
QString Settings::getUserHomeDir() const
{
    return userHomeDir;
}

void Settings::setUserHomeDir(const QString &value)
{
    userHomeDir = QString(value);
}
QString Settings::getUserCurrentDir() const
{
    return userCurrentDir;
}

void Settings::setUserCurrentDir(const QString &value)
{
    userCurrentDir = QString(value);
}
QString Settings::getApplicationDir() const
{
    return applicationDir;
}

void Settings::setApplicationDir(const QString &value)
{
    applicationDir = QString(value);
}
QString Settings::getHostName() const
{
    return hostName;
}

void Settings::setHostName(const QString &value)
{
    hostName = QString(value);
}
QString Settings::getCurrentDate() const
{
    return currentDate;
}

void Settings::setCurrentDate(const QString &value)
{
    currentDate = QString(value);
}
QString Settings::getRiverDesc() const
{
    return riverDesc;
}

void Settings::setRiverDesc(const QString &value)
{
    riverDesc = QString(value);
}
QString Settings::getInitialData() const
{
    return initialData;
}

void Settings::setInitialData(const QString &value)
{
    initialData = QString(value);
}
QString Settings::getRiverData() const
{
    return riverData;
}

void Settings::setRiverData(const QString &value)
{
    riverData = QString(value);
}
QString Settings::getDamOpsData() const
{
    return damOpsData;
}

void Settings::setDamOpsData(const QString &value)
{
    damOpsData = QString(value);
}
QString Settings::getCalibData() const
{
    return calibData;
}

void Settings::setCalibData(const QString &value)
{
    calibData = QString(value);
}
QString Settings::getPostbnData() const
{
    return postbnData;
}

void Settings::setPostbnData(const QString &value)
{
    postbnData = QString(value);
}
QString Settings::getEtcData() const
{
    return etcData;
}

void Settings::setEtcData(const QString &value)
{
    etcData = QString(value);
}
QString Settings::getReleaseData() const
{
    return releaseData;
}

void Settings::setReleaseData(const QString &value)
{
    releaseData = QString(value);
}
QString Settings::getAdultConfigFile() const
{
    return adultConfigFile;
}

void Settings::setAdultConfigFile(const QString &value)
{
    adultConfigFile = QString(value);
}
QString Settings::getOutputControl() const
{
    return outputControl;
}

void Settings::setOutputControl(const QString &value)
{
    outputControl = QString(value);
}
QString Settings::getRTInputPath() const
{
    return RTInputPath;
}

void Settings::setRTInputPath(const QString &value)
{
    RTInputPath = QString(value);
}
bool Settings::getFullGui() const
{
    return fullGui;
}

void Settings::setFullGui(bool value)
{
    fullGui = value;
}
int Settings::getTspd4Print4Flag() const
{
    return tspd4Print4Flag;
}

void Settings::setTspd4Print4Flag(int value)
{
    tspd4Print4Flag = value;
}






























