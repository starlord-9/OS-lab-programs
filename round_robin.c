#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10

struct Process {
    int pid;
    int burst_time;
    int arrival_time;
    int remaining_time;
    int turnaround_time;
    int waiting_time;
    int completion_time;
};

void round_robin(struct Process proc[], int n, int quantum) {
    int current_time = 0;
    int completed_processes = 0;

    while (completed_processes < n) {
        bool process_found = false;
        for (int i = 0; i < n; i++) {
            if (proc[i].remaining_time > 0 && proc[i].arrival_time <= current_time) {
                process_found = true;
                if (proc[i].remaining_time > quantum) {
                    current_time += quantum;
                    proc[i].remaining_time -= quantum;
                } else {
                    current_time += proc[i].remaining_time;
                    proc[i].completion_time = current_time;
                    proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
                    proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
                    proc[i].remaining_time = 0;
                    completed_processes++;
                }
            }
        }
        if (!process_found) {
            current_time++;
        }
    }

    // Print the results
    printf("\nPID\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    float total_completion_time = 0, total_turnaround_time = 0, total_waiting_time = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].arrival_time,
               proc[i].burst_time, proc[i].completion_time, proc[i].turnaround_time, proc[i].waiting_time);
        total_completion_time += proc[i].completion_time;
        total_turnaround_time += proc[i].turnaround_time;
        total_waiting_time += proc[i].waiting_time;
    }

    // Calculate and display averages
    printf("\nAverage Completion Time: %.2f\n", total_completion_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
}

int main() {
    int n, quantum;

    printf("Enter the total number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);
    if (n > MAX_PROCESSES) {
        printf("Number of processes exceeds maximum limit.\n");
        return 1;
    }

    struct Process proc[MAX_PROCESSES];

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &proc[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &proc[i].burst_time);
        proc[i].pid = i + 1;
        proc[i].remaining_time = proc[i].burst_time; // Initialize remaining time
        proc[i].turnaround_time = 0; // Initialize turnaround time
        proc[i].waiting_time = 0; // Initialize waiting time
        proc[i].completion_time = 0; // Initialize completion time
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);

    round_robin(proc, n, quantum);

    return 0;
}
