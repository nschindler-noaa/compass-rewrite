#ifndef DAMSTRUCTURES_H
#define DAMSTRUCTURES_H

#include <QString>

class Spillway
{
public:
    Spillway();

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

enum FishwayType {
    Ladder,
    Bypass,
    Lift,
    Trans,
    NumTypes
};

class Fishway
{
public:
    Fishway();

    FishwayType getType() const;
    bool setType(QString &value);
    void setType(const FishwayType &value);
    QString &getTypeString();

    float getLength() const;
    void setLength(float value);

    float getCapacity() const;
    void setCapacity(float value);

    float getVelocity() const;
    void setVelocity(float value);

private:
    FishwayType type;
    float length;
    float capacity;
    float velocity;

};

#endif // DAMSTRUCTURES_H
