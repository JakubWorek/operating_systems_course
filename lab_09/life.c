#include "grid.h"
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 3// number of threads

typedef struct {
    char *foreground;
    char *background;
    int thread_id;
} ThreadArgs;

void *update_grid_thread(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int start_row = args->thread_id * (30 / NUM_THREADS);
    int end_row = (args->thread_id == NUM_THREADS - 1)? 30 : (args->thread_id + 1) * (30 / NUM_THREADS);

    update_grid(args->foreground, args->background, start_row, end_row);
    return NULL;
}

int main()
{
    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode

    char *foreground = create_grid();
    char *background = create_grid();
    char *tmp;

    init_grid(foreground);

    pthread_t threads[NUM_THREADS];

    while (true)
    {
        draw_grid(foreground);
        usleep(500 * 1000);

        // Step simulation
        for (int i = 0; i < NUM_THREADS; i++)
        {
            ThreadArgs *args = malloc(sizeof(ThreadArgs));
            args->foreground = foreground;
            args->background = background;
            args->thread_id = i;
            pthread_create(&threads[i], NULL, update_grid_thread, (void *)args);
        }

        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(threads[i], NULL);
        }

        tmp = foreground;
        foreground = background;
        background = tmp;
    }

    endwin(); // End curses mode
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}