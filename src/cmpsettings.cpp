#include "cmpsettings.h"

cmpSettings::cmpSettings()
{
    initial();
}

cmpSettings::cmpSettings(QStringList args)
{
    initial();
    parseCommandArguments(args);
}

void cmpSettings::initial()
{
    organization = QString("NOAA/NMFS/NWFSC");
    version = QString("3.0.1");
    datafileVersion = QString("15");

    command = new cmpCommandSettings();

    user = new cmpUserSettings();
    user->setOptions();

    data = new cmpDataSettings();
}

cmpSettings::~cmpSettings()
{
    delete data;
    delete user;
    delete command;
}

QString cmpSettings::getAppOrganization()
{
    return organization;
}

QString cmpSettings::getAppVersion()
{
    return version;
}

QString cmpSettings::getDataFileVersion()
{
    return datafileVersion;
}

cmpCommandSettings * cmpSettings::getCommandSettings()
{
    return command;
}

void cmpSettings::parseCommandArguments (QStringList args)
{
    command->parseArguments(args);
}

cmpDataSettings * cmpSettings::getDataSettings()
{
    return data;
}

cmpUserSettings * cmpSettings::getUserSettings()
{
    return user;
}

void cmpSettings::setUserSettings()
{
    user->setOptions();
}
