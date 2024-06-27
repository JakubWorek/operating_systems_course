#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>
#include <sched.h>

const int grid_width = 30;
const int grid_height = 30;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
char *foreground_global;
char *background_global;

char *create_grid(){
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid){
    free(grid);
}

void draw_grid(char *grid){
    for (int i = 0; i < grid_height; ++i){
        for (int j = 0; j < grid_width; ++j){
            if (grid[i * grid_width + j]){
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else{
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid){
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid){
    int count = 0;
    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++){
            if (i == 0 && j == 0){
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width){
                continue;
            }
            if (grid[grid_width * r + c]){
                count++;
            }
        }
    }

    if (grid[row * grid_width + col]){
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else{
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char *src, char *dst, int start_row, int end_row){
    for (int i = start_row; i < end_row; ++i){
        for (int j = 0; j < grid_width; ++j){
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

void *thread_func(void *arg){
    int thread_id = *((int *)arg);
    int num_threads = *((int *)arg + 1);

    while (true){
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);

        // Update the background grid
        update_grid(foreground_global, background_global, thread_id * grid_height / num_threads, (thread_id + 1) * grid_height / num_threads);

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond); // Signal the main thread
    }

    return NULL;
}

char *get_foreground(){
    char *tmp;
    pthread_mutex_lock(&mutex);
    tmp = foreground_global;
    pthread_mutex_unlock(&mutex);
    return tmp;
}

char *get_background(){
    char *tmp;
    pthread_mutex_lock(&mutex);
    tmp = background_global;
    pthread_mutex_unlock(&mutex);
    return tmp;
}

void set_foreground(char *grid){
    pthread_mutex_lock(&mutex);
    foreground_global = grid;
    pthread_mutex_unlock(&mutex);
}

void set_background(char *grid){
    pthread_mutex_lock(&mutex);
    background_global = grid;
    pthread_mutex_unlock(&mutex);
}