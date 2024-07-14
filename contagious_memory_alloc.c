#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_BLOCKS 100
#define MAX_PROCESSES 100

typedef struct {
    int size; // Size of the block
    int allocated; // Indicates if the block is allocated (1) or not (0)
    int process_id; // Process ID if allocated, -1 if not allocated
} MemoryBlock;

typedef struct {
    int id; // Process ID
    int size; // Size of the process
} Process;

void worstFit(MemoryBlock blocks[], int m, Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        int max_index = -1;
        int max_size = -1;
        for (int j = 0; j < m; j++) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                if (blocks[j].size > max_size) {
                    max_size = blocks[j].size;
                    max_index = j;
                }
            }
        }
        if (max_index != -1) {
            blocks[max_index].allocated = 1;
            blocks[max_index].process_id = processes[i].id;
            printf("Process %d allocated to Block %d\n", processes[i].id, max_index);
        } else {
            printf("Process %d cannot be allocated\n", processes[i].id);
        }
    }
}

void bestFit(MemoryBlock blocks[], int m, Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        int best_index = -1;
        int best_size = INT_MAX;
        for (int j = 0; j < m; j++) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                if (blocks[j].size < best_size) {
                    best_size = blocks[j].size;
                    best_index = j;
                }
            }
        }
        if (best_index != -1) {
            blocks[best_index].allocated = 1;
            blocks[best_index].process_id = processes[i].id;
            printf("Process %d allocated to Block %d\n", processes[i].id, best_index);
        } else {
            printf("Process %d cannot be allocated\n", processes[i].id);
        }
    }
}

void firstFit(MemoryBlock blocks[], int m, Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        int allocated = 0;
        for (int j = 0; j < m; j++) {
            if (!blocks[j].allocated && blocks[j].size >= processes[i].size) {
                blocks[j].allocated = 1;
                blocks[j].process_id = processes[i].id;
                printf("Process %d allocated to Block %d\n", processes[i].id, j);
                allocated = 1;
                break;
            }
        }
        if (!allocated) {
            printf("Process %d cannot be allocated\n", processes[i].id);
        }
    }
}

void nextFit(MemoryBlock blocks[], int m, Process processes[], int n) {
    int next_index = 0;
    for (int i = 0; i < n; i++) {
        int allocated = 0;
        for (int j = 0; j < m; j++) {
            int index = (next_index + j) % m;
            if (!blocks[index].allocated && blocks[index].size >= processes[i].size) {
                blocks[index].allocated = 1;
                blocks[index].process_id = processes[i].id;
                printf("Process %d allocated to Block %d\n", processes[i].id, index);
                next_index = (index + 1) % m;
                allocated = 1;
                break;
            }
        }
        if (!allocated) {
            printf("Process %d cannot be allocated\n", processes[i].id);
        }
    }
}

int main() {
    int m, n;

    printf("Enter the number of memory blocks: ");
    scanf("%d", &m);
    MemoryBlock blocks[MAX_BLOCKS];

    printf("Enter the size of each memory block:\n");
    for (int i = 0; i < m; i++) {
        blocks[i].allocated = 0;
        blocks[i].process_id = -1;
        printf("Block %d size: ", i);
        scanf("%d", &blocks[i].size);
    }

    printf("Enter the number of processes: ");
    scanf("%d", &n);
    Process processes[MAX_PROCESSES];

    printf("Enter the size of each process:\n");
    for (int i = 0; i < n; i++) {
        processes[i].id = i;
        printf("Process %d size: ", i);
        scanf("%d", &processes[i].size);
    }

    printf("\nWorst Fit Allocation:\n");
    MemoryBlock blocks_copy[MAX_BLOCKS];
    for (int i = 0; i < m; i++)
        blocks_copy[i] = blocks[i];
    worstFit(blocks_copy, m, processes, n);

    printf("\nBest Fit Allocation:\n");
    for (int i = 0; i < m; i++)
        blocks_copy[i] = blocks[i];
    bestFit(blocks_copy, m, processes, n);

    printf("\nFirst Fit Allocation:\n");
    for (int i = 0; i < m; i++)
        blocks_copy[i] = blocks[i];
    firstFit(blocks_copy, m, processes, n);

    printf("\nNext Fit Allocation:\n");
    for (int i = 0; i < m; i++)
        blocks_copy[i] = blocks[i];
    nextFit(blocks_copy, m, processes, n);

    return 0;
}
