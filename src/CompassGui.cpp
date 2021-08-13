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
#include "releasedialog.h"

CompassGui::CompassGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CompassGui)
{
    compassSettings = new Settings();
    QStringList args (qApp->arguments ());
    compassSettings->restore();

    compassSettings->readArguments(args);

    ui->setupUi(this);

    makeWindowTitle();

    // message logging
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

    fManager = new FileManager (this);
    sManager = new ScenarioManager (this);
    connect (ui->actionOpen_River_desc_File, SIGNAL(triggered()), fManager,
             SLOT(readRiverDescriptionFile(sManager->getScenario(), compassSettings)));
    connect (ui->actionOpen_Data_File, SIGNAL(triggered()), fManager,
             SLOT(readDataFiles(sManager->getScenario(), compassSettings)));
    connect (ui->actionSave_Data_File, SIGNAL(triggered()), fManager,
             SLOT(writeDataFiles(sManager->getScenario(), compassSettings)));

    // set up tools
    rlsTool = new ReleaseDialog(this);
    rlsTool->setVisible(false);
    connect (ui->actionRelease_Tool, SIGNAL(toggled(bool)), this, SLOT(showRlsTool(bool)));
    connect (rlsTool, SIGNAL(visibilityChanged(bool)), ui->actionRelease_Tool, SLOT(setChecked(bool)));

    eqnDialog = nullptr;
    connect (ui->actionShow_Equation, SIGNAL(triggered()), this, SLOT(showEquation()));

    hManager = new HelpDialog(this);
    connect (ui->actionHelp_Dialog, SIGNAL(triggered()), hManager, SLOT(show()));
    connect (ui->action_About, SIGNAL(triggered()), SLOT());
    connect (ui->actionAbout_Qt, SIGNAL(triggered()), SLOT(aboutQt()));

    ui->dockWidget_tools->setFloating(false);
    connect (ui->dockWidget_tools, SIGNAL (visibilityChanged (bool)),
             ui->actionShow_Toolbox, SLOT (setChecked (bool)));
    connect (ui->actionShow_Toolbox, SIGNAL (triggered (bool)),
             ui->dockWidget_tools, SLOT (setVisible (bool)));

    connect (ui->actionShow_Map, SIGNAL (triggered (bool)), SLOT (showMap (bool)));

    lw->add (Log::Force, QString ("Running COMPASS in gui mode with the following arguments:\n"));
    for (int i = 0; i < args.count (); i++)
    {
        lw->add (Log::Force, (QString ("\t") + args[i]));
    }

    currWindow = IO;
    showMap(false);

    run();
}

CompassGui::~CompassGui()
{
    delete ui;
    lw->close ();
}

void CompassGui::addLogWindow (QWidget *container)
{
//    Log * outlog = LogWindow::instance (container);
//    logw = static_cast <LogWindow *> (outlog);

//    container->layout ()->addWidget (logw);
}

void CompassGui::selectFiles()
{

}

void CompassGui::run ()
{
    bool okay = true;
    // check for river description file, use default or ask
    Scenario scenario (this);
    Results results (compassSettings);

    // read in river description file
    okay = fManager->readRiverDescFile (&scenario, compassSettings);

#ifdef DEBUG // write out river description file
    if (okay) {
        QString fn(QString("%1River.check").arg(scenario.river->rivers->at(0)->getName()));
        okay = fManager->writeRiverDescFile (scenario.river, compassSettings, fn);
    }
#endif
    // put all the segments together - create headwaters if needed, etc.
    if (okay)
        okay = scenario.constructRiver (compassSettings);
    // read in scenario information file(s)
    if (okay)
    {
        okay = fManager->readFiles (&scenario, compassSettings);
    }

    // check, fill in missing, and initialize data elements (flow, spill, etc.)
    if (okay)
    {
        scenario.initialize ();
    }
/*    // run the scenario
    if (okay)
    {
        okay = scenario.run (&results, compassSettings);
    }
    // output results data, if requested
    if (okay && compassSettings->getSummary())
    {
        okay = fManager->writeSummary (&results, compassSettings->getOutputData());
    }
    // output specified data file(s), if any
    if (okay)
    {
        okay = fManager->writeFiles (&scenario, compassSettings);
    }

    if (!okay)
    {
        qWarning("Error in running COMPASS console application.");
    }*/
}

void CompassGui::showTool (Window page)
{
    ui->stackedWidget->setCurrentIndex (page);
    currWindow = page;
}

void CompassGui::showMap(bool show)
{
    if (show)
        showTool(Map);
    else
        showTool(IO);
}

void CompassGui::showRlsTool(bool show)
{
    rlsTool->setVisible(show);
    ui->actionRelease_Tool->setChecked(show);
    if (show)
    {
        rlsTool->raise();
    }
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

void CompassGui::exit()
{
    close();
}

void CompassGui::about()
{
    QMessageBox h (QMessageBox::Question, "About COMPASS", "some text.");
    h.exec ();
}

void CompassGui::aboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

/*void CompassGui::on_action_About_Qt_triggered()
{
    QMessageBox::aboutQt(this,tr("About Qt"));
}

void CompassGui::on_actionShow_Log_toggled(bool show)
{
    lw->setVisible (show);
}*/

void CompassGui::showEquation(cmpEquation *eqn)
{
// this is to test the window
    if (eqn == nullptr)
        eqn = new cmpEquation();
//
    if (eqn != nullptr)
    {
        if (eqnDialog == nullptr)
            eqnDialog = new EquationDialog(eqn, this);
        eqnDialog->setWindowTitle("Dam or Reach");
        eqnDialog->show();
    }
}

void  guiOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString file = context.file ? context.file : "";
    QString function = context.function ? context.function : "";
    QString title(QString("File: %1, line: %2, function: %3").arg(file).arg(context.line).arg(function));
    switch (type) {
    case QtDebugMsg:
        title.prepend("Debug    -- ");
        QMessageBox::information(nullptr, title, msg);
        break;
    case QtInfoMsg:
        title.prepend("Info     -- ");
        QMessageBox::information(nullptr, title, msg);
        break;
    case QtWarningMsg:
        title.prepend("Warning  -- ");
        QMessageBox::warning(nullptr, title, msg);
        break;
    case QtCriticalMsg:
        title.prepend("Critical!-- ");
        QMessageBox::critical(nullptr, title, msg);
        break;
    case QtFatalMsg:
        title.prepend("Fatal!   -- ");
        QMessageBox::critical(nullptr, title, msg);
        break;
    }
}


