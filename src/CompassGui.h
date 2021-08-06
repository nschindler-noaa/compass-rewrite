#ifndef C_COMPASSGUI_H
#define C_COMPASSGUI_H

#include <QMainWindow>

#include "LogWidget.h"
#include "FileManager.h"
#include "ScenarioManager.h"
#include "settings.h"
#include "releasedialog.h"
#include "HelpDialog.h"

enum Window {
    IO,
    Map,
    Monte,
    Hydro
};

namespace Ui {
    class CompassGui;
}

class CompassGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit CompassGui(QWidget *parent = nullptr);
    ~CompassGui();

    FileManager *fManager;
    ScenarioManager *sManager;
    Settings *compassSettings;
    ReleaseDialog *rlsTool;
    HelpDialog *hManager;

public slots:
    void run ();
    void showTool (Window page);
    void showMap (bool show);
    void showRlsTool (bool show);

private:
//    LogWindow *logw;
    QDialog *logd;
    LogWidget *lw;
    Ui::CompassGui *ui;
    void makeWindowTitle ();
    void addLogWindow (QWidget *container);

    Window currWindow;

private slots:
    void aboutQt();
    void on_action_About_triggered();
//    void on_action_About_Qt_triggered();
//    void on_action_Help_triggered();
//    void on_actionShow_Log_toggled(bool show);
    void on_actionE_xit_triggered();

};

void  guiOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // C_COMPASSGUI_H
