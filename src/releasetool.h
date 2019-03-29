#ifndef C_RELEASETOOL_H
#define C_RELEASETOOL_H

#include <QWidget>
#include <QStringList>

#include "Release.h"
#include "GraphDialog.h"
#include "ScheduleDialog.h"


namespace Ui {
class ReleaseTool;
}

class ReleaseTool : public QWidget
{
    Q_OBJECT

public:
    explicit ReleaseTool(QWidget *parent = nullptr);
    ~ReleaseTool();


public slots:
    void setReleases (QList<Release *> rellist);
    void deleteReleases ();
    void setRelease (Release *rel);
    void Apply(Release *rel = nullptr);
    void Reset(Release *rel = nullptr);
    void ApplyAll();
    void ResetAll();
    void showGraph(bool show = true);
    void showSchedule(bool show = true);

private slots:
    void relChanged(bool changed);
    void newRelease();
    void copyRelease(Release *rel);
    void deleteRelease();

signals:
    void relClean(bool clean);

private:
    Ui::ReleaseTool *ui;

    GraphDialog *graph;
    ScheduleDialog *sched;

    Release *release;
    QList<Release *> *releaseList;
    QList<Release *> localList;
    QList<bool> cleanList;


};

#endif // C_RELEASETOOL_H
