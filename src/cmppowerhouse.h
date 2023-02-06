#ifndef CMPPOWERHOUSE_H
#define CMPPOWERHOUSE_H

#include "Period.h"
#include "cmpfile.h"
#include "cmpdamspecies.h"

#include <QStringList>

/** \class cmpPowerhouse
 * \brief A structure to represent a powerhouse associated with a dam.
 *   This includes the number of the powerhouse, its priority (which can be changed).
 */

class cmpPowerhouse
{
public:
    cmpPowerhouse(int num = 0);
    ~cmpPowerhouse ();

    void allocate (int days, int slices);
    void allocateSpecies (int num);
    void setSpeciesNames (QStringList &spNames);
    void deleteSpecies();
    cmpDamSpecies *getSpecies(QString name);
    cmpDamSpecies *getSpecies(int index);

    void writeData (cmpFile *outfile, int indent, bool outputAll);
    void writeAllData (cmpFile *outfile, int indent);
    void writeSecondData(cmpFile *outfile, int indent, bool outputAll);

    int getPriority() const;
    void setPriority(int value = 0);

    float getThreshold() const;
    void setThreshold(float value);

    float getCapacity() const;
    void setCapacity(float value);

    void setActive (int day, int start, int stop, bool active = true);
    bool getActive (int day, int slice);

    void setFlowFraction (int day, float fraction);
    float getFlowFraction (int day);

    int getNumber() const;
    void setNumber(int value);

    float getRswCapacity() const;
    void setRswCapacity(float newRswCapacity);

private:
    int number;
    int priority;    /**< Ranking of each phouse. The best priority is
                      *   0 followed by increasing positive numbers. */
    float threshold; /**< Min flow controls when this powerhouse turns on */
    float capacity;  /**< Capacity in KCFS of this powerhouse. */
    float rswCapacity;

    Bool2TierList schedule; /**< Powerhouse operation schedule
                      * (in case some powerhouses are only available at
                      * certain times). This is a two-tier period list. */

    QList<cmpDamSpecies *> species;

    /** The threshold and schedule are used at run time to determine
     * the actual percentage flow through this powerhouse, tracked
     * here as flow_fraction */
    QList<float> flowFraction;

    int getSpeciesIndex (QString spec);
};

#endif // CMPPOWERHOUSE_H
