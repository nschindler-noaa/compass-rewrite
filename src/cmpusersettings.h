#ifndef CMPUSERSETTINGS_H
#define CMPUSERSETTINGS_H

#include <QStringList>

class cmpUserSettings
{
public:
    cmpUserSettings();

    const QString &getUserName() const;
    void setUserName(const QString &newUserName);

    const QString &getServerName() const;
    void setServerName(const QString &newServerName);

    const QString &getDefaultDir() const;
    void setDefaultDir(const QString &newDefaultDir);

private:
    QString userName;
    QString serverName;
    QString defaultDir;

};

#endif // CMPUSERSETTINGS_H
