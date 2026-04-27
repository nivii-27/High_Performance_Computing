#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100
#define CONSTANT 50

int main(int argc, char *argv[]) {
    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    int *A = (int *)malloc(SIZE * sizeof(int));
    int *threads = (int *)malloc(SIZE * sizeof(int));
    for (int i = 0; i < SIZE; i++) {
        A[i] = i;
    }
    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        A[i] = A[i] + CONSTANT;
        threads[i] = omp_get_thread_num();
    }
    printf("Scalar Addition (First 10 results using %d threads):\n", num_threads);
    for (int i = 0; i < 10; i++) {
        printf("Index %d: New Value = %d (Thread %d)\n", i, A[i], threads[i]);
    }

    free(A); free(threads);
    return 0;
}