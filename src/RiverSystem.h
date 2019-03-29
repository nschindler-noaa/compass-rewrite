#ifndef C_RIVERSYSTEM_H
#define C_RIVERSYSTEM_H

#include <QObject>
#include <QStringList>
//#include <QList>
#include "RiverSegment.h"
#include "Species.h"
#include "Transport.h"
#include "Release.h"

class Release;
class ReleaseSite;

/*struct river {

    QString * name;
    float flowMax;
    float flowMin;
};
typedef struct river River;
River * newRiver (QString name = QString (""));
void deleteRiver (River *riv);
*/
class River : public QObject
{
public:
    River (QObject *parent = nullptr);
    River (QString name, QObject *parent = nullptr);
    ~River ();

    QString * name;
    float flowMax;
    float flowMin;
};

class RiverSystem : public QObject
{
    Q_OBJECT

public:
    RiverSystem (QObject *parent = nullptr);
    RiverSystem (QString filename, QObject *parent = nullptr);
    ~RiverSystem ();
    void setup ();
    void reset ();
    void deleteAll ();

    QList <River *> *rivers;
    QList <RiverSegment *> *segments;
    QList <Species *> *species;
    QList <Stock *> *stocks;
    QList <Transport *> *transports;
    QList <Release *> *releases;
    QList <ReleaseSite *> *releaseSites;

    QStringList *speciesNames;
    QStringList *stockNames;
    QStringList *powerhouses;
    QStringList *dams;
    QStringList *reaches;
    QStringList *headwaters;
    QStringList *basins;

    River *findRiver (QString name);
    RiverSegment *findSegment (QString name);
    RiverSegment *findSegment (RiverPoint *pt);
    Species *findSpecies (QString name);
    Stock *findStock (QString name);
    Transport *findTransport (QString name);
    Release *findRelease (QString name);
    ReleaseSite *findReleaseSite (QString name);


signals:
    void constructed (bool okay);
    void flows_computed (bool okay);
    void temps_computed (bool okay);


public slots:
    bool parseDesc (CompassFile *cfile);
    bool parse (CompassFile *cfile);
    bool initialize ();
    bool construct ();
    void computeFlows ();
    void markRegulationPts ();
    void fillHeadwaters ();
    void computeSegFlow (RiverSegment *seg);
    void computeTemps ();
    void computeSegTemp (RiverSegment *seg);
    void computeSpill ();
    void computeSegSpill (RiverSegment *seg);
    RiverSegment *getMouth() {return segments->at(0);}
    void deleteReleases ();
    void deleteSpill ();

private:
};

//extern RiverSystem *riverSystem;

#endif // C_RIVERSYSTEM_H
