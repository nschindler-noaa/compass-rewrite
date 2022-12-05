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
    void outputData();

    QStringList *getPostRivMethods() const;
    void setPostRivMethods();

signals:
    void done();
    void canceled();

private:
    cmpSettings *settings;
    cmpRiverSystem *system;
    QList<cmpRelease *> releases;
    QStringList *files;
    QStringList *postRivMethods;
    QStringList *postRivMethodNames;

};

#endif // CMPSCENARIO_H
