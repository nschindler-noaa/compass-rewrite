#ifndef CMPCONSOLE_H
#define CMPCONSOLE_H

#include "cmpsettings.h"
#include "cmpfile.h"
#include "cmpriversystem.h"

#include <QObject>

class cmpConsole : public QObject
{
    Q_OBJECT
public:
    explicit cmpConsole(QObject *parent = nullptr);
    ~cmpConsole() override;

public slots:
    int run(QStringList args);

signals:
    void done(int retval= 0);

private:
    cmpSettings settings;
    cmpRiverSystem system;
    QList<cmpFile *> files;
    cmpFile * outfile;
};

bool consoleMode(int argc, char *argv[]);

#endif // CMPCONSOLE_H
