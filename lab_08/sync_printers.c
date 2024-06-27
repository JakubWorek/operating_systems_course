#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

#define N 5
#define M 3
#define QUEUE_SIZE 10

typedef struct {
    char text[10];
} print_job;

typedef struct {
    print_job jobs[QUEUE_SIZE];
    int head;
    int tail;
    sem_t mutex;
    sem_t slots;
    sem_t items;
} print_queue;

print_queue *queue;

void *user(void *arg) {
    // Generate a print job
    print_job job;
    for (int i = 0; i < 10; i++) {
        job.text[i] = 'a' + rand() % 26;
    }

    // Add the job to the queue
    sem_wait(&queue->slots);
    sem_wait(&queue->mutex);
    queue->jobs[queue->tail] = job;
    queue->tail = (queue->tail + 1) % QUEUE_SIZE;
    sem_post(&queue->mutex);
    sem_post(&queue->items);

    // Wait for a random amount of time
    sleep(rand() % 5);
}

void *printer(void *arg) {
    while (1) {
        // Get a job from the queue
        sem_wait(&queue->items);
        sem_wait(&queue->mutex);
        print_job job = queue->jobs[queue->head];
        queue->head = (queue->head + 1) % QUEUE_SIZE;
        sem_post(&queue->mutex);
        sem_post(&queue->slots);

        // Print the job
        for (int i = 0; i < 10; i++) {
            putchar(job.text[i]);
            fflush(stdout);
            sleep(1);
        }
        putchar('\n');

        // if queue is empty, break the loop
        if (queue->head == queue->tail) {
            break;
        }
    }
}

int main() {
    pthread_t users[N], printers[M];

    // Initialize the print queue
    queue = mmap(NULL, sizeof(print_queue), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(&queue->mutex, 1, 1);
    sem_init(&queue->slots, 1, QUEUE_SIZE);
    sem_init(&queue->items, 1, 0);
    queue->head = 0;
    queue->tail = 0;

    // Create the user and printer threads
    for (int i = 0; i < N; i++) {
        pthread_create(&users[i], NULL, user, NULL);
    }
    for (int i = 0; i < M; i++) {
        pthread_create(&printers[i], NULL, printer, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < N; i++) {
        pthread_join(users[i], NULL);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(printers[i], NULL);
    }

    return 0;
}
