#ifndef CMPSCENARIO_H
#define CMPSCENARIO_H

#include "cmpsettings.h"
#include "cmpriversystem.h"
#include "cmprelease.h"

#include <QObject>

class cmpScenario : public QObject
{
    Q_OBJECT
public:
    explicit cmpScenario(QObject *parent = nullptr);
    cmpScenario(cmpSettings *settings, QObject *parent = nullptr);
    ~cmpScenario() override;

    void setSettings (cmpSettings *sets);
    cmpSettings *getSettings();

    void setSystem (cmpRiverSystem *sys);
    cmpRiverSystem *getSystem();

    // read in river description file
    void readDescriptionFile ();
    // read in data files
    void readDataFiles();

    int getNumberReleases();
    cmpRelease *getRelease(int index);
    void deleteReleases();
    // run the scenario
    void run();
    void runScenario();
    void runMonteCarlo();
    void runMonteCarloMV();
    void runRealTime();
    // write any output files
    void outputData(QString filename, bool outputAll);
    void writeDescData(cmpFile *outfile);
    QStringList &writeCtrlData(cmpFile *outfile);
    void writeDataSettings(cmpFile *outfile, bool outputAll);
    void writeReachClassNames(cmpFile *outfile);
    void writeAllSpecies(cmpFile *outfile, bool outputAll);
    void writeAllStocks(cmpFile *outfile, bool outputAll);
    void writeRunSettings(cmpFile *outfile, bool outputAll);
    void writeRiverYrData(cmpFile *outfile, bool outputAll);
    void writeDamOpsData(cmpFile *outfile, bool outputAll);
    void writeCalibData(cmpFile *outfile, bool outputAll);
    void writeReleaseData(cmpFile *outfile, bool outputAll);
    void writePostRivData(cmpFile *outfile, bool outputAll);
    void writeConfigData(cmpFile *outfile, bool outputAll);
    void writeScnData(cmpFile *outfile, bool outputAll);


    QStringList &getPostRivMethods();
    void setPostRivMethods();

signals:
    void done();
    void canceled();

private:
    cmpSettings *settings;
    QList<cmpFile *> files;
    cmpRiverSystem *system;
    QList<cmpRelease *> releases;
    QStringList fileNames;
    QStringList postRivMethods;
    QStringList postRivMethodNames;
    QStringList outputFiles;
};

#endif // CMPSCENARIO_H
