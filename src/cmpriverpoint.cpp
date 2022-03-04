#include "cmpriverpoint.h"

#include <QStringList>

cmpRiverPoint::cmpRiverPoint(QString line)
{
    reset ();
    if (!line.isEmpty())
    {
        parse(line);
    }
}
cmpRiverPoint::cmpRiverPoint(cmpRiverPoint &other)
{
    copy(other);
}

cmpRiverPoint::~cmpRiverPoint ()
{
    reset ();
}

void cmpRiverPoint::reset ()
{
    lon = 0.0;
    lat = 0.0;
    updateText();
}

void cmpRiverPoint::parse(QString line)
{
    QStringList tokens = line.split(' ', QString::SkipEmptyParts);
    parse(tokens);
}

void cmpRiverPoint::parse(QStringList tokens)
{
    setLatLon(tokens);
}

double cmpRiverPoint::setLon (const double x)
{
    lon = x;
    return lon;
}

double cmpRiverPoint::setLon (int d, int m, int s)
{
    lon  = s / 3600.0;
    lon += m / 60.0;
    lon += d;
    return lon;
}

double cmpRiverPoint::setLon (QString deg, QString min, QString sec)
{
    int d = deg.toInt ();
    int m = min.toInt ();
    int s = sec.toInt ();
    return setLon (d, m, s);
}

void cmpRiverPoint::setLonDir (QString ew)
{
    if (ew.contains ("W", Qt::CaseInsensitive))
        lon = lon < 0.0? -lon: lon;
    else
        lon = lon > 0.0? -lon: lon;
}


double cmpRiverPoint::setLat (const double x)
{
    lat = x;
    return lat;
}

double cmpRiverPoint::setLat (int d, int m, int s)
{
    lat  = s / 3600.0;
    lat += m / 60.0;
    lat += d;
    return lat;
}

double cmpRiverPoint::setLat (QString deg, QString min, QString sec)
{
    int d = deg.toInt ();
    int m = min.toInt ();
    int s = sec.toInt ();
    return setLat (d, m, s);
}

void cmpRiverPoint::setLatDir (QString ns)
{
    if (ns.compare ("S", Qt::CaseInsensitive) == 0)
        if (lat > 0)
            lat = lat * -1.0;
}

void cmpRiverPoint::copy(cmpRiverPoint &other)
{
    setLat(other.getLat());
    setLon(other.getLon());
    updateText();
}

cmpRiverPoint & cmpRiverPoint::operator=(cmpRiverPoint &other)
{
    copy(other);
    return *this;
}

bool cmpRiverPoint::equals (const cmpRiverPoint rhs)
{
    double prec = .0001;
    bool equal = true;
    if (lon < rhs.lon - prec || lon > rhs.lon + prec)
        equal = false;
    if (lat < rhs.lat - prec || lat > rhs.lat + prec)
        equal = false;

    return equal;
}

bool cmpRiverPoint::operator == (const cmpRiverPoint rhs)
{
    double prec = .0001;
    bool equal = true;
    if (lon < rhs.lon - prec || lon > rhs.lon + prec)
        equal = false;
    if (lat < rhs.lat - prec || lat > rhs.lat + prec)
        equal = false;

    return equal;
}


QString &cmpRiverPoint::getLatLon ()
{
    return text;
}

void cmpRiverPoint::setLatLon(const QString txt)
{
    QStringList items;
    items = txt.split(' ', QString::SkipEmptyParts);
    if (items.at(0).compare("latlon", Qt::CaseInsensitive) == 0)
        items.takeFirst();
    setLatLon(items);
}

void cmpRiverPoint::setLatLon (const QStringList items)
{
    if (items.count() == 8)
    {
        setLat (items.at(0), items.at(1), items.at(2));
        setLatDir(items.at(3));
        setLon (items.at(4), items.at(5), items.at(6));
        setLonDir(items.at(7));
        text = QString(QString("%1 %2 %3 %4 %5 %6 %7 %8").arg(
                           items.at(0),items.at(1),items.at(2),items.at(3),
                          items.at(4),items.at(5),items.at(6),items.at(7)));
    }
}

QString & cmpRiverPoint::updateText()
{
    text.clear();
    double absLon = abs(lon);
    double absLat = abs(lat);
    int d, m, s;

    d = static_cast<int>(absLat);
    m = static_cast<int>((absLat - d) * 60.0);
    s = static_cast<int>((absLat - d - (m / 60.0)) * 60.0);
    text.append (QString::number (d));
    text.append (' ');
    text.append (QString::number (m));
    text.append (' ');
    text.append (QString::number (s));
    text.append (' ');
    if (lat > 0)
        text.append ("N ");
    else
        text.append ("S ");

    d = static_cast<int>(absLon);
    m = static_cast<int>((absLon - d) * 60.0);
    s = static_cast<int>((absLon - d - (m / 60.0)) * 60.0);
    text.append (QString::number (d));
    text.append (' ');
    text.append (QString::number (m));
    text.append (' ');
    text.append (QString::number (s));
    text.append (' ');
    if (lon > 0)
        text.append ("E ");
    else
        text.append ("W ");

    return text;
}

double cmpRiverPoint::distance(cmpRiverPoint &other)
{
    double dist = 0;
    return dist;
}

