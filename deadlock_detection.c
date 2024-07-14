#include <stdio.h>
#include <stdbool.h>

void deadlockDetection(int P, int R, int processes[], int avail[], int alloc[][R], int request[][R]) {
    int work[R];
    bool finish[P];

    // Initialize work with available resources
    for (int i = 0; i < R; i++)
        work[i] = avail[i];

    // Initialize finish for all processes as false
    for (int i = 0; i < P; i++)
        finish[i] = false;

    // Find an unfinished process with its requests less than or equal to work
    bool found;
    do {
        found = false;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (request[i][j] > work[j])
                        break;
                }
                if (j == R) { // If all requests of process i can be satisfied
                    for (int k = 0; k < R; k++)
                        work[k] += alloc[i][k];
                    finish[i] = true;
                    found = true;
                }
            }
        }
    } while (found);

    // Check if any process is not finished
    bool deadlock = false;
    for (int i = 0; i < P; i++) {
        if (!finish[i]) {
            deadlock = true;
            printf("Process %d is in deadlock.\n", i);
        }
    }
    if (!deadlock)
        printf("No deadlock detected.\n");
}

int main() {
    int P, R;
    printf("Enter the number of processes: ");
    scanf("%d", &P);
    printf("Enter the number of resource types: ");
    scanf("%d", &R);

    int processes[P];
    int avail[R];
    int alloc[P][R];
    int request[P][R];

    for (int i = 0; i < P; i++)
        processes[i] = i;

    printf("Enter the available resources (A B C ...): ");
    for (int i = 0; i < R; i++)
        scanf("%d", &avail[i]);

    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < P; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < R; j++)
            scanf("%d", &alloc[i][j]);
    }

    printf("Enter the request matrix:\n");
    for (int i = 0; i < P; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < R; j++)
            scanf("%d", &request[i][j]);
    }

    deadlockDetection(P, R, processes, avail, alloc, request);

    return 0;
}
