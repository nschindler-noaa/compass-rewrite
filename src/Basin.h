#ifndef C_BASIN_H
#define C_BASIN_H

#include <QVector>

#include "definitions.h"

/** \class basin
 * \brief A structure to represent a storage basin associated with a dam.
 */
class Basin
{
public:
    Basin();
    ~Basin();

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

#endif // C_BASIN_H
