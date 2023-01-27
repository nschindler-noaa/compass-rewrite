#include "cmpusersettings.h"

#include <QDir>
#include <QDate>


cmpUserSettings::cmpUserSettings()
{
    setOptions();
}

void cmpUserSettings::setOptions()
{
    setUserName();
    setServerName();
    setDefaultDir();
    setCurrentDate();
}

const QString &cmpUserSettings::getUserName() const
{
    return userName;
}

void cmpUserSettings::setUserName(const QString &newUserName)
{
    if (newUserName.isEmpty())
    {
        userName = qEnvironmentVariable("user");
    }
    else
    {
        userName = newUserName;
    }
}

const QString &cmpUserSettings::getServerName() const
{
    return serverName;
}

void cmpUserSettings::setServerName(const QString &newServerName)
{
    if (newServerName.isEmpty())
    {
        serverName = qEnvironmentVariable("server");
    }
    else
    {
        serverName = newServerName;
    }
}

const QString &cmpUserSettings::getDefaultDir() const
{
    return defaultDir;
}

void cmpUserSettings::setDefaultDir(const QString &newDefaultDir)
{
    if (newDefaultDir.isEmpty())
    {
        defaultDir = QDir::currentPath();
    }
    else
    {
        defaultDir = newDefaultDir;
    }
}

const QString &cmpUserSettings::getCurrentDate() const
{
    return currDate;
}

void cmpUserSettings::setCurrentDate(const QString &newCurrDate)
{
    if (newCurrDate.isEmpty())
    {
        currDate = QDate::currentDate().toString("yyyy-MM-dd");
    }
    else
    {
        currDate = newCurrDate;
    }
}
