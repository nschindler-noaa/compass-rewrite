#ifndef C_COMPASSCONSOLE_H
#define C_COMPASSCONSOLE_H

#include <QObject>
#include "cmplog.h"
#include "FileManager.h"
#include "ScenarioManager.h"
#include "settings.h"

class CompassConsole : public QObject
{
    Q_OBJECT
public:
    explicit CompassConsole (QObject *parent = nullptr);
    ~CompassConsole ();

signals:
    void done(bool);

public slots:
    void run();

private:
    cmpLog *out;
    FileManager *fManager;
    ScenarioManager *sManager;
    Settings *compassSettings;

};

/** looks for a "-b" (batch mode flag) in the command arguments.
 *  returns true if found, false otherwise (gui mode) */
bool consoleMode (int argc, char *argv[]);

/** Installs a message handler for QtDebugMsg, QtInfoMsg, QtWarningMsg
 * QtCriticalMsg, QtFatalMsg, and QtSystemMsg
 * used by qDebug(), qInfo(), qWarning(), qCritical(), qFatal() - similar to fprint() */
// TODO:
// We'll see if this works.
void  consoleOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);


#endif // C_COMPASSCONSOLE_H
