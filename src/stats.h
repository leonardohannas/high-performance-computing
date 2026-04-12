#ifndef STATS_H
#define STATS_H

typedef struct {
    float min;
    float max;
    float mean;
    float median;
    float stddev;
} Stats;

void calculate_stats(const float *restrict data, int n, Stats *restrict out_stats, float *restrict temp_buffer);

#endif
