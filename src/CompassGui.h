#ifndef C_COMPASSGUI_H
#define C_COMPASSGUI_H

#include <QMainWindow>

#include "LogWidget.h"
#include "FileManager.h"
#include "ScenarioManager.h"
#include "settings.h"
#include "releasedialog.h"
#include "transporttool.h"
#include "HelpDialog.h"
#include "equationdialog.h"

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
    ~CompassGui() override;

    FileManager *fManager;
    ScenarioManager *sManager;
    Settings *compassSettings;
    ReleaseDialog *rlsTool;
    TransportTool *transTool;
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
    EquationDialog *eqnDialog;

    Window currWindow;

private slots:
    void aboutQt();
    void about();
    void exit();

    void selectFiles();
    void showEquation(cmpEquation *eqn = nullptr);

};

void  guiOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // C_COMPASSGUI_H
