#include "stats.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void swap(float *a, float *b) {
    float t = *a;
    *a = *b;
    *b = t;
}

static int partition(float *arr, int left, int right) {
    float pivot = arr[right];
    int i = left;
    for (int j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[right]);
    return i;
}

static float quickselect(float *arr, int left, int right, int k) {
    if (left == right)
        return arr[left];

    int pivot_index = partition(arr, left, right);

    if (k == pivot_index) {
        return arr[k];
    } else if (k < pivot_index) {
        return quickselect(arr, left, pivot_index - 1, k);
    } else {
        return quickselect(arr, pivot_index + 1, right, k);
    }
}

void calculate_stats(const float *restrict data, int n, Stats *restrict out_stats, float *restrict temp_buffer) {
    if (n <= 0)
        return;

    const int prefetch_distance = 16;
    float min_val = data[0];
    float max_val = data[0];
    float mean = 0.0f;
    float M2 = 0.0f;

    for (int i = 0; i < n; i++) {
        if (i + prefetch_distance < n) {
            __builtin_prefetch(&data[i + prefetch_distance], 0, 0);
        }

        float x = data[i];

        min_val = (x < min_val) ? x : min_val;
        max_val = (x > max_val) ? x : max_val;

        float delta = x - mean;
        mean += delta / (i + 1);
        float delta2 = x - mean;
        M2 = fmaf(delta, delta2, M2);
    }

    out_stats->min = min_val;
    out_stats->max = max_val;
    out_stats->mean = mean;
    out_stats->stddev = sqrtf(M2 / n);

    // Hint to the compiler that this straight-line copy loop is safe to vectorize.
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC ivdep
#endif
    for (int i = 0; i < n; i++) {
        temp_buffer[i] = data[i];
    }

    int mid = n / 2;
    if (n % 2 != 0) {
        out_stats->median = quickselect(temp_buffer, 0, n - 1, mid);
    } else {
        float m1 = quickselect(temp_buffer, 0, n - 1, mid - 1);
        float m2 = quickselect(temp_buffer, mid, n - 1, mid);
        out_stats->median = (m1 + m2) / 2.0f;
    }
}