#include "commondialog.h"

#include "releasedialog.h"

ReleaseDialog::ReleaseDialog(QWidget *parent)
    : CommonDialog(parent)
{
    tool = new ReleaseTool(this);
//    graph = new GraphDialog(this);
//    schedule = new ScheduleDialog(this);

//    connect (tool, SIGNAL(showGraph(bool)), SLOT(showGraph(bool)));
//    connect (tool, SIGNAL(showSchedule(bool)), SLOT(showSchedule(bool)));
    connect (this, SIGNAL(Apply()), tool, SLOT(Apply()));
    connect (this, SIGNAL(Reset()), tool, SLOT(Reset()));
    connect (this, SIGNAL(OK()),     SLOT(OK()));
    connect (this, SIGNAL(Cancel()), SLOT(Cancel()));


}

ReleaseDialog::~ReleaseDialog()
{
    delete tool;
}

void ReleaseDialog::setReleases(QList<Release *> relList)
{

}

void ReleaseDialog::OK()
{
    tool->ApplyAll();
    close();
}

void ReleaseDialog::Cancel()
{
    close();
}

/*void ReleaseDialog::closeEvent(QCloseEvent *evnt)
{
    hide();
}*/
