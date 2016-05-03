#include "Scenario.h"

Scenario::Scenario (QObject *parent) :
    QObject(parent)
{
    river = new RiverSystem (parent);
}

Scenario::Scenario (Settings *set, QObject *parent)
{
    river = new RiverSystem (set->getRiverDesc(), parent);
}

Scenario::~Scenario()
{

}

bool Scenario::initialize()
{

}

bool Scenario::constructRiver (Settings *settings)
{
    bool okay = true;

    okay = river->construct();

    return okay;
}

bool Scenario::run (Results *result, Settings *settings)
{
    bool okay = true;

    switch (settings->getRunmode())
    {
    case Settings::Scenario:
    {
        // set up: compute water flow, temp, and spill
        river->computeFlows ();
        river->computeTemps ();
        river->computeSpill ();
        // run a plain scenario
        okay = runScenario (result, settings);
        break;
    }
    case Settings::MonteCarlo:
    {
        // run an old style Monte Carlo (batch mode default)
        okay = runMonteCarlo (result, settings);
        break;
    }
    case Settings::MonteCarloMV:
    {
        // run a Monte Carlo Multivariate scenario
        okay = runMonteCarloMV (result, settings);
        break;
    }
    case Settings::MultiScenario:
    {
        // run several scenarios, releases, or both
        okay = runMultiScenario (result, settings);
        break;
    }
/*    case Settings::RTScenario:
    {
        // run a real time scenario
        okay = runScenarioRT (result);
        break;
    }
    case Settings::RTMonteCarlo:
    {
        // run a real time, old style Monte Carlo
        okay = runMonteCarloRT (result, settings);
        break;
    }
    case Settings::Calibration:
    {
        // run the calibration routine (for advanced users only)
        okay = runCalibration (result);
        break;
    }*/
    case Settings::None:
    default:
        okay = false;
    }

    return okay;
}

void Scenario::cancel()
{
    stop = true;
}

/** Plain scenario run. This is called by all methods once additional set-up has occurred */
bool Scenario::runScenario (Results *result, Settings *settings)
{
    bool okay = true;
    if (stop)
        return false;

    return okay;
}

bool Scenario::runMonteCarlo (Results *result, Settings *settings)
{

}

bool Scenario::runMonteCarloMV (Results *result, Settings *settings)
{

}

bool Scenario::runMultiScenario (Results *result, Settings *settings)
{

}

bool Scenario::runScenarioRT (Results *result)
{

}

bool Scenario::runMonteCarloRT (Results *result, Settings *settings)
{

}

bool Scenario::runCalibration (Results *result)
{

}

float Scenario::calc_segment_WTT (RiverSegment *seg)
{

}

float Scenario::calc_WTT ()
{

}
