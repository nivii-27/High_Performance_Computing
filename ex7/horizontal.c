#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);
    int r, col;
    printf("Enter the number of rows (r): ");
    scanf("%d", &r);
    printf("Enter the number of columns (col): ");
    scanf("%d", &col);
    int matrix[r][col];

    srand(time(NULL));
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < col; j++) {
            matrix[i][j] = rand() % 100;
        }
    }

    printf("\nInitial Matrix:\n");
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < col; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    // 6. Flip Logic (YOUR ORIGINAL LOGIC - UNCHANGED)
    #pragma omp parallel for
    for (int i = 0; i < r; i++) {
        // Swap elements from left and right up to the middle column
        for (int j = 0; j < col / 2; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[i][col - 1 - j];
            matrix[i][col - 1 - j] = temp;
        }
    }

    // 7. Print the Final Matrix
    printf("\nFinal Transformed Matrix (Horizontal Flip using %d threads):\n", num_threads);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < col; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }


    return 0;
}