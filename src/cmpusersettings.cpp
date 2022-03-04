#include "cmpusersettings.h"

cmpUserSettings::cmpUserSettings()
{

}

const QString &cmpUserSettings::getUserName() const
{
    return userName;
}

void cmpUserSettings::setUserName(const QString &newUserName)
{
    userName = newUserName;
}

const QString &cmpUserSettings::getServerName() const
{
    return serverName;
}

void cmpUserSettings::setServerName(const QString &newServerName)
{
    serverName = newServerName;
}

const QString &cmpUserSettings::getDefaultDir() const
{
    return defaultDir;
}

void cmpUserSettings::setDefaultDir(const QString &newDefaultDir)
{
    defaultDir = newDefaultDir;
}
