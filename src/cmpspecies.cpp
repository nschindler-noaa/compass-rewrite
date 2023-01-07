#include "cmpspecies.h"

extern QStringList equationNames;

cmpSpecies::cmpSpecies()
{
    name = QString();
    setDefaults();
}

cmpSpecies::cmpSpecies(const cmpSpecies &rhs)
{
    name = QString();
    copy(rhs);
}

cmpSpecies::~cmpSpecies()
{
    for (int i = 0, total = reachClasses.count(); i < total; i++)
    {
        delete reachClasses[i];
    }
    reachClasses.clear();

}

void cmpSpecies::copy(const cmpSpecies &rhs)
{
    name = rhs.getName();

    setReachClassNames (rhs.getReachClassNames());
    for (int i = 0, total = reachClasses.count(); i < total; i++)
        reachClasses[i]->copy(*rhs.getReachClass(i));
    tailracePredCoef = rhs.getTailracePredCoef();
    forebayPredCoef = rhs.getForebayPredCoef();
    gasMortEqn->copy(*rhs.getGasmortEqn());
    fishDepthEqn->copy(*rhs.getFishdensEqn());
    // return (ocean survival)
    inriverLatentMort = rhs.getInriverLatentMort();
    transportLatentMort = rhs.getTransportLatentMort();
    differentialReturn = rhs.getDifferentialReturn();

    inriverReturnEqn->copy(*rhs.getInriverReturnEqn());
    transportReturnEqn->copy(*rhs.getTransportReturnEqn());

    inriverLatentMortEqn->copy(*rhs.getInriverLatentMortEqn());
}

const QString &cmpSpecies::getName() const
{
    return name;
}

void cmpSpecies::setName(const QString &newName)
{
    name = newName;
}

void cmpSpecies::allocateClasses(int numReachClasses)
{
    while (reachClasses.count() < numReachClasses)
    {
        reachClasses.append(new cmpReachClass);
    }
    while (reachClasses.count() > numReachClasses)
    {
        cmpReachClass *rc = reachClasses.takeLast();
        delete rc;
    }
}

void cmpSpecies::allocate(int steps, int numReachClasses)
{
    for (int i = 0; i < numReachClasses; i++)
        reachClasses.at(i)->allocate(steps);
}

void cmpSpecies::setDefaults()
{
    allocateClasses(1);
    reachClasses[0]->setDefaults();
    reachClasses[0]->allocate(732);

    tailracePredCoef = 0;
    forebayPredCoef = 0;

    gasMortEqn = new cmpEquation(EQ_GMORT3);
    fishDepthEqn = new cmpEquation(EQ_FDENS);

    inriverLatentMort = 0;
    transportLatentMort = 0;
    differentialReturn = 1;
    inriverLatentMortEqn = new cmpEquation(EQ_LATENT_MORT_WTT);
    inriverReturnEqn = new cmpEquation(51);
    transportReturnEqn = new cmpEquation(51);
}

const QStringList &cmpSpecies::getReachClassNames() const
{
    return reachClassNames;
}

void cmpSpecies::setReachClassNames(const QStringList &newReachClassNames)
{
    reachClassNames = newReachClassNames;
    int total = reachClassNames.count();
    while (reachClasses.count() < total)
        reachClasses.append(new cmpReachClass());
    while (reachClasses.count() > total)
    {
        cmpReachClass *crc = reachClasses.takeLast();
        delete crc;
    }
    for (int i = 0; i < total; i++)
    {
        reachClasses[i]->setName(reachClassNames[i]);
        reachClasses[i]->setDefaults();
    }
}

const QString &cmpSpecies::getReachClassName (int rc) const
{
    return reachClassNames.at(rc);
}

void cmpSpecies::setReachClassName(int rc, QString &newName)
{
    if (rc < reachClassNames.count())
        reachClassNames[rc] = newName;
}

const cmpReachClass *cmpSpecies::getReachClass(int rc) const
{
    return reachClasses.at(rc);
}

void cmpSpecies::setReachClass(int rc, cmpReachClass *newReachClass)
{
    if (reachClasses.at(rc) != nullptr)
        delete reachClasses[rc];
    reachClasses[rc] = newReachClass;
}

bool cmpSpecies::parseData(cmpFile *cfile)
{
    bool okay = true, end = false;
    int tmpInt = 0, rc = 0;
    float tmpFloat = 0;
    QString tmpStr;
    QString token ("");
    QStringList tokens;

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Species data.");
            okay = false;
        }
        // migration
        else if (token.compare("v_var", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setVvar(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("migr_var_coef", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setMigrVarCoef(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("distance_coef", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setDistCoeff(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("time_coef", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setTimeCoeff(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("migration_eqn", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            tokens = tmpStr.split(' ', QString::SkipEmptyParts);
            rc = reachClassNames.indexOf(tokens[0]);
            if (rc > -1)
            {
                cmpEquation *neweqn = new cmpEquation(tokens[1]);
                okay = neweqn->parseData(cfile, "migration_eqn");
                reachClasses[rc]->setMigrationEqn(neweqn);
            }
            else
            {
                cfile->printError("problem with migration eqn for reach class");
                reachClasses[rc]->setMigrationEqn(nullptr);
                cfile->skipToEnd();
            }
        }
        else if (token.compare("reach_survival_coef", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setReachSurvivalCoef(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("reach_pred_coef", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setReachPredCoef(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("sigma_d", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setSigmaD(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("reach_pred_coef", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setReachPredCoef(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("custom_survival_eqn", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            tokens = tmpStr.split(' ', QString::SkipEmptyParts);
            rc = reachClassNames.indexOf(tokens[0]);
            if (rc > -1)
            {
                cmpEquation *neweqn = new cmpEquation(tokens[1]);
                okay = neweqn->parseData(cfile, "custom_survival_eqn");
                reachClasses[rc]->setCustomSurvivalEqn(neweqn);
            }
            else
            {
                cfile->printError("problem with custom survival eqn for reach class");
                reachClasses[rc]->setCustomSurvivalEqn(nullptr);
                cfile->skipToEnd();
            }
        }
        else if (token.compare("pprime_a", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setPprimeA(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("pprime_b", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            okay |= readReachClassValue(tmpStr, rc, tmpFloat);
            if (okay)
                reachClasses[rc]->setPprimeB(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("forebay_pred_coef", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readFloatOrNa(tmpStr, tmpFloat);
            if (okay)
                setForebayPredCoef(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("tailrace_pred_coef", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readFloatOrNa(tmpStr, tmpFloat);
            if (okay)
                setTailracePredCoef(tmpFloat);
            else
                cfile->skipLine();
        }
        else if (token.compare("gas_mort_eqn", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            if (okay)
            {
                cmpEquation *neweqn = new cmpEquation(tmpStr);
                okay = neweqn->parseData(cfile, "gas_mort_eqn");
                if (okay)
                    setGasmortEqn(neweqn);
                else
                    delete neweqn;
            }
        }
        else if (token.compare("fish_depth_eqn", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            if (okay)
            {
                cmpEquation *neweqn = new cmpEquation(tmpStr);
                okay = neweqn->parseData(cfile, "fish_depth_eqn");
                if (okay)
                    setFishdensEqn(neweqn);
                else
                    delete neweqn;
            }
        }
        else if (token.compare("inriver_return_eqn", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            if (okay)
            {
                cmpEquation *neweqn = new cmpEquation(tmpStr);
                okay = neweqn->parseData(cfile, "inriver_return_eqn");
                if (okay)
                    setInriverReturnEqn(neweqn);
                else
                    delete neweqn;
            }
        }
        else if (token.compare("transport_return_eqn", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->readString(tmpStr);
            if (okay)
            {
                cmpEquation *neweqn = new cmpEquation(tmpStr);
                okay = neweqn->parseData(cfile, "transport_return_eqn");
                if (okay)
                    setTransportReturnEqn(neweqn);
                else
                    delete neweqn;
            }
        }

        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->checkEnd("species", name);
            end = true;
        }
        else
        {
            cfile->unknownToken(token, name);
        }
    }

    return okay;
}

bool cmpSpecies::readReachClassValue(QString &newString, int &rc, float &value)
{
    bool okay = true;
    QStringList tokens(newString.split(' ', QString::SkipEmptyParts));
    if (tokens.count() != 2)
    {
        okay = false;
    }
    else
    {
        rc = tokens[0].toInt(&okay);
        if (!okay)
        {
            rc = reachClassNames.indexOf(tokens.at(0));
        }
        value = tokens[1].toFloat(&okay);
    }
    return okay;
}

void cmpSpecies::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    outfile->writeString(indent, "species", name);
    writeReachClassData(outfile, indent+1, outputAll);
    writeSpeciesData(outfile, indent+1, outputAll);
    outfile->writeNewline();
    outfile->writeEnd(indent, "species", name);
}

void cmpSpecies::writeReachClassData(cmpFile *outfile, int indent, bool outputAll)
{
    int total = reachClasses.count();
    QString rcName;
    cmpEquation *eqn;
    for (int i = 0; i < total; i++)
    {
        rcName = reachClassNames.at(i);
        outfile->writeTitledValue(indent, "reach_pred_coef", rcName, getReachPredCoef(i), (outputAll? 100000: 0.0));
        outfile->writeTitledValue(indent, "pprime_a", rcName, getPprimeA(i), (outputAll? 100000: 0.0));
        outfile->writeTitledValue(indent, "pprime_b", rcName, getPprimeB(i), (outputAll? 100000: 0.0));
        outfile->writeTitledValue(indent, "v_var", rcName, getVvar(i), (outputAll? 100000: 100.0));
        outfile->writeTitledValue(indent, "migr_var_coef", rcName,getMigrVarCoef(i), (outputAll? 100000: 1.0));
        outfile->writeTitledValue(indent, "time_coef", rcName, getTimeCoeff(i), (outputAll? 100000: 1.0));
        outfile->writeTitledValue(indent, "distance_coef", rcName, getDistCoeff(i), (outputAll? 100000: 0.0));
        outfile->writeTitledValue(indent, "sigma_d", rcName, getSigmaD(i), (outputAll? 100000: 0.0));
        outfile->writeTitledValue(indent, "reach_survival_coef", rcName, getReachSurvivalCoef(i), (outputAll? 100000: 1.0));
        eqn = reachClasses[i]->getMigrationEqn();   //     migration_eqn Class_0 0
        outfile->writeString(indent, "migration_eqn", rcName, QString::number(eqn->getId()));
        eqn->writeParameters(outfile, indent, outputAll);
        outfile->writeEnd(indent, "migration_eqn", rcName);
        eqn = reachClasses[i]->getCustomSurvivalEqn(); //        custom_survival_eqn Class_0 57
        outfile->writeString(indent, "custom_survival_eqn", rcName, QString::number(eqn->getId()));
        eqn->writeParameters(outfile, indent, outputAll);
        outfile->writeEnd(indent, "custom_survival_eqn", rcName);

        outfile->writeNewline();
    }
}

void cmpSpecies::writeSpeciesData(cmpFile *outfile, int indent, bool outputAll)
{
    float dval = 0;
    cmpEquation *eqn;
    if (outputAll)
        dval = 1000000;
    outfile->writeValue(indent, "forebay_pred_coef", getForebayPredCoef(), dval);
    outfile->writeValue(indent, "tailrace_pred_coef", getTailracePredCoef(), dval);
    outfile->writeNewline();
    eqn = getFishdensEqn();
    outfile->writeValue(indent, "fish_depth_eqn", eqn->getId());
    eqn->writeParameters(outfile, indent, outputAll);
    outfile->writeEnd(indent, "fish_depth_eqn");
    outfile->writeNewline();
    eqn = getGasmortEqn();
    outfile->writeValue(indent, "gas_mort_eqn", eqn->getId());
    eqn->writeParameters(outfile, indent, outputAll);
    outfile->writeEnd(indent, "gas_mort_eqn");
    outfile->writeNewline();
    writeFishReturnEqns(outfile, indent, outputAll);
}

void cmpSpecies::writeFishReturnEqns(cmpFile *outfile, int indent, bool outputAll)
{
    cmpEquation *eqn;
    eqn = getInriverReturnEqn();
    outfile->writeValue(0, "inriver_return_eqn", eqn->getId());
    eqn->writeParameters(outfile, indent, outputAll);
    outfile->writeEnd(0, "inriver_return_eqn");
    outfile->writeNewline();
    eqn = getTransportReturnEqn();
    outfile->writeValue(0, "transport_return_eqn", eqn->getId());
    eqn->writeParameters(outfile, indent, outputAll);
    outfile->writeEnd(0, "transport_return_eqn");
}

const cmpEquation *cmpSpecies::getMigrationEqn(int rc) const
{
    return reachClasses[rc]->getMigrationEqn();
}

void cmpSpecies::setMigrationEqn(int rc, cmpEquation *newMigrationEqn)
{
    reachClasses[rc]->setMigrationEqn(newMigrationEqn);
}

float cmpSpecies::getMigrVarCoef(int rc) const
{
    return reachClasses.at(rc)->getMigrVarCoef();
}

void cmpSpecies::setMigrVarCoef(int rc, const float newMvCoef)
{
    reachClasses.at(rc)->setMigrVarCoef(newMvCoef);
}

float cmpSpecies::getDistCoeff(int rc) const
{
    return reachClasses.at(rc)->getDistCoeff();
}

void cmpSpecies::setDistCoeff(int rc, float newDistanceCoeff)
{
    reachClasses.at(rc)->setDistCoeff(newDistanceCoeff);
}

float cmpSpecies::getTimeCoeff(int rc) const
{
    return reachClasses.at(rc)->getTimeCoeff();
}

void cmpSpecies::setTimeCoeff(int rc, float newTimeCoeff)
{
    reachClasses.at(rc)->setTimeCoeff(newTimeCoeff);
}

float cmpSpecies::getSigmaD(int rc) const
{
    return reachClasses.at(rc)->getSigmaD();
}

void cmpSpecies::setSigmaD(int rc, float newSigmaD)
{
    reachClasses.at(rc)->setSigmaD(newSigmaD);
}

float cmpSpecies::getProcStdDev(int rc) const
{
    return reachClasses.at(rc)->getProcStdDev();
}

void cmpSpecies::setProcStdDev(int rc, float newProcStdDev)
{
    reachClasses.at(rc)->setProcStdDev(newProcStdDev);
}

float cmpSpecies::getMigrB1Factor(int rc, int step) const
{
    return reachClasses.at(rc)->getMigrB1Factor(step);
}

void cmpSpecies::setMigrB1Factor(int rc, int step, float newMigrB1Factor)
{
    reachClasses.at(rc)->setMigrB1Factor(step, newMigrB1Factor);
}

float cmpSpecies::getVvar(int rc) const
{
    return reachClasses.at(rc)->getVvar();
}

void cmpSpecies::setVvar(int rc, float newVvar)
{
    reachClasses.at(rc)->setVvar(newVvar);
}

cmpEquation *cmpSpecies::getCustomSurvivalEqn(int rc) const
{
    return reachClasses.at(rc)->getCustomSurvivalEqn();
}

void cmpSpecies::setCustomSurvivalEqn(int rc, cmpEquation *newCustomSurvivalEqn)
{
    reachClasses.at(rc)->setCustomSurvivalEqn(newCustomSurvivalEqn);
}

float cmpSpecies::getReachSurvivalCoef(int rc) const
{
    return reachClasses.at(rc)->getReachSurvivalCoef();
}

void cmpSpecies::setReachSurvivalCoef(int rc, float newReachSurvivalCoef)
{
   reachClasses.at(rc)->setReachSurvivalCoef(newReachSurvivalCoef);
}

const cmpMonteCarloMulti *cmpSpecies::getCovmat(int rc) const
{
    return reachClasses.at(rc)->getCovmat();
}

void cmpSpecies::setCovmat(int rc, cmpMonteCarloMulti *newCovmat)
{
    reachClasses.at(rc)->setCovmat(newCovmat);
}

float cmpSpecies::getReachPredCoef(int rc) const
{
    return reachClasses.at(rc)->getReachPredCoef();
}

void cmpSpecies::setReachPredCoef(int rc, float newReachPredCoef)
{
    reachClasses[rc]->setReachPredCoef(newReachPredCoef);
}

float cmpSpecies::getPprimeA(int rc) const
{
    return reachClasses.at(rc)->getPprimeA();
}

void cmpSpecies::setPprimeA(int rc, float newPprimeA)
{
    reachClasses[rc]->setPprimeA(newPprimeA);
}


float cmpSpecies::getPprimeB(int rc) const
{
    return reachClasses.at(rc)->getPprimeB();
}

void cmpSpecies::setPprimeB(int rc, float newPprimeB)
{
    reachClasses[rc]->setPprimeB(newPprimeB);
}

float cmpSpecies::getTailracePredCoef() const
{
    return tailracePredCoef;
}

void cmpSpecies::setTailracePredCoef(float newTailracePredCoef)
{
    tailracePredCoef = newTailracePredCoef;
}

float cmpSpecies::getForebayPredCoef() const
{
    return forebayPredCoef;
}

void cmpSpecies::setForebayPredCoef(float newForebayPredCoef)
{
    forebayPredCoef = newForebayPredCoef;
}

cmpEquation *cmpSpecies::getGasmortEqn() const
{
    return gasMortEqn;
}

void cmpSpecies::setGasmortEqn(cmpEquation *newGasmortEqn)
{
    gasMortEqn = newGasmortEqn;
}

cmpEquation *cmpSpecies::getFishdensEqn() const
{
    return fishDepthEqn;
}

void cmpSpecies::setFishdensEqn(cmpEquation *newFishdensEqn)
{
    fishDepthEqn = newFishdensEqn;
}

float cmpSpecies::getInriverLatentMort() const
{
    return inriverLatentMort;
}

void cmpSpecies::setInriverLatentMort(float newInriverLatentMort)
{
    inriverLatentMort = newInriverLatentMort;
}

float cmpSpecies::getTransportLatentMort() const
{
    return transportLatentMort;
}

void cmpSpecies::setTransportLatentMort(float newTransportLatentMort)
{
    transportLatentMort = newTransportLatentMort;
}

float cmpSpecies::getDifferentialReturn() const
{
    return differentialReturn;
}

void cmpSpecies::setDifferentialReturn(float newDifferentialReturn)
{
    differentialReturn = newDifferentialReturn;
}

cmpEquation *cmpSpecies::getInriverReturnEqn() const
{
    return inriverReturnEqn;
}

void cmpSpecies::setInriverReturnEqn(cmpEquation *newInriverReturnEqn)
{
    inriverReturnEqn = newInriverReturnEqn;
}

cmpEquation *cmpSpecies::getTransportReturnEqn() const
{
    return transportReturnEqn;
}

void cmpSpecies::setTransportReturnEqn(cmpEquation *newTransportReturnEqn)
{
    transportReturnEqn = newTransportReturnEqn;
}

cmpEquation *cmpSpecies::getInriverLatentMortEqn() const
{
    return inriverLatentMortEqn;
}

void cmpSpecies::setInriverLatentMortEqn(cmpEquation *newInriverLatentMortEqn)
{
    inriverLatentMortEqn = newInriverLatentMortEqn;
}


