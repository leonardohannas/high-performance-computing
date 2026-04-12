#ifndef STATS_H
#define STATS_H

typedef struct {
    float min;
    float max;
    float mean;
    float median;
    float stddev;
} Stats;

void calculate_stats(const float *data, int n, Stats *out_stats);

#endif
