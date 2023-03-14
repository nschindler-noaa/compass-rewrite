#include "cmpstock.h"

#include "cmpdatasettings.h"

cmpStock::cmpStock() : cmpSpecies()
{}

bool cmpStock::parseData(cmpFile *cfile)
{
    bool okay = true, end = false;
    QString tmpStr;
    QString token ("");
    QStringList tokens;

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Stock data.");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->checkEnd("stock", name);
            end = true;
        }
        else
        {
            okay = parseToken(token, cfile);
        }
    }
    return okay;
}

void cmpStock::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    int indent2 = indent + 1;
    outfile->writeString(indent, "stock", name);
    outfile->writeString(indent2, "species", speciesName);
    writeFishReturnEqns(outfile, indent2, outputAll);
    outfile->writeNewline();
    writeReachClassData(outfile, indent2, outputAll);
    outfile->writeEnd(indent, "stock", name);
}

void cmpStock::writeReachClassData(cmpFile *outfile, int indent, bool outputAll)
{
    int indent2 = indent+1;
    int total = reachClasses.count();
    QString rcName;
    cmpEquation *eqn;
    for (int i = 0; i < total; i++)
    {
        rcName = reachClasses.at(i)->getName();
        outfile->writeTitledValue(indent, "v_var", rcName, getVvar(i), Data::Scientific, (outputAll? 100000: 100.0));
        outfile->writeTitledValue(indent, "migr_var_coef", rcName, getMigrVarCoef(i), Data::Scientific, (outputAll? 100000: 1.0));
        outfile->writeTitledValue(indent, "time_coef", rcName, getTimeCoeff(i), Data::Scientific, (outputAll? 100000: 1.0));
        outfile->writeTitledValue(indent, "distance_coef", rcName, getDistCoeff(i), Data::Scientific, (outputAll? 100000: 0.0));
        outfile->writeTitledValue(indent, "sigma_d", rcName, getSigmaD(i), Data::Scientific, (outputAll? 100000: 0.0));
        outfile->writeTitledValue(indent, "reach_survival_coef", rcName, getReachSurvivalCoef(i), Data::Scientific, (outputAll? 100000: 1.0));
        eqn = reachClasses[i]->getMigrationEqn();   //     migration_eqn Class_0 0
        outfile->writeTitledValue(indent, "migration_eqn", rcName, eqn->getId());// QString::number(eqn->getId()));
        eqn->writeParameters(outfile, indent2, outputAll);
        outfile->writeEnd(indent, "migration_eqn", rcName);
        outfile->writeNewline();
        eqn = reachClasses[i]->getCustomSurvivalEqn(); //        custom_survival_eqn Class_0 57
        outfile->writeString(indent, "custom_survival_eqn", rcName, QString::number(eqn->getId()));
        eqn->writeParameters(outfile, indent2, outputAll);
        outfile->writeEnd(indent, "custom_survival_eqn", rcName);
        outfile->writeNewline();
    }
}

const QString &cmpStock::getSpeciesName() const
{
    return speciesName;
}

void cmpStock::setSpeciesName(const QString &newSpeciesName)
{
    speciesName = newSpeciesName;
}

