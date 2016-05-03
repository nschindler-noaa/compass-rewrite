#ifndef C_RIVERPOINT_H
#define C_RIVERPOINT_H

#include <QString>
/*

class RiverPoint
{
public:
    explicit RiverPoint(QObject *parent = 0);
    RiverPoint (const RiverPoint &other);

    const float lat () {return latitude;}
    const float lon () {return longitude;}
    const float width () {return wide;}

    float setLon (int d, int m, int s);
    float setLat (int d, int m, int s);
    float setLon (const float x);
    float setLat (const float x);
    float setWidth (const float w);

private:
    float latitude;
    float longitude;
    float wide;
};
*/
class RiverPoint
{
public:
    RiverPoint ();
    RiverPoint(RiverPoint &other);
    ~RiverPoint ();

    float lat;
    float lon;
    float width;
    float setLon (float ln);
    float setLon (int d, int m, int s);
    float setLon (QString deg, QString min, QString sec);
    void setLonDir (QString ew);
    float getLon () {return lon;}

    float setLat (float lt);
    float setLat (int d, int m, int s);
    float setLat (QString deg, QString min, QString sec);
    void setLatDir (QString ns);
    float getLat () {return lat;}

    float setWidth (float wd);
    float getWidth () {return width;}

    void reset ();

    bool equals (const RiverPoint rhs);
    bool operator == (const RiverPoint rhs);

    void setText (QString txt);
    QString getText();
};



#endif // C_RIVERPOINT_H
