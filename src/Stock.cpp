#include "Stock.h"

Stock::Stock(QString &nm)
{
    setName(nm);

    inriverReturnEqn = nullptr;
    transportReturnEqn = nullptr;

    allocate (1);
}

void Stock::allocate(int num)
{
    QList<float> stepFact;
    for (int i = 0; i < STEPS_IN_SEASON; i++)
        stepFact.append(0);

    while (migrationEqn.count() < num)
    {
        migrationEqn.append(nullptr);
        customSurvivalEqn.append(nullptr);
        predTempResponseEqn.append(nullptr);
        mvcoef.append(0);
        distanceCoeff.append(0);
        timeCoeff.append(0);
        migrB1Factor.append(stepFact);
        vvar.append(0);
        sigmaD.append(0);
        reachSurvivalCoef.append(0);

    }
}

Stock::~Stock ()
{
    delete name;
    deleteAll ();
}

void Stock::deleteAll()
{
    Equation *eptr = nullptr;

    while (migrationEqn.count() > 0)
    {
        eptr = migrationEqn.takeFirst();
        if (eptr != nullptr)
            delete eptr;
        eptr = customSurvivalEqn.takeFirst();
        if (eptr != nullptr)
            delete eptr;
        eptr = predTempResponseEqn.takeFirst();
        if (eptr != nullptr)
            delete eptr;
    }
    delete inriverReturnEqn;
    delete transportReturnEqn;
    mvcoef.clear();
    distanceCoeff.clear();
    timeCoeff.clear();
    migrB1Factor.clear();
    vvar.clear();
    sigmaD.clear();
    reachSurvivalCoef.clear();
}

QString &Stock::getName() const
{
    return *name;
}

void Stock::setName(QString &value)
{
    if (value.isEmpty ())
        name = new QString ("Generic");
    else
        name = new QString(value);
}

Equation * Stock::getMigrationEqn(int index) const
{
    return migrationEqn[index];
}

void Stock::setMigrationEqn(int index, Equation * &value)
{
    migrationEqn[index] = value;
}

float Stock::getMvcoef(int index) const
{
    return mvcoef[index];
}

void Stock::setMvcoef(int index, const float &value)
{
    mvcoef[index] = value;
}

float Stock::getDistanceCoeff(int index) const
{
    return distanceCoeff[index];
}

void Stock::setDistanceCoeff(int index, const float &value)
{
    distanceCoeff[index] = value;
}

float Stock::getTimeCoeff(int index) const
{
    return timeCoeff[index];
}

void Stock::setTimeCoeff(int index, const float &value)
{
    timeCoeff[index] = value;
}

float Stock::getSigmaD(int index) const
{
    return sigmaD[index];
}

void Stock::setSigmaD(int index, const float &value)
{
    sigmaD[index] = value;
}

float Stock::getMigrB1Factor(int index, int step) const
{
    return migrB1Factor[index][step];
}

void Stock::setMigrB1Factor(int index, int step, const float &value)
{
    migrB1Factor[index][step] = value;
}

float Stock::getVvar(int index) const
{
    return vvar[index];
}

void Stock::setVvar(int index, const float &value)
{
    vvar[index] = value;
}

Equation * Stock::getCustomSurvivalEqn(int index) const
{
    return customSurvivalEqn[index];
}

void Stock::setCustomSurvivalEqn(int index, Equation * value)
{
    customSurvivalEqn[index] = value;
}

Equation * Stock::getPredTempResponseEqn(int index) const
{
    return predTempResponseEqn[index];
}

void Stock::setPredTempResponseEqn(int index, Equation *value)
{
    predTempResponseEqn[index] = value;
}

Equation * Stock::getInriver_return_eqn() const
{
    return inriverReturnEqn;
}

void Stock::setInriver_return_eqn(Equation *value)
{
    inriverReturnEqn = value;
}

Equation *Stock::getTransportReturnEqn() const
{
    return transportReturnEqn;
}

void Stock::setTransportReturnEqn(Equation *value)
{
    transportReturnEqn = value;
}

float Stock::getReachSurvivalCoef(int index) const
{
    return reachSurvivalCoef[index];
}

void Stock::setReachSurvivalCoef(int index, const float &value)
{
    reachSurvivalCoef[index] = value;
}
