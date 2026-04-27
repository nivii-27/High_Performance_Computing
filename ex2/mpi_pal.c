#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mpi.h"

int isPalindrome(char str[]) {
    int l = 0;
    int h = strlen(str) - 1;
    while (h > l) {
        // Handle case-insensitivity for palindrome check if needed
        if (tolower(str[l++]) != tolower(str[h--])) return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Status status;
    char message[100];
    char *str1 = "Hello";
    char *str2 = "Malayalam";
    char *str3 = "Pen";
    char *str4 = "Welcome";
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Recv(message, 100, MPI_CHAR, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == 1) {
                for (int j = 0; message[j]; j++) message[j] = toupper(message[j]);
                printf("Master: Rank %d sent (Type 1), Uppercase: %s\n", status.MPI_SOURCE, message);
            }
            else if (status.MPI_TAG == 2) {
                if (isPalindrome(message))
                    printf("Master: Rank %d sent (Type 2), %s is a Palindrome\n", status.MPI_SOURCE, message);
                else
                    printf("Master: Rank %d sent (Type 2), %s is NOT a Palindrome\n", status.MPI_SOURCE, message);
            }
        }
    } else {
        if (rank == 1) {
            MPI_Send(str1, strlen(str1) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        } else if (rank == 2) {
            MPI_Send(str2, strlen(str2) + 1, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
        } else if (rank == 3) {
            MPI_Send(str3, strlen(str3) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        } else if (rank == 4) {
            MPI_Send(str4, strlen(str4) + 1, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}