#include <QMenuBar>

#include "LogWidget.h"
#include "ui_LogWidget.h"

LogWidget::LogWidget(QWidget *parent) :
    QWidget(parent),
    Log (parent),
    ui(new Ui::LogWidget)
{
    ui->setupUi(this);
/*    menu = new QMenuBar (this);
    QMenu *filemenu = new QMenu ("&File");
    filemenu->addAction (ui->action_Reset);
    filemenu->addAction (ui->actionSave_As);
    filemenu->addAction (ui->action_Print);
    menu->addMenu (filemenu);
    QMenu *editmenu = new QMenu ("&Edit");
    editmenu->addAction (ui->actionSelect_All);
    editmenu->addAction (ui->action_Copy);
    menu->addMenu (editmenu);
    QMenu *logging = new QMenu ("&Logging");
    logging->addAction (ui->actionMessage);
    logging->addAction (ui->actionWarning);
    logging->addAction (ui->actionDebug);
    logging->addAction (ui->actionRaw_Data);
    menu->addMenu (logging);
    QMenu *help = new QMenu ("&Help");
    //help->addAction (ui->ac);
    menu->addSeparator();
    menu->addMenu (help);
    menu->addSeparator();*/

    QMenu *textMenu = new QMenu ("&Text", ui->toolButton_text);
    textMenu->addAction(ui->action_Reset);
    textMenu->addAction(ui->actionSave_As);
    textMenu->addSeparator();
    textMenu->addAction(ui->actionSelect_All);
    textMenu->addAction(ui->action_Copy);
    textMenu->addSeparator();
    textMenu->addAction(ui->action_Print);
    ui->toolButton_text->setMenu (textMenu);

    QMenu *logMenu = new QMenu ("&Logging", ui->toolButton_logging);
    logMenu->addAction(ui->actionMessage);
    logMenu->addAction(ui->actionWarning);
    logMenu->addAction(ui->actionDebug);
    logMenu->addAction(ui->actionRaw_Data);
    ui->toolButton_logging->setMenu (logMenu);

    QMenu *helpMenu = new QMenu ("&Help", ui->toolButton_help);
    helpMenu->addAction(ui->action_Help);
    ui->toolButton_help->setMenu(helpMenu);

    ui->plainTextEdit->setFont(QFont("Courier", 9));
}

LogWidget::~LogWidget()
{
    delete ui;
}

void LogWidget::display (QString text)
{
    ui->plainTextEdit->appendPlainText (text);
}


void LogWidget::on_actionMessage_toggled(bool show)
{
    setMessage (show);
}

void LogWidget::on_actionWarning_toggled(bool show)
{
    setWarning (show);
}

void LogWidget::on_actionRaw_Data_toggled(bool show)
{
    setRawDump (show);
}

void LogWidget::on_actionDebug_toggled(bool show)
{
    setDebug (show);
}

void LogWidget::on_action_Reset_triggered()
{
    clear();
}

void LogWidget::on_actionSave_As_triggered()
{
    // save plainTextEdit to a file
    // emit saveText(ui->plainTextEdit->text());
}

void LogWidget::on_action_Print_triggered()
{
    // print text from plainTextEdit
    // emit printText(ui->plainTextEdit->text());
}

void LogWidget::setMessage(bool flg)
{
    ui->actionMessage->setChecked(flg);
    message = flg;
}

void LogWidget::setWarning (bool flg)
{
    ui->actionWarning->setChecked(flg);
    warning = flg;
}

void LogWidget::setRawDump (bool flg)
{
    ui->actionRaw_Data->setChecked(flg);
    rawdump = flg;
}

void LogWidget::setDebug (bool flg)
{
    ui->actionDebug->setChecked(flg);
    debug = flg;
}

void LogWidget::clear ()
{
    ui->plainTextEdit->clear();
    logFile->close ();
    logFile->open ();
}
