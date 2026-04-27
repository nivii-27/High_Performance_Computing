#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100

int main(int argc, char *argv[]) {
    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    int *A = (int *)malloc(SIZE * sizeof(int));
    int *B = (int *)malloc(SIZE * sizeof(int));
    int *C = (int *)malloc(SIZE * sizeof(int));
    int *threads = (int *)malloc(SIZE * sizeof(int));

    for (int i = 0; i < SIZE; i++) {
        A[i] = i;
        B[i] = i * 10;
    }

    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        C[i] = A[i] + B[i];
        threads[i] = omp_get_thread_num();
    }

    printf("Vector Addition (First 10 results using %d threads):\n", num_threads);
    for (int i = 0; i < 10; i++) {
        printf("Index %d: %d + %d = %d (Thread %d)\n", i, A[i], B[i], C[i], threads[i]);
    }

    free(A); free(B); free(C); free(threads);
    return 0;
}