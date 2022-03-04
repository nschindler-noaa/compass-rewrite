#ifndef C_RIVERSYSTEM_H
#define C_RIVERSYSTEM_H

#include "RiverSegment.h"
#include "Species.h"
#include "Transport.h"
#include "Release.h"

#include <QObject>
#include <QStringList>

class Release;
class ReleaseSite;


class River : public QObject
{
public:
    River (QObject *parent = nullptr);
    River (QString name, QObject *parent = nullptr);
    ~River ();

    QString &getName();
    void setName(QString &value);

    float getFlowMax() const;
    void setFlowMax(float value);

    float getFlowMin() const;
    void setFlowMin(float value);

    QList<RiverSegment *> getSegments() const;
    void setSegments(const QList<RiverSegment *> &value);
    RiverSegment *getSegment(int index) const;
    void addSegment(RiverSegment *value);
    int getNumSegments();

    bool parseDesc(CompassFile *descfile);
    bool outputDesc(CompassFile *descfile);
    bool output(CompassFile *cfile);

private:
    QString name;
    float flowMax;
    float flowMin;
    QList<RiverSegment *> segments;
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
    bool parseDesc (CompassFile *descfile);
    bool parse (CompassFile *cfile);
    bool outputDesc (CompassFile *descfile);
    bool output (CompassFile *cfile);
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

extern RiverSystem *riverSystem;

#endif // C_RIVERSYSTEM_H
