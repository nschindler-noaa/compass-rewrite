#ifndef C_COMPASSCONSOLE_H
#define C_COMPASSCONSOLE_H

#include <QObject>
#include "Log.h"
#include "FileManager.h"
#include "ScenarioManager.h"
#include "settings.h"

class CompassConsole : public QObject
{
    Q_OBJECT
public:
    explicit CompassConsole (QObject *parent = 0);
    ~CompassConsole ();

    void run();
    Log *out;
    FileManager *fManager;
    ScenarioManager *sManager;
    Settings *compassSettings;

signals:
    void done(bool);

public slots:
};

/** looks for a "-b" (batch mode flag) in the command arguments.
 *  returns true if found, false otherwise (gui mode) */
bool consoleMode (int argc, char *argv[]);



#endif // C_COMPASSCONSOLE_H
