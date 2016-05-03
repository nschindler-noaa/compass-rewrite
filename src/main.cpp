#include <QObject>
#include <QCoreApplication>
#include <QApplication>

#include "CompassConsole.h"
#include "CompassGui.h"
#include "settings.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    int retval = 0;
    // if there is a "-b" in arguments, start console mode
    if (consoleMode(argc, argv))
    {
        QCoreApplication a (argc, argv);
        CompassConsole c;
        QObject::connect(&c, SIGNAL(done(bool)),
                         &a, SLOT(quit()), Qt::QueuedConnection);
        c.run();

        retval = a.exec ();
    }
    else  // start gui mode
    {
        QApplication a (argc, argv);
        CompassGui w;
        w.show();
        MainWindow mw;
        mw.show();


        retval = a.exec();
    }

    return retval;
}

