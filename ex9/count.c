#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, target, thread_count;
    int count = 0;
    printf("\nEnter number of elements: ");
    scanf("%d", &n);
    int *arr = (int *)malloc(n * sizeof(int));
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    printf("Enter the target value: ");
    scanf("%d", &target);
    printf("Enter thread count: ");
    scanf("%d", &thread_count);

    #pragma omp parallel for num_threads(thread_count) reduction(+:count)
    for (int i = 0; i < n; i++) {
        if (arr[i] < target) {
            count++;
        }
    }
    printf("Number of elements less than %d: %d\n\n", target, count);
    free(arr);
    return 0;
}