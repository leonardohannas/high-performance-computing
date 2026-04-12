#include <stdio.h>
#include <stdlib.h>

#define MEMORY_ALLOCATION_ERROR 1


void read_input_file(const char* filename, int* R, int* C, int* A, int* N, int* T, int* seed) {
    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        exit(MEMORY_ALLOCATION_ERROR);
    }

    fscanf(f, "%d %d %d %d %d %d", R, C, A, N, T, seed);
    printf("R: %d, C: %d, A: %d, N: %d, T: %d, seed: %d\n", *R, *C, *A, *N, *T, *seed);
    fclose(f);
}

void** allocate_matrix(int R, int C, int A, int N) {

    void** matrix_student_grade = (void**) malloc(sizeof(void*) * R * C * A);
    if (matrix_student_grade == NULL) {
        printf("Erro ao alocar memória para a matriz de notas dos alunos!\n");
        return (void**) MEMORY_ALLOCATION_ERROR;
    }

    for (int i = 0; i < R * C * A; i++) {
        matrix_student_grade[i] = malloc(sizeof(void) * N);
        if (matrix_student_grade[i] == NULL) {
            printf("Erro ao alocar memória para a linha %d da matriz de notas dos alunos!\n", i);
            return (void**) MEMORY_ALLOCATION_ERROR;
        }
    }

    return matrix_student_grade;

}




int main(int argc, char* argv[]) {

    int R, C, A, N, T, seed;
    read_input_file("../docs/exemplo_entrada_0.txt", &R, &C, &A, &N, &T, &seed);
 
    void** matrix_student_grade = (void**) allocate_matrix(R, C, A, N);
    if (matrix_student_grade == (void**) MEMORY_ALLOCATION_ERROR) {
        return MEMORY_ALLOCATION_ERROR;
    }



    return 0;

}

