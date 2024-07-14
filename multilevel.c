#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 20

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    char type;
};

void calculate_times(struct Process proc[], int n) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        current_time += proc[i].burst_time;
        proc[i].completion_time = current_time;
        proc[i].turnaround_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].waiting_time = proc[i].turnaround_time - proc[i].burst_time;
    }
}

void print_processes(struct Process proc[], int n) {
    printf("PID\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\tType\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%c\n",
               proc[i].pid, proc[i].arrival_time, proc[i].burst_time,
               proc[i].completion_time, proc[i].turnaround_time,
               proc[i].waiting_time, proc[i].type);
    }
}

int compare_arrival(const void *a, const void *b) {
    return ((struct Process *)a)->arrival_time - ((struct Process *)b)->arrival_time;
}

void compute_averages(struct Process proc[], int n) {
    float total_completion_time = 0, total_turnaround_time = 0, total_waiting_time = 0;
    for (int i = 0; i < n; i++) {
        total_completion_time += proc[i].completion_time;
        total_turnaround_time += proc[i].turnaround_time;
        total_waiting_time += proc[i].waiting_time;
    }
    printf("\nAverage Completion Time: %.2f\n", total_completion_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
}

int main() {
    struct Process processes[MAX_PROCESSES];
    int n, system_count = 0, user_count = 0;

    printf("Enter the total number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);
    if (n > MAX_PROCESSES) {
        printf("Number of processes exceeds maximum limit.\n");
        return 1;
    }

    printf("Enter Process details (Type: 'S' for system, 'U' for user):\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d:\n", i + 1);
        printf("Type (S/U): ");
        scanf(" %c", &processes[i].type);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        processes[i].pid = i + 1;
        if (processes[i].type == 'S') {
            system_count++;
        } else if (processes[i].type == 'U') {
            user_count++;
        }
    }

    struct Process system_processes[MAX_PROCESSES];
    struct Process user_processes[MAX_PROCESSES];
    int sys_index = 0, user_index = 0;

    for (int i = 0; i < n; i++) {
        if (processes[i].type == 'S') {
            system_processes[sys_index++] = processes[i];
        } else if (processes[i].type == 'U') {
            user_processes[user_index++] = processes[i];
        }
    }

    qsort(system_processes, system_count, sizeof(struct Process), compare_arrival);
    qsort(user_processes, user_count, sizeof(struct Process), compare_arrival);

    calculate_times(system_processes, system_count);
    calculate_times(user_processes, user_count);

    printf("\nSystem Processes:\n");
    print_processes(system_processes, system_count);
    compute_averages(system_processes, system_count);

    printf("\nUser Processes:\n");
    print_processes(user_processes, user_count);
    compute_averages(user_processes, user_count);

    return 0;
}
