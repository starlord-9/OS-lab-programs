#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_PROCESSES 10

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int deadline; // For EDF scheduling
    int period; // For RMS scheduling
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int priority; // For RMS scheduling
    bool is_completed;
};

void rate_monotonic_scheduling(struct Process proc[], int n) {
    int current_time = 0;
    int completed = 0;

    while (completed < n) {
        int highest_priority_index = -1;
        int highest_priority = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!proc[i].is_completed && proc[i].arrival_time <= current_time &&
                proc[i].priority < highest_priority) {
                highest_priority = proc[i].priority;
                highest_priority_index = i;
            }
        }

        if (highest_priority_index == -1) {
            current_time++;
        } else {
            proc[highest_priority_index].remaining_time--;
            current_time++;

            if (proc[highest_priority_index].remaining_time == 0) {
                proc[highest_priority_index].completion_time = current_time;
                proc[highest_priority_index].turnaround_time = proc[highest_priority_index].completion_time - proc[highest_priority_index].arrival_time;
                proc[highest_priority_index].waiting_time = proc[highest_priority_index].turnaround_time - proc[highest_priority_index].burst_time;
                proc[highest_priority_index].is_completed = true;
                completed++;
            }
        }
    }
}

void earliest_deadline_first(struct Process proc[], int n) {
    int current_time = 0;
    int completed = 0;

    while (completed < n) {
        int earliest_deadline_index = -1;
        int earliest_deadline = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!proc[i].is_completed && proc[i].arrival_time <= current_time &&
                proc[i].deadline < earliest_deadline) {
                earliest_deadline = proc[i].deadline;
                earliest_deadline_index = i;
            }
        }

        if (earliest_deadline_index == -1) {
            current_time++;
        } else {
            proc[earliest_deadline_index].remaining_time--;
            current_time++;

            if (proc[earliest_deadline_index].remaining_time == 0) {
                proc[earliest_deadline_index].completion_time = current_time;
                proc[earliest_deadline_index].turnaround_time = proc[earliest_deadline_index].completion_time - proc[earliest_deadline_index].arrival_time;
                proc[earliest_deadline_index].waiting_time = proc[earliest_deadline_index].turnaround_time - proc[earliest_deadline_index].burst_time;
                proc[earliest_deadline_index].is_completed = true;
                completed++;
            }
        }
    }
}

void proportional_scheduling(struct Process proc[], int n) {
    int total_burst_time = 0;
    for (int i = 0; i < n; i++) {
        total_burst_time += proc[i].burst_time;
    }

    float time_slice[MAX_PROCESSES];
    for (int i = 0; i < n; i++) {
        time_slice[i] = (float)proc[i].burst_time / total_burst_time;
    }

    int current_time = 0;
    int completed = 0;
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!proc[i].is_completed && proc[i].arrival_time <= current_time) {
                int slice = (int)(time_slice[i] * total_burst_time);
                if (slice > proc[i].remaining_time) {
                    slice = proc[i].remaining_time;
                }

                proc[i].remaining_time -= slice;
                current_time += slice;

                if (proc[i].remaining_time == 0) {
                    proc[i].completion_time = current_time;
                    proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
                    proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
                    proc[i].is_completed = true;
                    completed++;
                }
            }
        }
    }
}

void print_processes(struct Process proc[], int n) {
    printf("PID\tArrival Time\tBurst Time\tDeadline\tPeriod\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t%d\t\t%d\t\t%d\n",
               proc[i].pid, proc[i].arrival_time, proc[i].burst_time,
               proc[i].deadline, proc[i].period, proc[i].completion_time,
               proc[i].turnaround_time, proc[i].waiting_time);
    }
}

int main() {
    struct Process proc[MAX_PROCESSES];
    int n;
    printf("Enter the total number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    printf("Enter process details (arrival_time, burst_time, deadline, period for EDF and RMS; type '0' for proportional scheduling):\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &proc[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &proc[i].burst_time);
        printf("Deadline: ");
        scanf("%d", &proc[i].deadline);
        printf("Period: ");
        scanf("%d", &proc[i].period);
        proc[i].pid = i + 1;
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].completion_time = 0;
        proc[i].turnaround_time = 0;
        proc[i].waiting_time = 0;
        proc[i].is_completed = false;
    }

    printf("\nRate-Monotonic Scheduling:\n");
    for (int i = 0; i < n; i++) {
        proc[i].priority = proc[i].period; // Priority is inverse of period
    }
    rate_monotonic_scheduling(proc, n);
    print_processes(proc, n);

    // Reset the process state for EDF scheduling
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].completion_time = 0;
        proc[i].turnaround_time = 0;
        proc[i].waiting_time = 0;
        proc[i].is_completed = false;
    }

    printf("\nEarliest-Deadline-First Scheduling:\n");
    earliest_deadline_first(proc, n);
    print_processes(proc, n);

    // Reset the process state for Proportional scheduling
    for (int i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].completion_time = 0;
        proc[i].turnaround_time = 0;
        proc[i].waiting_time = 0;
        proc[i].is_completed = false;
    }

    printf("\nProportional Scheduling:\n");
    proportional_scheduling(proc, n);
    print_processes(proc, n);

    return 0;
}
