
#include "cmpconsole.h"
#include "cmpmainwindow.h"
#include "cmpcommandsettings.h"

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
#ifdef QT_DEBUG
//        qInstallMessageHandler(consoleOutput);
#endif
        QCoreApplication a (argc, argv);
        cmpConsole c;
        QObject::connect(&c, SIGNAL(done(int)),
                         &a, SLOT(QCoreApplication::exit(int)), Qt::QueuedConnection);
        retval = c.run(qApp->arguments());
    }
    // else, start gui mode
    else
    {
#ifdef QT_DEBUG
//        qInstallMessageHandler(windowOutput);
#endif
        QApplication a (argc, argv);
        cmpMainWindow w;
        w.show();

        retval = a.exec();
    }

    return retval;
}

