#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int m, n, p;

    printf("\nEnter m n p: ");
    scanf("%d %d %d", &m, &n, &p);

    /* Dynamic memory allocation */
    int **A = (int **)malloc(m * sizeof(int *));
    int **B = (int **)malloc(n * sizeof(int *));
    int **C = (int **)malloc(m * sizeof(int *));

    for (int i = 0; i < m; i++)
        A[i] = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
        B[i] = (int *)malloc(p * sizeof(int));

    for (int i = 0; i < m; i++)
        C[i] = (int *)malloc(p * sizeof(int));

    /* Random number generation */
    srand(time(NULL));

    //printf("Matrix A:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10;
           // printf("%d ", A[i][j]);
        }
       // printf("\n");
    }

   // printf("Matrix B:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            B[i][j] = rand() % 10;
     //       printf("%d ", B[i][j]);
        }
       // printf("\n");
    }

    /* High-resolution timing */
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    /* Serial matrix multiplication */
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

   /* printf("Result Matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    /* Time calculation */
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec)
               + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nSerial Execution Time: %.9f seconds\n\n", time_taken);

    /* Free memory */
    for (int i = 0; i < m; i++) free(A[i]);
    for (int i = 0; i < n; i++) free(B[i]);
    for (int i = 0; i < m; i++) free(C[i]);
    free(A);
    free(B);
    free(C);

    return 0;
}