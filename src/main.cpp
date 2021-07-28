
#include "CompassConsole.h"
#include "CompassGui.h"
#include "settings.h"
#include "mainwindow.h"

#include <QObject>
#include <QCoreApplication>
#include <QApplication>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int retval = 0;
    // if there is a "-b" in arguments, start console mode
    if (consoleMode(argc, argv))
    {
        qInstallMessageHandler(consoleOutput);
        QCoreApplication a (argc, argv);
        CompassConsole c;
        QObject::connect(&c, SIGNAL(done(bool)),
                         &a, SLOT(quit()), Qt::QueuedConnection);
        c.run();

        retval = a.exec ();
    }
    else  // start gui mode
    {
        qInstallMessageHandler(guiOutput);
        QApplication a (argc, argv);
        CompassGui w;
        w.show();

        retval = a.exec();
    }

    return retval;
}

