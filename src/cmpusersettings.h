#ifndef CMPUSERSETTINGS_H
#define CMPUSERSETTINGS_H

#include <QStringList>

class cmpUserSettings
{
public:
    cmpUserSettings();

    void setOptions();

    const QString &getUserName() const;
    void setUserName(const QString &newUserName = QString());

    const QString &getServerName() const;
    void setServerName(const QString &newServerName = QString());

    const QString &getDefaultDir() const;
    void setDefaultDir(const QString &newDefaultDir = QString());

    const QString &getCurrentDate() const;
    void setCurrentDate(const QString &newCurrDate = QString());

private:
    QString userName;
    QString serverName;
    QString defaultDir;
    QString currDate;

};

#endif // CMPUSERSETTINGS_H
