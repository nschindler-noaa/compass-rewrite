#ifndef C_SCENARIOMANAGER_H
#define C_SCENARIOMANAGER_H

#include <QObject>
#include "RiverSystem.h"
#include "settings.h"

class ScenarioManager : public QObject
{
    Q_OBJECT
public:
    explicit ScenarioManager(QObject *parent = nullptr);

signals:
    void complete ();
    void canceled ();
    void running ();
    void progress (int num_run, int year, int release, int game);

public slots:
    bool run (RiverSystem *&rs, Settings *&settings);

};

#endif // C_SCENARIOMANAGER_H
