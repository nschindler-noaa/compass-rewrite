#ifndef C_INFORMATION_H
#define C_INFORMATION_H

#include <QList>
#include <QStringList>

#include "Dam.h"
#include "Reach.h"
#include "Headwater.h"
#include "PowerHouse.h"
#include "Release.h"
#include "Transport.h"
#include "RiverSystem.h"

struct information
{
    QStringList files;
    QStringList dams;
    QStringList powerhouses;
    QStringList reaches;
    QStringList headwaters;
    QStringList allSegments;
    QStringList allFiles;
    QStringList releases;
    QStringList transports;
    QStringList species;
    QStringList stocks;
    QList<struct release_site *> releaseSites;
    QList<River *> rivers;
    Reach * riverMouth;
};
typedef struct information Information;

void clearInfo (Information *inf)
{
    inf->riverMouth = NULL;
    inf->files.clear ();
    inf->dams.clear ();
    inf->powerhouses.clear ();
    inf->reaches.clear ();
    inf->headwaters.clear ();
    inf->allSegments.clear ();
    inf->allFiles.clear ();
    inf->releases.clear ();
    inf->transports.clear ();
    inf->species.clear ();
    inf->stocks.clear ();
    inf->releaseSites.clear ();
    inf->rivers.clear ();
}

Information *newInfo ()
{
    Information *newinfo = new Information;
    clearInfo (newinfo);
    return newinfo;
}

void deleteInfo (Information *inf)
{
    clearInfo (inf);
    delete inf;
    inf = NULL;
}

extern Information *compassInfo;

#endif // C_INFORMATION_H
