#pragma once
#include <stdbool.h>
#include <pthread.h>

extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
extern char *foreground_global;
extern char *background_global;

char *create_grid();
void destroy_grid(char *grid);
void draw_grid(char *grid);
void init_grid(char *grid);
bool is_alive(int row, int col, char *grid);
void update_grid(char *src, char *dst, int start_row, int end_row);

void *thread_func(void *arg);

char *get_foreground();
char *get_background();
void set_foreground(char *grid);
void set_background(char *grid);