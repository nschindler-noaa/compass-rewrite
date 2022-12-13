#ifndef CMPRIVERSYSTEM_H
#define CMPRIVERSYSTEM_H

#include "cmpriver.h"
#include "cmpspecies.h"
#include "cmpstock.h"
#include "cmptransport.h"
#include "cmpriversegment.h"
#include "cmpdam.h"
#include "cmpreach.h"
#include "cmpheadwater.h"
#include "cmpreleasesite.h"
#include "cmprelease.h"
#include "cmpsettings.h"

#include <QObject>

class cmpRiverSystem : public QObject
{
    Q_OBJECT
public:
    explicit cmpRiverSystem(QObject *parent = nullptr);
    cmpRiverSystem (QString filename, QObject *parent = nullptr);
    ~cmpRiverSystem () override;
    void setup ();
    void resetData ();
    void deleteAll ();

    cmpSettings *cSettings; // not created here, just a reference

    QList <cmpSpecies *> species;
    QList <cmpStock *> stocks;
    QList <cmpRiver *> rivers;
    QList <cmpRiverSegment *> segments;
    QList <cmpTransport *> transports;
    QList <cmpRelease *> releases;
    QList <cmpReleaseSite *> releaseSites;

    QStringList speciesNames;
    QStringList stockNames;
    QStringList reachClassNames;
    QStringList powerhouses;
    QStringList dams;
    QStringList reaches;
    QStringList headwaters;
    QStringList basins;

    cmpRiver *findRiver (QString name);
    cmpRiverSegment *findSegment (QString name);
    cmpRiverSegment *findSegment (cmpRiverPoint *pt);
    cmpSpecies *findSpecies (QString name);
    cmpStock *findStock (QString name);
    cmpTransport *findTransport (QString name);
    cmpRelease *findRelease (QString name);
    cmpReleaseSite *findReleaseSite (QString name);

    cmpSettings *getSettings() const;
    void setSettings(cmpSettings *newCSettings);

    const QStringList &getReachClassNames() const;
    void setReachClassName(int index, const QString &newReachClassName);

signals:
    void constructed (bool okay);
    void flows_computed (bool okay);
    void temps_computed (bool okay);


public slots:
    bool parseDesc (cmpFile *descfile);
    bool parseData (cmpFile *cfile);
    bool parseReleaseSite(cmpFile *cfile, cmpReleaseSite *relsite);
    bool outputDesc (cmpFile *descfile);
    bool outputData (cmpFile *cfile);
    bool initialize ();
    bool construct ();
    void computeFlows ();
    void markRegulationPts ();
    void fillHeadwaters ();
    void computeSegFlow (cmpRiverSegment *seg);
    void computeTemps ();
    void computeSegTemp (cmpRiverSegment *seg);
    void computeSpill ();
    void computeSegSpill (cmpRiverSegment *seg);
    cmpRiverSegment *getMouth() {return segments.at(0);}
    void deleteReleases ();
    void deleteSpill ();
    void allocate(int numDays, int numSteps, int numSlices);

private:

};

#endif // CMPRIVERSYSTEM_H
