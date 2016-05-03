#include "ScenarioManager.h"
#include "FileManager.h"


ScenarioManager::ScenarioManager(QObject *parent) :
    QObject(parent)
{
}

bool ScenarioManager::run (RiverSystem *&rs, Settings *&settings)
{
    bool okay = true;

    switch (settings->getRunmode())
    {
    case Settings::Scenario:
    {
        // compute water flow, temp, and spill
        rs->computeFlows ();
        rs->computeTemps ();
        rs->computeSpill ();
        break;
    }
    case Settings::MonteCarlo:
    {
        // compute water flow, temp, and spill
        rs->computeFlows ();
        rs->computeTemps ();
        rs->computeSpill ();
        break;
    }
    case Settings::MonteCarloMV:
    {
        // compute water flow, temp, and spill
        rs->computeFlows ();
        rs->computeTemps ();
        rs->computeSpill ();
        break;
    }
    case Settings::MultiScenario:
    {
        // compute water flow, temp, and spill
        rs->computeFlows ();
        rs->computeTemps ();
        rs->computeSpill ();
        break;
    }
/*    case Settings::RTScenario:
    {
        break;
    }
    case Settings::RTMonteCarlo:
    {
        break;
    }*/
    case Settings::None:
    default:
        okay = false;
    }

    return okay;
}

