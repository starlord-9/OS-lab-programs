#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_PROCESSES 10

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int start_time;
    bool started;
};

// Function to find the highest priority process (smallest priority number)
int findHighestPriorityProcess(struct Process processes[], int n, int current_time, bool preemptive) {
    int highest_priority_index = -1;
    int highest_priority = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time <= current_time &&
            (preemptive ? processes[i].remaining_time > 0 : processes[i].completion_time == 0) &&
            processes[i].priority < highest_priority) {
            highest_priority = processes[i].priority;
            highest_priority_index = i;
        }
    }
    return highest_priority_index;
}

// Function to simulate Priority Preemptive Scheduling
void priorityPreemptiveScheduling(struct Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    float total_completion_time = 0;
    int gantt_chart[100];
    int gantt_chart_index = 0;

    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].started = false;
    }

    while (completed < n) {
        int highest_priority_index = findHighestPriorityProcess(processes, n, current_time, true);
        if (highest_priority_index == -1) {
            current_time++;
        } else {
            if (!processes[highest_priority_index].started) {
                processes[highest_priority_index].start_time = current_time;
                processes[highest_priority_index].started = true;
            }

            processes[highest_priority_index].remaining_time--;
            gantt_chart[gantt_chart_index++] = processes[highest_priority_index].pid;
            current_time++;

            if (processes[highest_priority_index].remaining_time == 0) {
                processes[highest_priority_index].completion_time = current_time;
                processes[highest_priority_index].turnaround_time =
                    processes[highest_priority_index].completion_time - processes[highest_priority_index].arrival_time;
                processes[highest_priority_index].waiting_time =
                    processes[highest_priority_index].turnaround_time - processes[highest_priority_index].burst_time;

                total_waiting_time += processes[highest_priority_index].waiting_time;
                total_turnaround_time += processes[highest_priority_index].turnaround_time;
                total_completion_time += processes[highest_priority_index].completion_time;
                completed++;
            }
        }
    }

    printf("\nPriority Preemptive Scheduling:\n");
    printf("Process\tArrival Time\tBurst Time\tPriority\tCompletion Time\tTurnaround Time\tWaiting Time\tResponse Time\n");
    for (int i = 0; i < n; i++) {
        int response_time = processes[i].start_time - processes[i].arrival_time;
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
               processes[i].priority, processes[i].completion_time,
               processes[i].turnaround_time, processes[i].waiting_time, response_time);
    }

    printf("\nAverage Completion Time: %.2f\n", total_completion_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);

    printf("\nGantt Chart (Preemptive):\n");
    for (int i = 0; i < gantt_chart_index; i++) {
        printf("P%d ", gantt_chart[i]);
    }
    printf("\n");
}

// Function to simulate Priority Non-Preemptive Scheduling
void priorityNonPreemptiveScheduling(struct Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    float total_completion_time = 0;
    int gantt_chart[100];
    int gantt_chart_index = 0;

    for (int i = 0; i < n; i++) {
        processes[i].completion_time = 0;
    }

    while (completed < n) {
        int highest_priority_index = findHighestPriorityProcess(processes, n, current_time, false);
        if (highest_priority_index == -1) {
            current_time++;
        } else {
            gantt_chart[gantt_chart_index++] = processes[highest_priority_index].pid;
            processes[highest_priority_index].completion_time = current_time + processes[highest_priority_index].burst_time;
            processes[highest_priority_index].turnaround_time =
                processes[highest_priority_index].completion_time - processes[highest_priority_index].arrival_time;
            processes[highest_priority_index].waiting_time =
                processes[highest_priority_index].turnaround_time - processes[highest_priority_index].burst_time;

            current_time = processes[highest_priority_index].completion_time;

            total_waiting_time += processes[highest_priority_index].waiting_time;
            total_turnaround_time += processes[highest_priority_index].turnaround_time;
            total_completion_time += processes[highest_priority_index].completion_time;
            completed++;
        }
    }

    printf("\nPriority Non-Preemptive Scheduling:\n");
    printf("Process\tArrival Time\tBurst Time\tPriority\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
               processes[i].priority, processes[i].completion_time,
               processes[i].turnaround_time, processes[i].waiting_time);
    }

    printf("\nAverage Completion Time: %.2f\n", total_completion_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);

    printf("\nGantt Chart (Non-Preemptive):\n");
    for (int i = 0; i < gantt_chart_index; i++) {
        printf("P%d ", gantt_chart[i]);
    }
    printf("\n");
}

int main() {
    int n;
    printf("Enter the total number of processes: ");
    scanf("%d", &n);
    if (n > MAX_PROCESSES) {
        printf("Number of processes exceeds maximum limit of %d.\n", MAX_PROCESSES);
        return 1;
    }

    struct Process processes[MAX_PROCESSES];
    struct Process processes_copy[MAX_PROCESSES];

    printf("Enter Arrival Time, Burst Time, and Priority for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        printf("Priority: ");
        scanf("%d", &processes[i].priority);
        processes[i].pid = i + 1;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].started = false;
        processes_copy[i] = processes[i];
    }

    priorityPreemptiveScheduling(processes, n);
    priorityNonPreemptiveScheduling(processes_copy, n);

    return 0;
}
