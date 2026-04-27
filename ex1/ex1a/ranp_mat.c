#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>

/* Structure for shared memory */
struct Matrix {
    int *C;            // Result matrix (flattened)
    double *exe_time;  // Execution time per process
};

int main() {
    int m, n, p, i, j, k;
    int **A, **B;
    double max;

    printf("\nEnter m n p: ");
    scanf("%d %d %d", &m, &n, &p);

    /* -------- Dynamic allocation for A -------- */
    A = (int **)malloc(m * sizeof(int *));
    for (i = 0; i < m; i++)
        A[i] = (int *)malloc(n * sizeof(int));

    /* -------- Dynamic allocation for B -------- */
    B = (int **)malloc(n * sizeof(int *));
    for (i = 0; i < n; i++)
        B[i] = (int *)malloc(p * sizeof(int));

    /* Seed random numbers */
    srand(getpid());

    /* Generate random matrix A */
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            A[i][j] = rand() % 10;

    /* Generate random matrix B */
    for (i = 0; i < n; i++)
        for (j = 0; j < p; j++)
            B[i][j] = rand() % 10;

    /* Print Matrix A
    printf("\nMatrix A:\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", A[i][j]);
        printf("\n");
    }

    /* Print Matrix B
    printf("\nMatrix B:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++)
            printf("%d ", B[i][j]);
        printf("\n");
    }

    /* Shared memory allocation */
    int shmid = shmget(IPC_PRIVATE,
                       sizeof(int) * m * p + sizeof(double) * m,
                       IPC_CREAT | 0666);

    int *shm = (int *)shmat(shmid, NULL, 0);
    int *C = shm;                    // Result matrix
    double *exe_time = (double *)(shm + m * p);

    /* Fork m processes */
    for (i = 0; i < m; i++) {
        int pid = fork();

        if (pid == 0) {
            struct timeval t1, t2;
            gettimeofday(&t1, NULL);

            for (j = 0; j < p; j++) {
                C[i * p + j] = 0;
                for (k = 0; k < n; k++) {
                    C[i * p + j] += A[i][k] * B[k][j];
                }
            }

            gettimeofday(&t2, NULL);

            exe_time[i] =
                (t2.tv_sec - t1.tv_sec) +
                (t2.tv_usec - t1.tv_usec) / 1000000.0;

            shmdt(shm);
            exit(0);
        }
    }

    /* Parent waits */
    for (i = 0; i < m; i++)
        wait(NULL);

    /* Find max execution time */
    max = exe_time[0];
    for (i = 1; i < m; i++)
        if (exe_time[i] > max)
            max = exe_time[i];

    /* Print result matrix
    printf("\nResult Matrix:\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++)
            printf("%d ", C[i * p + j]);
        printf("\n");
    }
*/
    printf("\nParallel Execution Time: %f seconds\n\n", max);

    /* Cleanup */
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);

    for (i = 0; i < m; i++)
        free(A[i]);
    free(A);

    for (i = 0; i < n; i++)
        free(B[i]);
    free(B);

    return 0;
}