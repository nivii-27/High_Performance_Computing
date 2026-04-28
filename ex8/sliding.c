#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    int n, k;
    printf("Enter the size of the array (n): ");
    scanf("%d", &n);
    printf("Enter the sliding window size (k): ");
    scanf("%d", &k);

    if (k > n || k <= 0) {
        printf("Error: Invalid window size!\n");
        return 1;
    }

    int *A = (int *)malloc(n * sizeof(int));
    int num_windows = n - k + 1;
    int *sums = (int *)malloc(num_windows * sizeof(int));
    int *threads = (int *)malloc(num_windows * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 100;
    }

    #pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < num_windows; i++) {
        int current_sum = 0;
        for (int j = 0; j < k; j++) {
            current_sum += A[i + j];
        }
        sums[i] = current_sum;
        threads[i] = omp_get_thread_num();
    }

    printf("\nSliding Window Sums :\n");
    int limit = (num_windows < 10) ? num_windows : 10;
    for (int i = 0; i < limit; i++) {
        printf("Window %d: Sum = %d (Thread %d)\n", i + 1, sums[i], threads[i]);
    }

    free(A);
    free(sums);
    free(threads);

    return 0;
}