#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PHILOSOPHERS 10

int chopstick[MAX_PHILOSOPHERS];
int num_philosophers;

void semaphore_wait(int *semaphore) {
    while (*semaphore <= 0) {
        // busy wait
    }
    (*semaphore)--;
}

void semaphore_signal(int *semaphore) {
    (*semaphore)++;
}

void* philosopher(void *arg) {
    int philosopher_id = *((int *)arg);
    int left_fork = philosopher_id;
    int right_fork = (philosopher_id + 1) % num_philosophers;

    while (1) {
        printf("Philosopher %d is thinking.\n", philosopher_id);
        sleep(1);

        semaphore_wait(&chopstick[left_fork]);
        semaphore_wait(&chopstick[right_fork]);

        printf("Philosopher %d is eating.\n", philosopher_id);
        sleep(1);

        semaphore_signal(&chopstick[left_fork]);
        semaphore_signal(&chopstick[right_fork]);
    }

    return NULL;
}

int main() {
    pthread_t philosophers[MAX_PHILOSOPHERS];
    int philosopher_ids[MAX_PHILOSOPHERS];

    printf("Enter the number of philosophers (max %d): ", MAX_PHILOSOPHERS);
    scanf("%d", &num_philosophers);

    if (num_philosophers < 2 || num_philosophers > MAX_PHILOSOPHERS) {
        printf("Invalid number of philosophers. Exiting.\n");
        return 1;
    }

    for (int i = 0; i < num_philosophers; ++i) {
        chopstick[i] = 1;
    }

    for (int i = 0; i < num_philosophers; ++i) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    for (int i = 0; i < num_philosophers; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
