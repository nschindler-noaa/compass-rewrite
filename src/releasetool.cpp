#include <QMessageBox>

#include "releasetool.h"
#include "ui_releasetool.h"

ReleaseTool::ReleaseTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReleaseTool)
{
    ui->setupUi(this);

    graph = new GraphDialog(this);
    sched = new ScheduleDialog(this);

    connect (ui->pushButton_graph, SIGNAL(toggled(bool)), SLOT(showGraph(bool)));
    connect (ui->pushButton_schedule, SIGNAL(toggled(bool)), SLOT(showSchedule(bool)));
    localList.append(nullptr);
    localList.clear();
}

ReleaseTool::~ReleaseTool()
{
    delete ui;
    delete graph;
    delete sched;
    while (localList.count() > 0)
        delete localList.takeLast();
}

void ReleaseTool::showGraph(bool show)
{
    if (show)
    {
        graph->show();
        graph->raise();
    }
    else
    {
        graph->hide();
    }
}

void ReleaseTool::showSchedule(bool show)
{
    if (show)
    {
        sched->show();
        sched->raise();
    }
    else
    {
        sched->hide();
    }
}

void ReleaseTool::setReleases(QList<Release *> rellist)
{
    Release *newrel;
    if (localList.count() > 0)
        deleteReleases();
    for (int i = 0; i < rellist.count(); i++)
    {
        newrel = new Release(*rellist.at(i));
        localList.append(newrel);
    }
}

void ReleaseTool::deleteReleases()
{
    while (localList.count() > 0)
    {
        Release *rel = localList.takeLast();
        delete rel;
    }
}

void ReleaseTool::setRelease (Release *rel)
{
    release = rel;
    // refresh();
}

void ReleaseTool::Apply(Release *rel)
{
    if (rel == nullptr)
        rel = release;

}

void ReleaseTool::Reset(Release *rel)
{
    if (rel == nullptr)
        rel = release;

}

void ReleaseTool::ApplyAll()
{
    for (int i = 0; i < localList.count(); i++)
        Apply(localList.at(i));
}

void ReleaseTool::ResetAll()
{
    for (int i = 0; i < localList.count(); i++)
        Reset(localList.at(i));
}

void ReleaseTool::relChanged(bool changed)
{
    int index = ui->comboBox_existing->currentIndex();
    cleanList[index] = false;
    emit relClean(false);
}

void ReleaseTool::newRelease()

{

}

void ReleaseTool::copyRelease(Release *rel)
{

}

void ReleaseTool::deleteRelease()
{
    int index;
    Release *rel;
    int b = QMessageBox::question(this, "Delete Release", "This will delete the permanent release. Are you sure?", QMessageBox::Yes, QMessageBox::No);
    switch(b)
    {
    case 0:
//        index = findRelease(release);
//        rel = localList(index);
        index = ui->comboBox_existing->currentIndex();
        rel = localList.takeAt(index);
        delete rel;
        break;
    case 1:
    default:
        index = -1;
    }
}

