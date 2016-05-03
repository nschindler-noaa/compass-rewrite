#include <QStringList>
#include <QMessageBox>
#include <QHBoxLayout>


#include "CompassGui.h"
#include "ui_CompassGui.h"
#include "LogWidget.h"
#include "version.h"
#include "HelpDialog.h"
#include "FileManager.h"
#include "ScenarioManager.h"
#include "settings.h"
#include "mainwindow.h"

CompassGui::CompassGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CompassGui)
{
    compassSettings = new Settings();
    QStringList args (qApp->arguments ());
    compassSettings->restore();

    compassSettings->readArguments(args);

    ui->setupUi(this);
    ui->logWindow->hide();
    lw = new LogWidget (ui->logWindow);
    lw->setMessage(true);
    lw->setWarning(true);

    ui->horizontalLayout_log->addWidget(lw);
 //   ui->logWindow->layout()->setMenuBar(lw->menuBar());
//    ui->logWindow->layout()->addWidget (lw);
//    ui->logWindow->setLayout(loglayout);

    ui->logWindow->setFloating(false);
    connect (ui->logWindow, SIGNAL (visibilityChanged(bool)),
             ui->actionShow_Log, SLOT (setChecked (bool)));
    connect (ui->actionShow_Log, SIGNAL (triggered (bool)),
             ui->logWindow, SLOT (setVisible (bool)));

    ui->dockWidget_tools->setFloating(false);
    connect (ui->dockWidget_tools, SIGNAL (visibilityChanged (bool)),
             ui->actionShow_Toolbox, SLOT (setChecked (bool)));
    connect (ui->actionShow_Toolbox, SIGNAL (triggered (bool)),
             ui->dockWidget_tools, SLOT (setVisible (bool)));

    connect (ui->actionShow_Map, SIGNAL (triggered (bool)), SLOT (showMap (bool)));

    makeWindowTitle();

    fManager = new FileManager (this);
    sManager = new ScenarioManager (this);
    HelpDialog hlp (this);
//    hlp.show ();
    connect (ui->actionHelp_Dialog, SIGNAL(triggered()), &hlp, SLOT(show()));


    lw->add (Log::Force, QString ("Running COMPASS in gui mode with the following arguments:\n"));
    for (int i = 0; i < args.count (); i++)
    {
        lw->add (Log::Force, (QString ("\t") + args[i]));
    }

    run();
}

CompassGui::~CompassGui()
{
    delete ui;
    lw->close ();
}

void CompassGui::addLogWindow (QWidget *container)
{
//    Log * log = LogWindow::instance (container);
//    logw = static_cast <LogWindow *> (log);

//    container->layout ()->addWidget (logw);
}

void CompassGui::run ()
{
    // check for river description file, use default or ask

    // read river desc file

    // add data to the map


}

void CompassGui::showTool (Window page)
{
    ui->stackedWidget->setCurrentIndex (page);
}

void CompassGui::makeWindowTitle ()
{
    QString title (version_string());
    switch (COMPASS_RELEASE_LEVEL_CHAR)
    {
    case 'a':
        title.append (" ALPHA level - TESTING ONLY - do not use for production requirements.");
        break;
    case 'b':
        title.append (" BETA level.");
        break;
    case 'x':
        title.append (" EXPERIMENTAL - do not use for production requirements.");
        break;
    }
    setWindowTitle (title);
}

void CompassGui::on_actionE_xit_triggered()
{
    close();
}

void CompassGui::on_action_About_triggered()
{
    QMessageBox h (QMessageBox::Question, "About COMPASS", "some text.");
    h.exec ();
}

void CompassGui::on_action_About_Qt_triggered()
{
    QMessageBox h (QMessageBox::Question, "About COMPASS", "some text.");
    h.exec ();
}
/*
void CompassGui::on_actionShow_Log_toggled(bool show)
{
    lw->setVisible (show);
}*/

