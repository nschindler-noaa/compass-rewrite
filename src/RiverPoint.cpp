#include "RiverPoint.h"

#include <QStringList>

RiverPoint::RiverPoint()
{
    reset ();
}
RiverPoint::RiverPoint(RiverPoint &other)
{
    copy(other);
}
RiverPoint::~RiverPoint ()
{
    reset ();
}
void RiverPoint::reset ()
{
    lon = 0.0;
    lat = 0.0;
    width = 0.0;
    updateText();
}

double RiverPoint::setLon (const double x)
{
    lon = x;
    return lon;
}

double RiverPoint::setLon (int d, int m, int s)
{
    lon = (double)d + (((double)m + ((double)s / 60.0)) / 60.0);
    return lon;
}

double RiverPoint::setLon (QString deg, QString min, QString sec)
{
    int d = deg.toInt ();
    int m = min.toInt ();
    int s = sec.toInt ();
    return setLon (d, m, s);
}

void RiverPoint::setLonDir (QString ew)
{
    if (ew.contains ("W", Qt::CaseInsensitive))
        lon = lon < 0.0? -lon: lon;
    else
        lon = lon > 0.0? -lon: lon;
}


double RiverPoint::setLat (const double x)
{
    lat = x;
    return lat;
}

double RiverPoint::setLat (int d, int m, int s)
{
    lat = (double) s / 60.0;
    lat += (double) m / 60.0;
    lat += (double) d;
//    lat = (double)d + (((double)m + ((double)s / 60.0)) / 60.0);
    return lat;
}

double RiverPoint::setLat (QString deg, QString min, QString sec)
{
    int d = deg.toInt ();
    int m = min.toInt ();
    int s = sec.toInt ();
    return setLat (d, m, s);
}

void RiverPoint::setLatDir (QString ns)
{
    if (ns.compare ("S", Qt::CaseInsensitive) == 0)
        if (lat > 0)
            lat = lat * -1.0;
}

double RiverPoint::setWidth (const double wd)
{
    width = wd;
    return width;
}

void RiverPoint::copy(RiverPoint &other)
{
    setLat(other.getLat());
    setLon(other.getLon());
    setWidth (other.width);
    updateText();
}

RiverPoint & RiverPoint::operator=(RiverPoint &other)
{
    copy(other);
    return *this;
}

bool RiverPoint::equals (const RiverPoint rhs)
{
    double prec = .0001;
    bool equal = true;
    if (lon < rhs.lon - prec || lon > rhs.lon + prec)
        equal = false;
    if (lat < rhs.lat - prec || lat > rhs.lat + prec)
        equal = false;

    return equal;
}

bool RiverPoint::operator == (const RiverPoint rhs)
{
    double prec = .0001;
    bool equal = true;
    if (lon < rhs.lon - prec || lon > rhs.lon + prec)
        equal = false;
    if (lat < rhs.lat - prec || lat > rhs.lat + prec)
        equal = false;

    return equal;
}


QString &RiverPoint::getLatLon ()
{
    return text;
/*    QString *txt = new QString("latlon ");
    float absLon = abs(lon);
    float absLat = abs(lat);
    int d, m, s;
    d = static_cast<int>(absLon);
    m = static_cast<int>((absLon - d) * 60.0);
    s = static_cast<int>((absLon - d - (m / 60.0)) * 60.0);
    txt->append (QString::number (d));
    txt->append (' ');
    txt->append (QString::number (m));
    txt->append (' ');
    txt->append (QString::number (s));
    txt->append (' ');
    if (lon > 0)
        txt->append ("E ");
    else
        txt->append ("W ");

    d = static_cast<int>(absLat);
    m = static_cast<int>((absLat - d) * 60.0);
    s = static_cast<int>((absLat - d - (m / 60.0)) * 60.0);
    txt->append (QString::number (d));
    txt->append (' ');
    txt->append (QString::number (m));
    txt->append (' ');
    txt->append (QString::number (s));
    txt->append (' ');
    if (lat > 0)
        txt->append ("N ");
    else
        txt->append ("S ");

    text = *txt;*/
}

void RiverPoint::setLatLon(const QString txt)
{
    QStringList items;
    items = txt.split(' ', QString::SkipEmptyParts);
    setLatLon(items);
}

void RiverPoint::setLatLon (const QStringList items)
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

QString & RiverPoint::updateText()
{
    QString *txt = new QString("");
    double absLon = abs(lon);
    double absLat = abs(lat);
    int d, m, s;

    d = static_cast<int>(absLat);
    m = static_cast<int>((absLat - d) * 60.0);
    s = static_cast<int>((absLat - d - (m / 60.0)) * 60.0);
    txt->append (QString::number (d));
    txt->append (' ');
    txt->append (QString::number (m));
    txt->append (' ');
    txt->append (QString::number (s));
    txt->append (' ');
    if (lat > 0)
        txt->append ("N ");
    else
        txt->append ("S ");

    d = static_cast<int>(absLon);
    m = static_cast<int>((absLon - d) * 60.0);
    s = static_cast<int>((absLon - d - (m / 60.0)) * 60.0);
    txt->append (QString::number (d));
    txt->append (' ');
    txt->append (QString::number (m));
    txt->append (' ');
    txt->append (QString::number (s));
    txt->append (' ');
    if (lon > 0)
        txt->append ("E ");
    else
        txt->append ("W ");

    text = *txt;
    return text;
}

