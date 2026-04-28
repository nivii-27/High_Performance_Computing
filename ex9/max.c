#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, thread_count;
    printf("\nEnter number of elements: ");
    scanf("%d", &n);
    int *arr = (int *)malloc(n * sizeof(int));
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    printf("Enter thread count: ");
    scanf("%d", &thread_count);
    int max_val = arr[0];

    #pragma omp parallel for num_threads(thread_count) reduction(max:max_val)
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    printf("\nThe maximum element in the array is: %d\n\n", max_val);

    free(arr);
    return 0;
}