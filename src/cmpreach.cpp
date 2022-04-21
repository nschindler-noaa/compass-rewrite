#include "cmpreach.h"
#include "log.h"
#include "cmath"

float Ufree = 0.045;		/* kfs x10-1*/

cmpReach::cmpReach(QObject *parent) : cmpRiverSegment(parent)
{
    type = cmpRiverSegment::Reach;

}

cmpReach::cmpReach (QString rname, QString rivName, QObject *parent) :
    cmpRiverSegment (rivName, parent)
{
    name = new QString (rname);
    type = cmpRiverSegment::Reach;

    clear ();
}

cmpReach::cmpReach (cmpReach &rhs) :
    cmpRiverSegment(rhs)
{
    name = new QString (*rhs.getName());
    type = cmpRiverSegment::Reach;
    copy (rhs);
}

cmpReach::~cmpReach ()
{
    clear();
}

void cmpReach::clear ()
{
    rclass = 0;
    length = 0.0;
    volume = 0.0;
    surfaceArea = 0.0;
    depth = 0.0;
    depthUpper = 0.0;
    depthLower = 0.0;
    elevLower = 0.0;
    slope = 0.0;

    lossMax = 0.0;
    lossMin = 0.0;

    waterParticleTT = 0.0;

    for (int i = 0; i < elevChange.count(); i++)
    {
        elevChange[i] = 0.0;
        loss[i] = 0.0;
    }
    for (int i = 0; i < velocity.count(); i++)
    {
        volumeCurr[i] = 0.0;
        velocity[i] = 0.0;
        tempDelta[i] = 0.0;
    }
}

void cmpReach::calculateFlow()
{
    if (!isRegPoint)
    {
        calculateFlowInputs(); // sets flow to sum of upstream segments
        calculateFlows ();     // modifies flow by characteristics of segment
    }
}

void cmpReach::calculateFlows()
{
    float newloss = 0.0;
    float newflow = 0.0;
    double slopetan = tan (slope);
    float lwr_depth_max = depthLower * .95; // 5% less than bottom
    float avg_flow = 0.0;
    static float secs_per_step = 3600.0 * 24.0/STEPS_PER_DAY;

    QString msg("");

    /* Adjust flow by loss in this segment -
     * if less than flow_min, change the loss value and notify user.
     * If flow greater than flow_max, set flow_max to actual flow. */
    for (int i = 0; i < DAYS_IN_SEASON; i++)
    {
        newflow = flow[i] - loss[i];
        if (newflow < flowMin - .0001)
        {
            newflow = flowMin;
            newloss = flow[i] - newflow;
            msg = QString (QString("insufficient flow, %1, on day %2 at %3;\n adjusting old loss: %4, new loss: %5\n")
                           .arg(QString::number(flow[i], 'g', 2), QString::number(i),
                                QString::number(loss[i], 'g' ,2), QString::number(newloss, 'g', 2)));
            Log::outlog->add(Log::Warning, msg);
            loss[i] = newloss;
        }
        flow[i] = newflow;

        // check flow_max
        if (flow[i] > flowMax)
            flowMax = flow[i];
    }

    /* Calculate velocity in miles per time step, averaged over input
       and output flows. Velocity is based on upper and lower depths
       and slope tangent. Volume is computed first and kept.

    /* calculate volume of the full reach */
    if (volume <= 0)
    {
        volume = computeVolume (0.0, depthUpper, depthLower, widthAve, slopetan);
    }
    if (volume < 0)
    {
        Log::outlog->add(Log::Error, QString(
                     QString("check river description file, segment %1\n").arg (*name)));
        return;
    }

    for (int i = 0; i < DAYS_IN_SEASON; i++)
    {
        float tempVol = 0.0;
        float tempVel = 0.0;

        /* Adjust elevation if out of range; use a minimum depth of 5%
           in forebay, as represeneted by lower depth. */
        if (elevChange[i] > -(lwr_depth_max))
        {
            elevChange[i] = -(lwr_depth_max);
            msg = QString (QString("elevation drop below depth in reach %1\n    adjusting ...\n")
                           .arg(*name));
        }
        avg_flow = flow[i] + (loss[i] / 2);
        if (avg_flow < 0.0)
            avg_flow = 0.0;  // cannot be less than 0 (duh!)

        tempVol = computeVolume (elevChange[i], depthUpper, depthLower, widthAve, slopetan);
        tempVel = computeVelocity (elevChange[i], depthUpper, depthLower, avg_flow);

        for (int j = 0; j < STEPS_PER_DAY; j++)
        {
            int index = i * STEPS_PER_DAY + j;
            volumeCurr [index] = tempVol;
            velocity [index] = tempVel * 0.19 * secs_per_step;
        }
    }

    /* Average water particle travel time */
    waterParticleTT = computeWTT (1, 365);//water_travel.first_day, water_travel.last_day);

    /* Print stats for day 1 */
    msg = QString (QString("reach %1 length (mi) %2, \n\tday 1: vel(mi/hr) %3, vel_conv(mi/day) %4, \n\tvol(acre-ft) %5, temp %6\n"))
          .arg(*name, QString::number(length, 'g', 2),
               QString::number(velocity[1], 'g', 2), QString::number(velocity[1]*24.0, 'g', 2),
               QString::number(volumeCurr[1], 'g', 2), QString::number(temp[1], 'g', 2));
    Log::outlog->add(Log::Debug, msg);
}

float cmpReach::computeVolume (float elev_chng, float upper_d, float lower_d, float wd, float slp_tan)
{
    float vol = 0.0;

    if (upper_d == lower_d)
    {
        vol = (wd - slp_tan * (lower_d + elev_chng)) * (lower_d - elev_chng);
    }
    else if (elev_chng == 0.0)
    {
        vol = wd * (upper_d + lower_d)/2.0 - slp_tan/3.0
                * ((upper_d + lower_d) * (upper_d + lower_d)/2.0 + upper_d * lower_d);
    }
    else
    {
        float vol_init = (1 / (lower_d - upper_d)) * (lower_d - elev_chng)
                * (lower_d - elev_chng) * (wd/2.0 - slp_tan * (lower_d/6.0 + upper_d/2.0 + elev_chng/3.0 ));
        float vol_exss = 0;
        if (elev_chng >= upper_d)
            vol_exss = 0.0;
        else
        {
            vol_exss = (1 / (lower_d - upper_d)) * (upper_d - elev_chng)
                * (upper_d - elev_chng) * (wd/2.0 - slp_tan * (lower_d/2.0 + upper_d/6.0 + elev_chng/3.0 ));
        }
        vol = vol_init - vol_exss;
    }
    return vol;
}

float cmpReach::computeVelocity(float elev_chng, float upper_d, float lower_d, float avg_fl)
{
    float vel = 0.0;

    if (-(elev_chng) < upper_d)
    {
        if (volume != 0)
            vel = avg_fl / volume;
    }
    else
    {
        vel = (avg_fl == 0)? 0.0
                                  : 1.0 / (volume /
               avg_fl + (1 - lower_d + elev_chng) / (lower_d - upper_d))
                                    / (Ufree * 1.0);
    }
    return vel;
}

float cmpReach::computeWTT(int firstDay, int lastDay)
{
    float wtt = 0.0;
    float avg_vel = 0.0;
    int i = 0;

    /* convert to time steps */
    int firstStep = firstDay * STEPS_PER_DAY;
    int lastStep = lastDay * STEPS_PER_DAY;

    for (i = firstStep; i <= lastStep && i < STEPS_IN_SEASON; ++i)
        avg_vel += velocity[i];
    avg_vel /= (i-firstStep + 1);

    /* return average water particle travel time through this seg in days */
    wtt = (length / avg_vel) / STEPS_PER_DAY;

    return wtt;
}

void cmpReach::calculateTemp()
{
    if (!readTemps)
    {
        calculateTempInputs();
        calculateTemps();
    }
}

void cmpReach::calculateTemps()
{

}

void cmpReach::calculateFish()
{

}

void cmpReach::calculateStats()
{

}

int cmpReach::getRclass() const
{
    return rclass;
}

void cmpReach::setRclass(int value)
{
    rclass = value;
}

float cmpReach::getLength() const
{
    return length;
}

void cmpReach::setLength(float value)
{
    length = value;
}

float cmpReach::getVolume() const
{
    return volume;
}

void cmpReach::setVolume(float value)
{
    volume = value;
}

float cmpReach::getsurfaceArea() const
{
    return surfaceArea;
}

void cmpReach::setsurfaceArea(float value)
{
    surfaceArea = value;
}

float cmpReach::getDepth() const
{
    return depth;
}

void cmpReach::setDepth(float value)
{
    depth = value;
}

float cmpReach::getDepthUpper() const
{
    return depthUpper;
}

void cmpReach::setDepthUpper(float value)
{
    depthUpper = value;
}

float cmpReach::getDepthLower() const
{
    return depthLower;
}

void cmpReach::setDepthLower(float value)
{
    depthLower = value;
}

float cmpReach::getSlope() const
{
    return slope;
}

void cmpReach::setSlope(float value)
{
    slope = value;
}

float cmpReach::getLossMax() const
{
    return lossMax;
}

void cmpReach::setLossMax(float value)
{
    lossMax = value;
}

float cmpReach::getLossMin() const
{
    return lossMin;
}

void cmpReach::setLossMin(float value)
{
    lossMin = value;
}

bool cmpReach::parse (cmpFile *cfile)
{
    bool okay = true, end = false;
    QString token ("");

    while (okay && !end)
    {
        token = cfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            cfile->printEOF("Reach data.");
            okay = false;
        }
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            okay = cfile->checkEnd("reach", *name);
            end = true;
        }
        else
        {
            okay = parseToken (token, cfile);
        }
    }

    return okay;
}

bool cmpReach::parseToken (QString token, cmpFile *cfile)
{
    bool okay = true;
    QString na("");

    if (token.compare ("elevation_change", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatArray (elevChange);
    }
    else if (token.compare("loss_max", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, lossMax);
    }
    else if (token.compare("loss_min", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatOrNa(na, lossMin);
    }
    else if (token.compare ("loss", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readFloatArray (loss);
    }
    else
    {
        okay = cmpRiverSegment::parseToken (token, cfile);
    }

    return okay;
}
