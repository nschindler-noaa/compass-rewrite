#ifndef C_SETTINGS_H
#define C_SETTINGS_H

#include "definitions.h"
#include "cmprunsettings.h"
//#include "cmpsettings.h"

#include <QString>
#include <QStringList>
#include <QSettings>

// default files are in the application directory
#define DEFAULT_RIVER_DESC  "columbia.desc"
#define DEFAULT_DATAFILE    "base.dat"

/** Check if a realtime run configured (mode is RT_SCENARIO or RT_MONTE) */
#define RT_MODE_ACTIVE(flag) ((flag)==RTScenario || (flag)==RTMonteCarlo)

/** \class cmpSettings \brief A class that holds various data that are global in nature.
 *  Saved settings (such as user name and default directory), command line
 *  parameters, file names, and run information (run_mode, activity, etc.). */
class Settings
{
public:
    /** Create a new settings structure */
    Settings();
    /** Delete the settings structure */
    ~Settings();
    /** \enum runMode definitions for modes of operation */
    enum RunMode {
        Scenario,         /**< Single scenario mode */
        MultiScenario,    /**< Multiple scenario mode */
        MonteCarlo,       /**< Monte Carlo mode */
        MonteCarloMV,     /**< Monte Carlo multivariate mode */
//        RTScenario,       /**< Real time scenario mode*/
//        RTMonteCarlo,     /**< Real time Monte Carlo mode */
//        Calibration       /**< Calibration mode */
        None
    };

    /** Retrieve saved settings from disk */
    bool restore();
    /** Save settings to disk */
    void save();
    /** Get user specific data from environment variables */
    void getCurrentUserData();
    /** Parse the command line arguments into the settings structure */
    bool readArguments(QStringList arguments);

    bool getLoading() const;
    void setLoading(bool value);
    bool getRunning() const;
    void setRunning(bool value);

    bool getRunConsole() const;
    void setRunConsole(bool value);

    RunMode getRunmode() const;
    void setRunmode(const RunMode &value);
    bool getMontecarlo() const;
    void setMontecarlo(bool value);
    bool getMultivariate() const;
    void setMultivariate(bool value);
    bool getMultiscenario() const;
    void setMultiscenario(bool value);
    bool getRealtime() const;
    void setRealtime(bool value);
    bool getCalibration() const;
    void setCalibration(bool value);
    bool getSummary() const;
    void setSummary(bool value);

    bool getBreakComputation() const;
    void setBreakComputation(bool value);

    QString getOutputData() const;
    void setOutputData(const QString &value);

    QString getCurrentUser() const;
    void setCurrentUser(const QString &value);

    QString getCurrentUserId() const;
    void setCurrentUserId(const QString &value);

    QString getUserHomeDir() const;
    void setUserHomeDir(const QString &value);

    QString getUserCurrentDir() const;
    void setUserCurrentDir(const QString &value);

    QString getApplicationDir() const;
    void setApplicationDir(const QString &value);

    QString getHostName() const;
    void setHostName(const QString &value);

    QString getCurrentDate() const;
    void setCurrentDate(const QString &value);

    QString getRiverDesc() const;
    void setRiverDesc(const QString &value);

    QString getInitialData() const;
    void setInitialData(const QString &value);

    QString getRiverData() const;
    void setRiverData(const QString &value);

    QString getDamOpsData() const;
    void setDamOpsData(const QString &value);

    QString getCalibData() const;
    void setCalibData(const QString &value);

    QString getPostbnData() const;
    void setPostbnData(const QString &value);

    QString getEtcData() const;
    void setEtcData(const QString &value);

    QString getReleaseData() const;
    void setReleaseData(const QString &value);

    QString getAdultConfigFile() const;
    void setAdultConfigFile(const QString &value);

    QString getOutputControl() const;
    void setOutputControl(const QString &value);

    QString getRTInputPath() const;
    void setRTInputPath(const QString &value);

    bool getFullGui() const;
    void setFullGui(bool value);

    int getTspd4Print4Flag() const;
    void setTspd4Print4Flag(int value);

private:
    /** Whether console (batch) mode is active, false if gui mode is active. */
    bool runConsole;

    /** A file is being read - don't disturb. */
    bool loading;
    /** A run is in progress, can be canceled with breakComputation */
    bool running;
    /** Cancel computations, exit smoothly and produce no errors */
    bool breakComputation;

    /** What kind of calculations to perform. Scenario, MonteCarlo, etc.
     *  see \ref runMode for valid options*/
    RunMode runmode;
    bool montecarlo;    /**< Run the old Monte Carlo mode - similar to crisp Monte Carlo. */
    bool multivariate;  /**< Run the newer Monte Carlo mode provided by Jim Faulkner. */
    bool multiscenario; /**< Run many scenarios or many releases. */
    bool realtime;      /**< Produce real-time output */
    bool calibration;   /**< Produce calibration output */
    /** Produce a summary.altn or summary.dat file */
    bool summary;
//    /** Pointer used in monte carlo mode */
 //   struct monte_carlo_data *monte_data;

    /** Current user name (full) */
    QString currentUser;
    /** Current user system id */
    QString currentUserId;
    /** The user's home directory */
    QString userHomeDir;
    /** The user's current directory */
    QString userCurrentDir;
    /** The application directory */
    QString applicationDir;
    /** Host name (if any - not used in Windows) */
    QString hostName;
    /** Current date - set once at beginning of session */
    QString currentDate;

    /** Input file names */
    /** River description (.desc) file */
    QString riverDesc;
    /** The initial data file to load (.dat, .scn, or .ctrl). This file
     *  will have all (or most) of the information in the files detailed
     *  below. The information in the last file loaded will overwrite
     *  previously read data. */
    QString initialData;
    /** The following files allow the user to select water year, operations,
     * calibration, release and post-Bonneville data. They are usually
     * combined in a .ctrl file using the 'include' command. */
    /** River environment (river year) file to load */
    QString riverData;
    /** Dam operations file to load */
    QString damOpsData;
    /** Calibration file to load */
    QString calibData;
    /** Post-Bonneville file to load */
    QString postbnData;
    /** Miscellaneous override file */
    QString etcData;
    /** Release file to load */
    QString releaseData;

    /** Adult config file */
    QString adultConfigFile;
    /** The output control file */
    QString outputControl;
#ifdef REALTIME
    /** Realtime input path */
    QString RTInputPath;
#endif
    // Output file name
    /** The data file to write out, only one file may be designated. */
    QString outputData;
    /** Whether all GUI options should be exposed */
    bool fullGui;
    /* This is a mild hack, it equals 1 if in adult model travel time
     *  calibration mode (use -t command line option). It causes
     *  output at 4 timesteps per day (if using 4 timesteps per day).
     *  Otherwise, output is coerced to 2 timesteps per day to
     *  accomodate the adult-realtime program's expectations.  */
    int tspd4Print4Flag;

};

//typedef struct settings Settings;

///** Create a new settings structure */
//Settings *new_settings ();
///** Fill all settings possible */
//bool set_settings (QStringList argList, Settings *set);
///** Parse the command line arguments into the settings structure */
//bool parse_arguments (QStringList argList, Settings *set);
///** Retrieve saved settings (only used in gui mode) */
//bool get_settings (Settings *set);
///** Get user specific data from environment variables */
//bool get_user_data (Settings *set);
///** Save settings (only used in gui mode) */
//bool save_settings (Settings *set);
///** Delete the settings structure */
//void delete_settings (Settings *set);

/** The global settings */
//Settings *compassSettings;

#endif // C_SETTINGS_H
