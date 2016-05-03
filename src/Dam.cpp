#include "Dam.h"


Dam::Dam(QString dname, QString rivName, QObject *parent) :
    RiverSegment (rivName, parent)
{
    name = new QString (dname);
    type = RiverSegment::Dam;
    clear ();
}

Dam::~Dam ()
{
}

void Dam::clear()
{
    basin = NULL;
    species = NULL;
    phouse_side =  (Location)-1;
    width = 0.0;
    tailrace_length = 0.0;
    base_elev = 0.0;
    forebay_elev = 0.0;
    tailrace_elev = 0.0;
    full_head = 0.0;
    full_fb_depth = 0.0;
    bypass_height = 0.0;
    collector = 0;
    ngates = 0;
    gate_width = 0.0;
    pergate = 0.0;
    spillway_width = 0.0;
    basin_length = 0.0;
    sgr = 0.0;

    for (int i = 0; i < DAM_SLICES_IN_SEASON; i++)
    {
        spill[i] = 0.0;
        planned_spill[i] = 0.0;
        daylight_proportion[i] = 0.0;
    }

    planned_spill_day = NULL;
    planned_spill_night = NULL;
    legacy_planned_spill = NULL;
    legacy_fish_spill = NULL;
    spill_cap = 0.0;
    spill_side = (Location)-1;
    rsw_spill_cap = 0.0;
    rsw_active = NULL;

    flow_max = 0.0;
    flow_min_project = 0.0;
    flow_min_river = 0.0;

    for (int i = 0; i < DAYS_IN_SEASON; i++)
    {
        flow[i] = 0.0;
        forebay_depth[i] = 0.0;
        tailrace_depth[i] = 0.0;
        drop_ratio[i] = 0.0;
        tr_drop_ratio[i] = 0.0;
    }

    transport = NULL;
}

void Dam::calculateFlow()
{
    calculateFlowInputs();
    calculateFlows();
}

void Dam::calculateFlows()
{

}

void Dam::calculateTemp()
{
    if (!read_temps)
    {
        calculateTempInputs();
        calculateTemps();
    }
}

void Dam::calculateTemps()
{

}

void Dam::calculateSpill()
{

}

void Dam::calculateFish()
{

}

void Dam::calculateStats()
{

}

void Dam::deleteSpill()
{

}

bool Dam::parse (CompassFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Dam data.");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->checkEnd("dam", *name);
            end = true;
        }
        else
        {
            okay = parseToken (token, cfile);
        }
    }

    return okay;
}

bool Dam::parseToken (QString token, CompassFile *cfile)
{
    bool okay = true;
    QString na ("");

    if (token.compare ("elevation_change", Qt::CaseInsensitive) == 0)
    {
//        okay = cfile->readFloatArray (elev_change);
    }
    else if (token.compare("loss_max", Qt::CaseInsensitive) == 0)
    {
//        okay = cfile->readFloatOrNa(na, loss_max);
    }
    else if (token.compare("loss_min", Qt::CaseInsensitive) == 0)
    {
//        okay = cfile->readFloatOrNa(na, loss_min);
    }
    else if (token.compare ("loss", Qt::CaseInsensitive) == 0)
    {
 //       okay = cfile->readFloatArray (loss);
    }
    else
    {
        okay = RiverSegment::parseToken (token, cfile);
    }

    return okay;
}
