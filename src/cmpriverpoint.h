#ifndef CMPRIVERPOINT_H
#define CMPRIVERPOINT_H

#include <QStringList>

class cmpRiverPoint
{
public:
    cmpRiverPoint(QString line = QString());
    cmpRiverPoint(cmpRiverPoint &other);
    ~cmpRiverPoint ();

    void parse(QString line);
    void parse(QStringList tokens);

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

    void reset ();

    void copy (cmpRiverPoint &other);
    cmpRiverPoint & operator = (cmpRiverPoint &other);
    bool equals (const cmpRiverPoint rhs);
    bool operator == (const cmpRiverPoint rhs);

    void setLatLon (QString txt);
    void setLatLon (QStringList items);
    QString &getLatLon();
    QString &updateText();

    double distance(cmpRiverPoint &other);

private:
    double lat;
    double lon;
    QString text;
};

#endif // CMPRIVERPOINT_H
