#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PROCESSES 10

// Structure to represent a process
struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time; // Remaining burst time for preemptive SJF
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int start_time; // Time when the process starts execution for the first time
    bool started; // To check if the process has started
};

// Function to find the process with the shortest remaining burst time among the arrived processes
int findShortestJob(struct Process processes[], int n, int current_time) {
    int shortest_job_index = -1;
    int shortest_job = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 &&
            processes[i].remaining_time < shortest_job) {
            shortest_job_index = i;
            shortest_job = processes[i].remaining_time;
        }
    }
    return shortest_job_index;
}

// Function to simulate the SJF preemptive scheduling algorithm
void SJF_Preemptive(struct Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    float total_completion_time = 0;
    int gantt_chart[100]; // Array to store process execution order
    int gantt_chart_index = 0;

    while (completed < n) {
        int shortest_job_index = findShortestJob(processes, n, current_time);
        if (shortest_job_index == -1) {
            current_time++; // Move to the next time unit
        } else {
            // Update the start time if the process starts for the first time
            if (!processes[shortest_job_index].started) {
                processes[shortest_job_index].start_time = current_time;
                processes[shortest_job_index].started = true;
            }

            // Execute the shortest job for one time unit
            processes[shortest_job_index].remaining_time--;
            gantt_chart[gantt_chart_index++] = processes[shortest_job_index].pid;
            current_time++;

            if (processes[shortest_job_index].remaining_time == 0) {
                // Update completion time and calculate turnaround time and waiting time
                processes[shortest_job_index].completion_time = current_time;
                processes[shortest_job_index].turnaround_time =
                    processes[shortest_job_index].completion_time - processes[shortest_job_index].arrival_time;
                processes[shortest_job_index].waiting_time =
                    processes[shortest_job_index].turnaround_time - processes[shortest_job_index].burst_time;

                // Accumulate total times
                total_waiting_time += processes[shortest_job_index].waiting_time;
                total_turnaround_time += processes[shortest_job_index].turnaround_time;
                total_completion_time += processes[shortest_job_index].completion_time;
                completed++;
            }
        }
    }

    // Display process details
    printf("\nSJF Preemptive Scheduling:\n");
    printf("Process\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\tResponse Time\n");
    for (int i = 0; i < n; i++) {
        int response_time = processes[i].start_time - processes[i].arrival_time;
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
               processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time, response_time);
    }

    // Calculate and display averages
    printf("\nAverage Completion Time: %.2f\n", total_completion_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);

    // Display Gantt chart
    printf("\nGantt Chart (Preemptive):\n");
    for (int i = 0; i < gantt_chart_index; i++) {
        printf("P%d ", gantt_chart[i]);
    }
    printf("\n");
}

// Function to simulate the SJF non-preemptive scheduling algorithm
void SJF_NonPreemptive(struct Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    float total_completion_time = 0;
    int gantt_chart[100]; // Array to store process execution order
    int gantt_chart_index = 0;

    // Initialize all processes' completion time to 0
    for (int i = 0; i < n; i++) {
        processes[i].completion_time = 0;
    }

    while (completed < n) {
        int min_burst_index = -1;
        int min_burst = INT_MAX;

        // Find the process with the minimum burst time among the arrived processes
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].completion_time == 0 &&
                processes[i].burst_time < min_burst) {
                min_burst_index = i;
                min_burst = processes[i].burst_time;
            }
        }

        if (min_burst_index == -1) {
            current_time++; // Move to the next time unit
        } else {
            // Execute the process with the minimum burst time
            gantt_chart[gantt_chart_index++] = processes[min_burst_index].pid;
            processes[min_burst_index].completion_time = current_time + processes[min_burst_index].burst_time;
            processes[min_burst_index].turnaround_time =
                processes[min_burst_index].completion_time - processes[min_burst_index].arrival_time;
            processes[min_burst_index].waiting_time =
                processes[min_burst_index].turnaround_time - processes[min_burst_index].burst_time;

            // Update current time
            current_time = processes[min_burst_index].completion_time;

            // Accumulate total times
            total_waiting_time += processes[min_burst_index].waiting_time;
            total_turnaround_time += processes[min_burst_index].turnaround_time;
            total_completion_time += processes[min_burst_index].completion_time;
            completed++;
        }
    }

    // Display process details
    printf("\nSJF Non-Preemptive Scheduling:\n");
    printf("Process\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid, processes[i].arrival_time, processes[i].burst_time,
               processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time);
    }

    // Calculate and display averages
    printf("\nAverage Completion Time: %.2f\n", total_completion_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);

    // Display Gantt chart
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

    printf("Enter Arrival Time and Burst Time for each process:\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d:\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].pid = i + 1;
        processes[i].started = false;
        processes_copy[i] = processes[i]; // Copy for non-preemptive SJF
    }

    SJF_Preemptive(processes, n);
    SJF_NonPreemptive(processes_copy, n);

    return 0;
}
