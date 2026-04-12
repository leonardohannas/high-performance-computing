#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE *f;
    f = fopen("../docs/exemplo_entrada_0.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }
    int R, C, A, N, T, seed;
    fscanf(f, "%d %d %d %d %d %d", &R, &C, &A, &N, &T, &seed);
    printf("R: %d, C: %d, A: %d, N: %d, T: %d, seed: %d\n", R, C, A, N, T, seed);
    fclose(f);
    return 0;
}