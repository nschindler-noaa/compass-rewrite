#ifndef CMPSPILLWAY_H
#define CMPSPILLWAY_H

#include <QStringList>

/** \class cmpSpillway
 * \brief A structure to represent a spillway associated with a dam.
 *   This has a width and a number of gates.
 */


class cmpSpillway
{
public:
    cmpSpillway();

    float getWidth() const;
    void setWidth(float value);

    int getNumGates() const;
    void setNumGates(int value);

    float getPerGate() const;
    void setPerGate(float value);

    float getGateWidth() const;
    void setGateWidth(float value);

private:
    float width;
    int numGates;
    float perGate;
    float gateWidth;
};

#endif // CMPSPILLWAY_H
