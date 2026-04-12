#ifndef UTILS_H
#define UTILS_H

#include "stats.h"

void timer_start(void);
double timer_stop(void);
void print_formatted_results(const Stats *city_stats, const Stats *region_stats, const Stats *brasil_stats, int R, int C);

#endif
