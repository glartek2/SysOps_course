#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    char *foreground;
    char *background;
    int start_row;
    int end_row;
    int grid_width;
} ThreadData;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_work(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    for (int row = data->start_row; row < data->end_row; ++row) {
        for (int col = 0; col < data->grid_width; ++col) {
            int index = row * data->grid_width + col;
            data->background[index] = is_alive(row, col, data->foreground);
            data->foreground[index] = is_alive(row, col, data->background);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        printf("Number of threads must be a positive integer\n");
        return 1;
    }

    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr();

    int grid_width = 30;
    int grid_height = 30;
    char *foreground = create_grid(grid_width, grid_height);
    char *background = create_grid(grid_width, grid_height);

    init_grid(foreground);

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    int rows_per_thread = grid_height / num_threads;
    int remaining_rows = grid_height % num_threads;
    int start_row = 0;

    for (int i = 0; i < num_threads; ++i) {
        int end_row = start_row + rows_per_thread;
        if (i == num_threads - 1) {
            end_row += remaining_rows;
        }

        thread_data[i].foreground = foreground;
        thread_data[i].background = background;
        thread_data[i].start_row = start_row;
        thread_data[i].end_row = end_row;
        thread_data[i].grid_width = grid_width;

        pthread_create(&threads[i], NULL, thread_work, (void *)&thread_data[i]);

        start_row = end_row;
    }

    while (true) {
        draw_grid(foreground);
        usleep(500 * 1000);

        for (int i = 0; i < num_threads; ++i) {
            pthread_join(threads[i], NULL);
        }

        char *tmp = foreground;
        foreground = background;
        background = tmp;

        for (int i = 0; i < num_threads; ++i) {
            pthread_create(&threads[i], NULL, thread_work, (void *)&thread_data[i]);
        }
    }

    endwin();
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}
