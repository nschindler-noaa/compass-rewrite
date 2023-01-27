#ifndef CMPBASIN_H
#define CMPBASIN_H

//#include "definitions.h"
//#include "cmpfile.h"

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

    bool parseDesc(QString text);
    void setText(QString text = QString());
    QString &getText();

    void allocate(int numDays);

    float getFlowOnDay(int day) const;
    void setFlowOnDay(int day, const float &value);

    float getVolumeMin() const;
    void setVolumeMin(float value);

    float getVolumeMax() const;
    void setVolumeMax(float value);

    float getVolumeOnDay(int day) const;
    void setVolumeOnDay(int day, const float &value);

    QList<float> &getFlow();
    void setFlow(const QList<float> &newFlow);

    QList<float> &getVolume();
    void setVolume(const QList<float> &newVolume);

private:
    QList<float>  flow; /** flow from the basin to the associated dam. */
    QList<float>  volume; /**< Per day storage basin volumes */
    float volumeMin;         /**< Minimum allowable volume */
    float volumeMax;         /**< Maximum allowable volume */
    QString valueStr;
};

#endif // CMPBASIN_H
