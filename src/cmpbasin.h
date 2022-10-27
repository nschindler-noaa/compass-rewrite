#ifndef CMPBASIN_H
#define CMPBASIN_H

#include "definitions.h"

#include <QStringList>

/** \class basin
 * \brief A class that represents a storage basin that can be
 * associated with a dam.
 * volume and flow are stored per day.
 */

class cmpBasin
{
public:
    cmpBasin();
    ~cmpBasin();

    void allocate(int numDays);

    float getFlow(int day) const;
    void setFlow(int day, const float &value);

    float getVolumeMin() const;
    void setVolumeMin(float value);

    float getVolumeMax() const;
    void setVolumeMax(float value);

    float getVolume(int day) const;
    void setVolume(int day, const float &value);

private:
    QList<float>  flow; /** flow from the basin to the associated dam. */
    QList<float>  volume; /**< Per day storage basin volumes */
    float volumeMin;         /**< Minimum allowable volume */
    float volumeMax;         /**< Maximum allowable volume */

};

#endif // CMPBASIN_H
