#ifndef CMPSETTINGS_H
#define CMPSETTINGS_H

#include "cmpcommandsettings.h"
#include "cmpdatasettings.h"
#include "cmpusersettings.h"

#include "version.h"

#include <iostream>
#define DEBUG_

class cmpSettings
{
public:
    cmpSettings();
    cmpSettings(QStringList args);
    ~cmpSettings();
    void initial();

    QString getAppOrganization();
    QString getAppVersion();
    QString getDataFileVersion();

    cmpCommandSettings * getCommandSettings();
    void parseCommandArguments (QStringList args);

    cmpDataSettings * getDataSettings();

    cmpUserSettings * getUserSettings();
    void setUserSettings();


private:
    QString organization;
    QString version;
    QString datafileVersion;

    cmpCommandSettings *command;
    cmpDataSettings *data;
    cmpUserSettings *user;

};

#endif // CMPSETTINGS_H
