#ifndef CMPFISHWAY_H
#define CMPFISHWAY_H

#include "cmpfile.h"

#include <QStringList>

/** \class cmpFishway
 * \brief A structure to represent a fishway associated with a dam.
 *   This can be in the form of a fish ladder, bypass, life, or transportation.
 */

class cmpFishway
{
public:
    enum Type {
        Ladder,
        Bypass,
        Lift,
        Trans,
        NumTypes
    };

    cmpFishway();

    bool parseDesc(cmpFile *descfile);
    void writeDesc(cmpFile *outfile);

    Type getType() const;
    bool setType(QString &value);
    void setType(const Type &value);
    QString &getTypeString();

    float getLength() const;
    void setLength(float value);

    float getCapacity() const;
    void setCapacity(float value);

    float getVelocity() const;
    void setVelocity(float value);

private:
    Type type;
    float length;
    float capacity;
    float velocity;

};

#endif // CMPFISHWAY_H
