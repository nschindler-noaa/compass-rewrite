
#include <QObject>
#include <QCoreApplication>
#include <QApplication>

#include "CompassConsole.h"
#include "CompassGui.h"
#include "settings.h"
#include "mainwindow.h"

#include <stdio.h>
#include <stdlib.h>

// Installs a message handler for QtDebugMsg, QtInfoMsg, QtWarningMsg
// QtCriticalMsg, QtFatalMsg, and QtSystemMsg
// used by qDebug(), qInfo(), qWarning(), qCritical(), qFatal() - similar to fprint()
// TODO:
// We'll see if this works.
void  consoleOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info:  %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    }
}

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
        qInstallMessageHandler(nullptr);
        QApplication a (argc, argv);
        CompassGui w;
        w.show();

        retval = a.exec();
    }

    return retval;
}

