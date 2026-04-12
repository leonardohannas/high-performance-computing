#define MEMORY_ALLOCATION_ERROR 1
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#include "stats.h"
#include "utils.h"

#define MEMORY_ALLOCATION_ERROR 1
#define INPUT_FILE_PATH "../docs/exemplo_entrada_0.txt"

void read_input_file(const char *filename, int *R, int *C, int *A, int *N, int *T, int *seed) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        exit(MEMORY_ALLOCATION_ERROR);
    }

    if (fscanf(f, "%d %d %d %d %d %d", R, C, A, N, T, seed) != 6) {
        printf("Erro ao ler parametros do arquivo!\n");
        fclose(f);
        exit(MEMORY_ALLOCATION_ERROR);
    }

    fclose(f);
}

float *allocate_matrix(int total_students, int N) {
    size_t total_elements = (size_t)total_students * (size_t)N;
    size_t total_bytes = total_elements * sizeof(float);
    size_t alignment = 64;
    size_t padded_bytes = (total_bytes + alignment - 1) & ~(alignment - 1);

    float *matrix_student_grade = (float *)aligned_alloc(alignment, padded_bytes);
    if (matrix_student_grade == NULL) {
        printf("Erro ao alocar memoria alinhada para a matriz de notas dos alunos!\n");
        return NULL;
    }
    return matrix_student_grade;
}

void populate_matrix(float *matrix, int total_students, int N, int seed) {
#pragma omp parallel for default(none) shared(matrix, total_students, N, seed)
    for (int i = 0; i < total_students; i++) {
        unsigned int local_seed = (unsigned int)(seed + i);

        for (int j = 0; j < N; j++) {
            int r = rand_r(&local_seed) % 1001;
            matrix[i * N + j] = r / 10.0f;
        }
    }
}

int main(void) {
    setlocale(LC_ALL, "");

    int R, C, A, N, T, seed;
    read_input_file(INPUT_FILE_PATH, &R, &C, &A, &N, &T, &seed);

    omp_set_num_threads(T);

    int total_cities = R * C;
    int total_students = R * C * A;

    float *matrix_student_grade = allocate_matrix(total_students, N);
    if (matrix_student_grade == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }

    populate_matrix(matrix_student_grade, total_students, N, seed);

    Stats *student_stats = (Stats *)malloc(sizeof(Stats) * (size_t)total_students);
    Stats *city_stats = (Stats *)malloc(sizeof(Stats) * (size_t)total_cities);
    Stats *region_stats = (Stats *)malloc(sizeof(Stats) * (size_t)R);
    float *all_student_means = (float *)malloc(sizeof(float) * (size_t)total_students);
    Stats brasil_stats;

    if (student_stats == NULL || city_stats == NULL || region_stats == NULL || all_student_means == NULL) {
        printf("Erro ao alocar memoria para estruturas de estatistica!\n");
        free(student_stats);
        free(city_stats);
        free(region_stats);
        free(all_student_means);
        free(matrix_student_grade);
        return MEMORY_ALLOCATION_ERROR;
    }

    timer_start();

#pragma omp parallel default(none) shared(matrix_student_grade, student_stats, all_student_means, total_students, N, stderr)
    {
        float *thread_local_buffer = (float *)malloc(sizeof(float) * (size_t)N);
        if (thread_local_buffer == NULL) {
            fprintf(stderr, "Fatal: Thread Local Allocation Failed!\n");
            abort();
        }

#pragma omp for
        for (int s = 0; s < total_students; s++) {
            calculate_stats(&matrix_student_grade[s * N], N, &student_stats[s], thread_local_buffer);
            all_student_means[s] = student_stats[s].mean;
        }

        free(thread_local_buffer);
    }

#pragma omp parallel default(none) shared(all_student_means, city_stats, total_cities, A)
    {
        float thread_local_buffer[A];
#pragma omp for
        for (int c = 0; c < total_cities; c++) {
            calculate_stats(&all_student_means[c * A], A, &city_stats[c], thread_local_buffer);
        }
    }

    int students_per_region = C * A;
#pragma omp parallel default(none) shared(all_student_means, region_stats, R, students_per_region)
    {
        float thread_local_buffer[students_per_region];
#pragma omp for
        for (int r = 0; r < R; r++) {
            calculate_stats(&all_student_means[r * students_per_region], students_per_region, &region_stats[r], thread_local_buffer);
        }
    }

    float *brasil_buffer = (float *)malloc(sizeof(float) * (size_t)total_students);
    if (brasil_buffer == NULL) {
        fprintf(stderr, "Fatal: Brasil buffer allocation failed!\n");
        abort();
    }
    calculate_stats(all_student_means, total_students, &brasil_stats, brasil_buffer);
    free(brasil_buffer);

    double elapsed_time = timer_stop();

    printf("Entrada: R=%d, C=%d, A=%d, N=%d, T=%d, seed=%d\n", R, C, A, N, T, seed);
    print_formatted_results(city_stats, region_stats, &brasil_stats, R, C);
    printf("Tempo de resposta em segundos, sem considerar E/S: %.3fs\n", elapsed_time);

    free(student_stats);
    free(city_stats);
    free(region_stats);
    free(all_student_means);
    free(matrix_student_grade);

    return 0;
}
