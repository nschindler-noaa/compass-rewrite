#ifndef C_RESULTS_H
#define C_RESULTS_H

#include <QList>

#include "settings.h"
#include "RiverSegment.h"

struct run_data {
    float inriver_in;
    float transport_in;
    float inriver_out;
    float transport_out;
    int day_start;
    int day_stop;
    float water_travel_time;
};

struct run_stats {
    float mean;
    float median;
    float std_dev;

};

class SegmentResults
{
public:
    SegmentResults(RiverSegment *seg, int games = 1);
    ~SegmentResults ();

    bool done;
    int games;
    RiverSegment * segment;

    QList <float> inriver_in;
    struct run_stats inriver_in_stats;
    QList <float> transport_in;
    struct run_stats transport_in_stats;
    QList <float> inriver_out;
    struct run_stats inriver_out_stats;
    QList <float> transport_out;
    struct run_stats transport_out_stats;
    QList <int> day_start;
    struct run_stats day_start_stats;
    QList <int> day_stop;
    struct run_stats day_stop_stats;
    QList <float> water_travel_time;
    struct run_stats water_travel_time_stats;


};

class Results
{
public:
    Results(Settings *set);
    ~Results ();

    bool done;
    int games;

    QList <SegmentResults> segResults;

    QList <float> inriver_in;
    struct run_stats inriver_in_stats;
    QList <float> transport_in;
    struct run_stats transport_in_stats;
    QList <float> inriver_out;
    struct run_stats inriver_out_stats;
    QList <float> transport_out;
    struct run_stats transport_out_stats;
    QList <int> day_start;
    struct run_stats day_start_stats;
    QList <int> day_stop;
    struct run_stats day_stop_stats;
    QList <float> water_travel_time;
    struct run_stats water_travel_time_stats;
};

#endif // C_RESULTS_H
