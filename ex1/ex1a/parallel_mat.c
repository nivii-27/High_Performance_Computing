#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>

#define MAX 10

struct Matrix {
    int C[MAX][MAX];
    double exe_time[MAX];
};

int main() {
    int m, n, p, i, j, k;
    int A[MAX][MAX], B[MAX][MAX];
    double max;

    printf("Enter m n p: ");
    scanf("%d %d %d", &m, &n, &p);

    printf("Enter matrix A:\n");
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter matrix B:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < p; j++)
            scanf("%d", &B[i][j]);

    /* As asked in the question */
    int shmid = shmget(7652, sizeof(int) * m * p,
                       IPC_CREAT | 0666);

    for (i = 0; i < m; i++) {
        int pid = fork();

        if (pid == 0) {
            struct Matrix *res = shmat(shmid, NULL, 0);
            struct timeval t1, t2;

            gettimeofday(&t1, NULL);

            /* One row computation */
            for (j = 0; j < p; j++) {
                res->C[i][j] = 0;
                for (k = 0; k < n; k++) {
                    res->C[i][j] += A[i][k] * B[k][j];
                }
            }

            gettimeofday(&t2, NULL);

            res->exe_time[i] =
                (t2.tv_sec - t1.tv_sec) +
                (t2.tv_usec - t1.tv_usec) / 1000000.0;

            shmdt(res);
            exit(0);
        }
    }

    /* Parent waits for all children */
    for (i = 0; i < m; i++)
        wait(NULL);

    /* Parent attaches shared memory */
    struct Matrix *res = shmat(shmid, NULL, 0);

    /* Find maximum execution time */
    max = res->exe_time[0];
    for (i = 1; i < m; i++) {
        if (res->exe_time[i] > max)
            max = res->exe_time[i];
    }

    printf("Result Matrix:\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++)
            printf("%d ", res->C[i][j]);
        printf("\n");
    }

    printf("Parallel Execution Time: %f seconds\n", max);

    shmdt(res);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}