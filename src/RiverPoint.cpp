#include "RiverPoint.h"

#include <QStringList>

RiverPoint::RiverPoint()
{
    reset ();
}
RiverPoint::RiverPoint(RiverPoint &other)
{
    QString txt (other.getText());
    setLon (other.lon);
    setLat (other.lat);
    setWidth (other.width);
    setText (txt);
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
}

float RiverPoint::setLon (const float x)
{
    lon = x;
    return lon;
}

float RiverPoint::setLon (int d, int m, int s)
{
    lon = (double)d + (((double)m + ((double)s / 60.0)) / 60.0);
    return lon;
}

float RiverPoint::setLon (QString deg, QString min, QString sec)
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


float RiverPoint::setLat (const float x)
{
    lat = x;
    return lat;
}

float RiverPoint::setLat (int d, int m, int s)
{
    lat = (float) s / 60.0;
    lat += (float) m / 60.0;
    lat += (float) d;
//    lat = (double)d + (((double)m + ((double)s / 60.0)) / 60.0);
    return lat;
}

float RiverPoint::setLat (QString deg, QString min, QString sec)
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

float RiverPoint::setWidth (const float wd)
{
    width = wd;
    return width;
}

bool RiverPoint::equals (const RiverPoint rhs)
{
    float prec = .0001;
    bool equal = true;
    if (lon < rhs.lon - prec || lon > rhs.lon + prec)
        equal = false;
    if (lat < rhs.lat - prec || lat > rhs.lat + prec)
        equal = false;

    return equal;
}

bool RiverPoint::operator == (const RiverPoint rhs)
{
    float prec = .0001;
    bool equal = true;
    if (lon < rhs.lon - prec || lon > rhs.lon + prec)
        equal = false;
    if (lat < rhs.lat - prec || lat > rhs.lat + prec)
        equal = false;

    return equal;
}


QString RiverPoint::getText ()
{
    QString *txt = new QString ("latlon ");
    int d, m, s;
    d = (int) (lon);
    m = (int) ((lon - d) * 60.0);
    s = (int) ((lon - d - (m / 60.0)) * 60.0);
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

    d = (int) (lat);
    m = (int) ((lat - d) * 60.0);
    s = (int) ((lat - d - (m / 60.0)) * 60.0);
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

    return *txt;
}
void RiverPoint::setText(const QString txt)
{
    QStringList items;
    items = txt.split(' ', QString::SkipEmptyParts);
    if (items.count() == 8)
    {
        setLon (items.at(0), items.at(1), items.at(2));
        setLonDir(items.at(3));
        setLat (items.at(4), items.at(5), items.at(6));
        setLatDir(items.at(7));
    }
}

/*
RiverPoint *newPoint ()
{
    RiverPoint *p = new RiverPoint;// *) malloc (sizeof (RiverPoint));
    p->lat = 0.0;
    p->lon = 0.0;
    p->width = 0.0;
    return p;
}

void deletePoint (RiverPoint *&rpt)
{
    rpt->lat = 0.0;
    rpt->lon = 0.0;
    rpt->width = 0.0;
    delete (rpt);
    rpt = NULL;
}*/

