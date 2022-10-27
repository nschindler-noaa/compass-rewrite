#ifndef CMPRELEASESITE_H
#define CMPRELEASESITE_H

#include "cmpriverpoint.h"
#include "cmpfile.h"

#include <QObject>

class cmpReach;

/** Release site structure. It contains name, location in lat/lon,
 *  pointer to the site's segment and position along that segment */
class cmpReleaseSite
{
public:
    cmpReleaseSite(QString rsname = QString (""));
    ~cmpReleaseSite ();

    bool parseDesc (cmpFile *cfile);
    bool outputDesc (cmpFile *cfile);

    const QString &getName() const;
    void setName(QString &value);

    cmpRiverPoint *getLatlon() const;
    void setLatlon(cmpRiverPoint *value);

    void setSeg(cmpReach *value);
    cmpReach *getSeg() const;

    float getSeg_mile() const;
    void setSeg_mile(float value);

private:
    QString name;            /**< Name of this site */
    cmpRiverPoint *latlon;       /**< Geographic location */
    cmpReach *seg;               /**< Pointer to the segment */
    float seg_mile;           /**< Miles from bottom of segment */
};

#endif // CMPRELEASESITE_H
