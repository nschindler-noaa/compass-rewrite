#ifndef C_RIVERPOINT_H
#define C_RIVERPOINT_H

#include <QStringList>


class RiverPoint
{
public:
    RiverPoint ();
    RiverPoint(RiverPoint &other);
    ~RiverPoint ();

    double setLon (double ln);
    double setLon (int d, int m, int s);
    double setLon (QString deg, QString min, QString sec);
    void setLonDir (QString ew);
    double getLon () {return lon;}

    double setLat (double lt);
    double setLat (int d, int m, int s);
    double setLat (QString deg, QString min, QString sec);
    void setLatDir (QString ns);
    double getLat () {return lat;}

    double setWidth (const double wd);
    double getWidth () {return width;}

    void reset ();

    void copy (RiverPoint &other);
    RiverPoint & operator = (RiverPoint &other);
    bool equals (const RiverPoint rhs);
    bool operator == (const RiverPoint rhs);

    void setLatLon (QString txt);
    void setLatLon (QStringList items);
    QString &getLatLon();
    QString &updateText();

private:
    double lat;
    double lon;
    double width;
    QString text;
};



#endif // C_RIVERPOINT_H
