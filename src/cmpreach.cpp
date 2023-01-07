#include "cmpreach.h"
#include "cmplog.h"
#include "cmath"

float Ufree = 0.045;		/* kfs x10-1*/

cmpReach::cmpReach(cmpRiver *parent) : cmpRiverSegment(parent)
{
    type = Reach;
    cmpRiverSegment::setType(cmpRiverSegment::Reach);
    clear();
    resetData();
}

cmpReach::cmpReach (QString rname, cmpRiver *parent) :
    cmpRiverSegment (parent)
{
    name = QString (rname);
    type = Reach;
    cmpRiverSegment::setType(cmpRiverSegment::Reach);
    clear ();
    resetData();
}

cmpReach::cmpReach (cmpReach &rhs) :
    cmpRiverSegment(rhs)
{
    name = QString (rhs.getName());
    type = Reach;
    cmpRiverSegment::setType(cmpRiverSegment::Reach);
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
    depthAve = 0.0;
    depthUpper = 0.0;
    depthLower = 0.0;
    elevLower = 0.0;
    wallSlope = 0.0;

    lossMax = 0.0;
    lossMin = 0.0;

    waterParticleTT = 0.0;

    if (!loss.isEmpty())
        loss.clear();
    if (!elevChange.isEmpty())
        elevChange.clear();
    if (!volumeCurr.isEmpty())
        volumeCurr.clear();
    if (!velocity.isEmpty())
        velocity.clear();
    if (!tempDelta.isEmpty())
        tempDelta.clear();

    cmpRiverSegment::setup();
}

void cmpReach::resetData()
{
    allocateDays(366, 2);
}

void cmpReach::allocateDays(int days, int steps)
{
    int daysteps = days * steps;

    cmpRiverSegment::allocateDays(days);

    if (!loss.isEmpty())
        loss.clear();
    for (int i = 0; i < days; i++)
        loss.append(0.0);

    if (!elevChange.isEmpty())
        elevChange.clear();
    if (!volumeCurr.isEmpty())
        volumeCurr.clear();
    if (!velocity.isEmpty())
        velocity.clear();
    if (!tempDelta.isEmpty())
        tempDelta.clear();
    for (int i = 0; i < daysteps; i++)
    {
        elevChange.append(0.0);
        volumeCurr.append(0.0);
        velocity.append(0.0);
        tempDelta.append(0.0);
    }
    cmpRiverSegment::allocateDays(days);
}

bool cmpReach::parseDesc(cmpFile *descfile)
{
    bool okay = true, end = false;
    QString token ("");
    QString na("");

    while (okay && !end)
    {
        token = descfile->popToken ();
        if (token.compare ("eof", Qt::CaseInsensitive) == 0)
        {
            descfile->printEOF("Reach description");
            okay = false;
        }
        else if (token.compare("width", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, widthAve);
        }
        else if (token.compare("upper_depth", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, depthUpper);
        }
        else if (token.compare("lower_depth", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, depthLower);
        }
        else if (token.compare("slope", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, wallSlope);
        }
        else if (token.compare("lower_elev", Qt::CaseInsensitive) == 0)
        {
            okay = descfile->readFloatOrNa(na, elevLower);
        }
/*        else if (token.compare("latlon", Qt::CaseInsensitive) == 0)
        {
            cmpRiverPoint *pt = new cmpRiverPoint();
            okay = descfile->readString(token);
            pt->parse(token);
            addCoursePoint(pt);
        }*/
        else if (token.compare("end", Qt::CaseInsensitive) == 0)
        {
            descfile->checkEnd("reach", name);
            end = true;
        }
        else
        {
            cmpRiverSegment::parseDescToken(token, descfile);
        }
    }
    return okay;
}

void cmpReach::outputDesc(cmpFile *ofile)
{
    if (ofile->isOpen())
    {
        ofile->writeString(1, QString("reach"), name);

        ofile->writeString(2, QString("width %1").arg(widthAve));
        ofile->writeString(2, QString("lower_depth %1").arg(depthLower));
        ofile->writeString(2, QString("upper_depth %1").arg(depthUpper));
        ofile->writeString(2, QString("slope %1").arg(wallSlope));
        ofile->writeString(2, QString("lower_elev %1").arg(elevLower));
        for (int i = 0; i < course.count(); i++)
        {
            ofile->writeString(2, course.at(i)->getLatLon());
        }

        ofile->writeEnd(1, QString("reach"), name);
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
    double slopetan = tan (wallSlope);
    float lwr_depth_max = depthLower * .95; // 5% less than bottom
    float avg_flow = 0.0;
    static float secs_per_step = 3600.0 * 24.0/stepsPerDay;

    QString msg("");

    /* Adjust flow by loss in this segment -
     * if less than flow_min, change the loss value and notify user.
     * If flow greater than flow_max, set flow_max to actual flow. */
    for (int i = 0; i < daysPerSeason; i++)
    {
        newflow = flow[i] - loss[i];
        if (newflow < flowMin - .0001)
        {
            newflow = flowMin;
            newloss = flow[i] - newflow;
            msg = QString (QString("insufficient flow, %1, on day %2 at %3;\n adjusting old loss: %4, new loss: %5\n")
                           .arg(QString::number(flow[i], 'g', 2), QString::number(i),
                                QString::number(loss[i], 'g' ,2), QString::number(newloss, 'g', 2)));
            cmpLog::outlog->add(cmpLog::Warning, msg);
            loss[i] = newloss;
        }
        flow[i] = newflow;

        // check flow_max
        if (flow[i] > flowMax)
            flowMax = flow[i];
    }

    /* Calculate velocity in miles per time step, averaged over input
       and output flows. Velocity is based on upper and lower depths
       and slope tangent. Volume is computed first and kept.*/

    // calculate volume of the full reach
    if (volume <= 0)
    {
        volume = computeVolume (0.0, depthUpper, depthLower, widthAve, slopetan);
    }
    if (volume < 0)
    {
        cmpLog::outlog->add(cmpLog::Error, QString(
                     QString("check river description file, segment %1\n").arg (name)));
        return;
    }

    for (int i = 0; i < daysPerSeason; i++)
    {
        float tempVol = 0.0;
        float tempVel = 0.0;

        // Adjust elevation if out of range; use a minimum depth of 5%
        // in forebay, as represeneted by lower depth.
        if (elevChange[i] > -(lwr_depth_max))
        {
            elevChange[i] = -(lwr_depth_max);
            msg = QString (QString("elevation drop below depth in reach %1\n    adjusting ...\n")
                           .arg(name));
        }
        avg_flow = flow[i] + (loss[i] / 2);
        if (avg_flow < 0.0)
            avg_flow = 0.0;  // cannot be less than 0 (duh!)

        tempVol = computeVolume (elevChange[i], depthUpper, depthLower, widthAve, slopetan);
        tempVel = computeVelocity (elevChange[i], depthUpper, depthLower, avg_flow);

        for (int j = 0; j < stepsPerDay; j++)
        {
            int index = i * stepsPerDay + j;
            volumeCurr [index] = tempVol;
            velocity [index] = tempVel * 0.19 * secs_per_step;
        }
    }

    // Average water particle travel time
    waterParticleTT = computeWTT (1, 365);//water_travel.first_day, water_travel.last_day);

    // Print stats for day 1
    msg = QString (QString("reach %1 length (mi) %2, \n\tday 1: vel(mi/hr) %3, vel_conv(mi/day) %4, \n\tvol(acre-ft) %5, temp %6\n"))
          .arg(name, QString::number(length, 'g', 2),
               QString::number(velocity[1], 'g', 2), QString::number(velocity[1]*24.0, 'g', 2),
               QString::number(volumeCurr[1], 'g', 2), QString::number(temp[1], 'g', 2));
    cmpLog::outlog->add(cmpLog::Debug, msg);
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
    int firstStep = firstDay * stepsPerDay;
    int lastStep = lastDay * stepsPerSeason;

    for (i = firstStep; i <= lastStep && i < stepsPerSeason; ++i)
        avg_vel += velocity[i];
    avg_vel /= (i-firstStep + 1);

    /* return average water particle travel time through this seg in days */
    wtt = (length / avg_vel) / stepsPerDay;

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

int cmpReach::getReachClass() const
{
    return rclass;
}

void cmpReach::setReachClass(int value)
{
    rclass = value;
}

QString cmpReach::getReachClassStr() const
{
    return reachClass;
}

void cmpReach::setReachClassStr(QString rclass)
{
    reachClass = rclass;
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

float cmpReach::getDepthAve() const
{
    return depthAve;
}

void cmpReach::setDepthAve(float value)
{
    depthAve = value;
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
    return wallSlope;
}

void cmpReach::setSlope(float value)
{
    wallSlope = value;
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

bool cmpReach::parseData (cmpFile *cfile)
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
            okay = cfile->checkEnd("reach", name);
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
    QString tmpstr;

    if (token.compare ("reach_class", Qt::CaseInsensitive) == 0)
    {
        okay = cfile->readString(tmpstr);
        reachClass = tmpstr;
    }
    else if (token.compare ("elevation_change", Qt::CaseInsensitive) == 0)
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

void cmpReach::writeData(cmpFile *outfile, int indent, bool outputAll)
{
    outfile->writeString(indent, "reach", name);
    writeConfigData(outfile, indent+1, outputAll);

    outfile->writeEnd(indent, "reach", name);
}

void cmpReach::writeConfigData(cmpFile *outfile, int indent, bool outputAll)
{
    outfile->writeString(indent, "reach_class", reachClass);
    outfile->writeValue (indent, "output_settings", outputSettings);
}
