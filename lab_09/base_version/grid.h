#pragma once
#include <stdbool.h>

extern const int grid_width;
extern const int grid_height;

char *create_grid();
void destroy_grid(char *grid);
void draw_grid(char *grid);
void init_grid(char *grid);
bool is_alive(int row, int col, char *grid);
void update_grid(char *src, char *dst, int start_row, int end_row);