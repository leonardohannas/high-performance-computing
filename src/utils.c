#define _POSIX_C_SOURCE 200809L

#include "utils.h"

#include <stdio.h>
#include <time.h>

static struct timespec start_time;

void timer_start(void) { clock_gettime(CLOCK_MONOTONIC, &start_time); }

double timer_stop(void) {
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double start_sec = (double)start_time.tv_sec + ((double)start_time.tv_nsec / 1e9);
    double end_sec = (double)end_time.tv_sec + ((double)end_time.tv_nsec / 1e9);

    return end_sec - start_sec;
}

void print_formatted_results(const Stats *city_stats, const Stats *region_stats, const Stats *brasil_stats, int R, int C) {
    const int COL_LAB = 15;
    const int COL_DAT = 10;

    printf("%-*s %*s %*s %*s %*s %*s\n", COL_LAB, "Cidades", COL_DAT, "Min Nota", COL_DAT, "Max Nota", COL_DAT, "Mediana", COL_DAT, "Media", COL_DAT, "DsvPdr");

    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            int idx = r * C + c;
            char label[32];
            snprintf(label, sizeof(label), "R=%d, C=%d", r, c);

            printf("%-*s %*.1f %*.1f %*.1f %*.1f %*.1f\n", COL_LAB, label, COL_DAT, city_stats[idx].min, COL_DAT, city_stats[idx].max, COL_DAT, city_stats[idx].median, COL_DAT, city_stats[idx].mean, COL_DAT, city_stats[idx].stddev);
        }
    }
    printf("\n");

    printf("%-*s %*s %*s %*s %*s %*s\n", COL_LAB, "Regioes", COL_DAT, "Min Nota", COL_DAT, "Max Nota", COL_DAT, "Mediana", COL_DAT, "Media", COL_DAT, "DsvPdr");

    for (int r = 0; r < R; r++) {
        char label[32];
        snprintf(label, sizeof(label), "R=%d", r);

        printf("%-*s %*.1f %*.1f %*.1f %*.1f %*.1f\n", COL_LAB, label, COL_DAT, region_stats[r].min, COL_DAT, region_stats[r].max, COL_DAT, region_stats[r].median, COL_DAT, region_stats[r].mean, COL_DAT, region_stats[r].stddev);
    }
    printf("\n");

    printf("%-*s %*s %*s %*s %*s %*s\n", COL_LAB, "Brasil", COL_DAT, "Min Nota", COL_DAT, "Max Nota", COL_DAT, "Mediana", COL_DAT, "Media", COL_DAT, "DsvPdr");

    printf("%-*s %*.1f %*.1f %*.1f %*.1f %*.1f\n", COL_LAB, "", COL_DAT, brasil_stats->min, COL_DAT, brasil_stats->max, COL_DAT, brasil_stats->median, COL_DAT, brasil_stats->mean, COL_DAT, brasil_stats->stddev);
    printf("\n");

    int best_region_idx = 0;
    float max_region_mean = region_stats[0].mean;
    for (int r = 1; r < R; r++) {
        if (region_stats[r].mean > max_region_mean) {
            max_region_mean = region_stats[r].mean;
            best_region_idx = r;
        }
    }

    int best_city_overall_idx = 0;
    float max_city_mean = city_stats[0].mean;
    for (int i = 1; i < (R * C); i++) {
        if (city_stats[i].mean > max_city_mean) {
            max_city_mean = city_stats[i].mean;
            best_city_overall_idx = i;
        }
    }

    int best_city_r = best_city_overall_idx / C;
    int best_city_c = best_city_overall_idx % C;

    printf("%-*s %-*s %*s\n", 16, "Premiacao", 8, "Reg/Cid", 10, "Media Arit");

    char best_reg_label[32];
    char best_cit_label[32];
    snprintf(best_reg_label, sizeof(best_reg_label), "R%d", best_region_idx);
    snprintf(best_cit_label, sizeof(best_cit_label), "R%d-C%d", best_city_r, best_city_c);

    printf("%-*s %-*s %10.1f\n", 16, "Melhor regiao:", 8, best_reg_label, max_region_mean);
    printf("%-*s %-*s %10.1f\n", 16, "Melhor cidade:", 8, best_cit_label, max_city_mean);
    printf("\n");
}
