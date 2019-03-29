#ifndef RELEASEDIALOG_H
#define RELEASEDIALOG_H

#include "commondialog.h"

#include "releasetool.h"
#include "GraphDialog.h"
#include "ScheduleDialog.h"


class ReleaseDialog : public CommonDialog
{
    Q_OBJECT

public:
    ReleaseDialog(QWidget *parent = nullptr);
    ~ReleaseDialog();

public slots:
    void setReleases (QList<Release *> relList);

    void Cancel();
    void OK();

private:
    ReleaseTool *tool;
};

#endif // RELEASEDIALOG_H
