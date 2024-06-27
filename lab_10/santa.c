#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define NUM_REINDEER 9
#define NUM_DELIVERS 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t reindeer_cond = PTHREAD_COND_INITIALIZER;

int delivers = 0;
int reindeer_counter = 0;

void *santa() {
    while (delivers < NUM_DELIVERS) {
        pthread_mutex_lock(&mutex);
        while (reindeer_counter < NUM_REINDEER) {
            pthread_cond_wait(&santa_cond, &mutex);
        }
        printf("Santa: I'm waking up\n");
        printf("Santa: I'm delivering toys\n");
        sleep(rand() % 3 + 2);
        delivers++;
        reindeer_counter = 0;
        pthread_cond_broadcast(&reindeer_cond);
        printf("Santa: I'm falling asleep\n");
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *reindeer(void *arg) {
    int id = *((int *) arg);
    while (delivers < NUM_DELIVERS) {
        sleep(rand() % 6 + 5);
        pthread_mutex_lock(&mutex);
        reindeer_counter++;
        printf("Reindeer with ID: %d arrived, %d reindeers are waiting for Santa\n", id, reindeer_counter);
        if (reindeer_counter == NUM_REINDEER) {
            printf("Reindeer with ID: %d: I'm waking up Santa\n", id);
            pthread_cond_signal(&santa_cond);
        }
        pthread_cond_wait(&reindeer_cond, &mutex);
        pthread_mutex_unlock(&mutex);
        sleep(2);
        printf("Reindeer with ID: %d: I'm going on vacation\n", id);
    }
    return NULL;
}

int main() {
    pthread_t santa_thread;
    pthread_t reindeer_threads[NUM_REINDEER];
    int reindeer_ids[NUM_REINDEER];

    pthread_create(&santa_thread, NULL, santa, NULL);

    for (int i = 0; i < NUM_REINDEER; i++) {
        reindeer_ids[i] = i + 1;
        pthread_create(&reindeer_threads[i], NULL, reindeer, &reindeer_ids[i]);
    }

    pthread_join(santa_thread, NULL);

    for (int i = 0; i < NUM_REINDEER; i++) {
        pthread_join(reindeer_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&santa_cond);
    pthread_cond_destroy(&reindeer_cond);
}