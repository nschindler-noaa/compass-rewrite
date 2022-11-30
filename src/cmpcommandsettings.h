#ifndef CMPCOMMANDSETTINGS_H
#define CMPCOMMANDSETTINGS_H

//#include <QSettings>
#include <QStringList>


/** /class cmpSettings /brief Settings derived from command line arguments. */
class cmpCommandSettings
{
public:
    cmpCommandSettings();
    ~cmpCommandSettings();

    void parseArguments(QStringList &argList);

    int getNumReachClasses();
    void setNumReachClasses(int newNumReachClasses);

    bool getMessage() const;
    void setMessage(bool newMessage);

    bool getWarn() const;
    void setWarn(bool newWarn);

    bool getDebug() const;
    void setDebug(bool newDebug);

    bool getRawData() const;
    void setRawData(bool newRawData);

    bool getBatch() const;
    void setBatch(bool newBatch);

    bool getSingle() const;
    void setSingle(bool newSingle);

    bool getMonte() const;
    void setMonte(bool newMonte);

    bool getRealtime() const;
    void setRealtime(bool newRealtime);

    bool getSummary() const;
    void setSummary(bool newSummary);

    bool getGui() const;
    void setGui(bool newGui);

    bool getCalib() const;
    void setCalib(bool newCalib);

    const QString &getRivDesc() const;
    void setRivDesc(const QString &newRivDesc);

    const QString &getInitialData() const;

    const QString &getCalibFile() const;
    void setCalibFile(const QString &newCalibFile);

    const QString &getRelFile() const;
    void setRelFile(const QString &newRelFile);

    const QString &getRivYearFile() const;
    void setRivYearFile(const QString &newRivYearFile);

    const QString &getDamOpsFile() const;
    void setDamOpsFile(const QString &newDamOpsFile);

    const QString &getPostRivFile() const;
    void setPostRivFile(const QString &newPostRivFile);

    const QString &getConfigFile() const;
    void setConfigFile(const QString &newConfigFile);

    const QString &getOutControlFile() const;
    void setOutControlFile(const QString &newOutControlFile);

    const QString &getScenarioFile() const;
    void setScenarioFile(const QString &newScenarioFile);

    const QString &getOutputFile() const;
    void setOutputFile(const QString &newOutputFile);

    bool getOutputAllData() const;
    void setOutputAllData(bool newOutputAllData);

    bool getStepwise() const;
    void setStepwise(bool newStepwise);

    bool getSurvCalib() const;
    void setSurvCalib(bool newSurvCalib);

    int getTspd4_print4() const;
    void setTspd4_print4(int newTspd4_print4);

    bool getCovar() const;
    void setCovar(bool newCovar);

    int getIterations() const;
    void setIterations(int newIterations);

private:
    /** Some defaults that are not yet variable. */
    int numReachClasses;

    /** Message logging options (in addition to errors) */
    bool message; /**< Normal (informational) messages */
    bool warn;    /**< Warning messages */
    bool debug;   /**< Debugging messages */
    bool rawData; /**< raw data messages */

    /** Run mode flags */
    bool batch;   /**< run in batch (console) mode. */
    bool single;  /**< run in single scenario mode. */
    bool monte;   /**< run in monte carlo mode. */
    bool covar;   /**< run in monte carlo covariate mode. */
    int  iterations; /**< number of monte carlo covariate iterations. */
    bool realtime; /**< run in real time mode. */
    bool stepwise; /**< run in real time mode by step. */
    bool summary; /**< create a summary file. */
    bool gui;     /**< run in gui (windowed) mode. */
    bool ttCalib;   /**< run travel time calibration mode. */
    int tspd4_print4; /**<  */
    bool survCalib;   /**< run survival calibration mode. */

    bool outputAllData;   /**< output all data, even defaults. */

    /** Data files */
    QString rivDesc; /**< The river description file holds all the base
                      * river data. Default file is {columbia.desc}. */

    /** The following files contain data for the specific scenario and may be
     *  in a single scenario file, or in separate files: calibration file,
     *  release file, river year file, dam operations file,
     *  post bonneville file,and extra configuration data. The separate files
     *  can also be specified in a .ctrl file using the 'include' command.
     *  If no file is specified, the application looks for {base.dat} in
     *  the application directory.*/

    QString calibFile;    /**< holds the COMPASS calibration data (.clb). */
    QString relFile;      /**< holds data for all the releases (.rls). */
    QString rivYearFile;  /**< holds the water year information (.riv). */
    QString damOpsFile;   /**< holds all dam operations data (.ops). */
    QString postRivFile;  /**< holds data for what happens between leaving
                           *   the river and returning to it (.pbn). */
    QString configFile;   /**< holds any additional data (.ect or .cnf). */
    QString outControlFile; /**< specifies output options for river segments. */
    QString scenarioFile; /**< holds all the above data (.scn, .dat, or .ctrl). */
    QString outputFile;   /**< specifies where to create a new file. This may be
                           *   of any type, depending on the extension. */
    QString initialData;  /**< Default data file. */
};

#endif // CMPCOMMANDSETTINGS_H
