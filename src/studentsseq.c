#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

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

float **allocate_matrix(int total_students, int N) {
    float **matrix_student_grade = (float **)malloc(sizeof(float *) * (size_t)total_students);
    if (matrix_student_grade == NULL) {
        printf("Erro ao alocar memoria para a matriz de notas dos alunos!\n");
        return NULL;
    }

    for (int i = 0; i < total_students; i++) {
        matrix_student_grade[i] = (float *)malloc(sizeof(float) * (size_t)N);
        if (matrix_student_grade[i] == NULL) {
            printf("Erro ao alocar memoria para a linha %d da matriz de notas dos alunos!\n", i);
            for (int j = 0; j < i; j++) {
                free(matrix_student_grade[j]);
            }
            free(matrix_student_grade);
            return NULL;
        }
    }

    return matrix_student_grade;
}

void free_matrix(float **matrix, int total_students) {
    for (int i = 0; i < total_students; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void populate_matrix(float **matrix, int total_students, int N, int seed) {
    srand(seed);
    for (int i = 0; i < total_students; i++) {
        for (int j = 0; j < N; j++) {
            int r = rand() % 1001;
            matrix[i][j] = r / 10.0f;
        }
    }
}

int main(void) {
    setlocale(LC_ALL, "");

    int R, C, A, N, T, seed;
    read_input_file(INPUT_FILE_PATH, &R, &C, &A, &N, &T, &seed);

    int total_cities = R * C;
    int total_students = R * C * A;

    float **matrix_student_grade = allocate_matrix(total_students, N);
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
        free_matrix(matrix_student_grade, total_students);
        return MEMORY_ALLOCATION_ERROR;
    }

    timer_start();

    for (int s = 0; s < total_students; s++) {
        calculate_stats(matrix_student_grade[s], N, &student_stats[s]);
        all_student_means[s] = student_stats[s].mean;
    }

    for (int c = 0; c < total_cities; c++) {
        calculate_stats(&all_student_means[c * A], A, &city_stats[c]);
    }

    int students_per_region = C * A;
    for (int r = 0; r < R; r++) {
        calculate_stats(&all_student_means[r * students_per_region], students_per_region, &region_stats[r]);
    }

    calculate_stats(all_student_means, total_students, &brasil_stats);

    double elapsed_time = timer_stop();

    printf("Entrada: R=%d, C=%d, A=%d, N=%d, T=%d, seed=%d\n", R, C, A, N, T, seed);
    print_formatted_results(city_stats, region_stats, &brasil_stats, R, C);
    printf("Tempo de resposta em segundos, sem considerar E/S: %.3fs\n", elapsed_time);

    free(student_stats);
    free(city_stats);
    free(region_stats);
    free(all_student_means);
    free_matrix(matrix_student_grade, total_students);
    return 0;
}
