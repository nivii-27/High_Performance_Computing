#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>

int main() {
    int m, n, p;
    int A[10][10], B[10][10], C[10][10];

    struct timespec start, end;

    printf("Enter m n p: ");
    scanf("%d %d %d", &m, &n, &p);

    printf("Enter matrix A:\n");
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter matrix B:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < p; j++)
            scanf("%d", &B[i][j]);

    /* Start high-resolution timer */
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Serial matrix multiplication
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    /* Stop timer */
    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("Result Matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec)
               + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Serial Execution Time: %.9f seconds\n", time_taken);

    return 0;
}