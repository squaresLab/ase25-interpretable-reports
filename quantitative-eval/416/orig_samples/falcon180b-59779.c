//Falcon-180B DATASET v1.0 Category: Pathfinding algorithms ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_ROW 10
#define MAX_COL 10

int grid[MAX_ROW][MAX_COL];
int start_row, start_col, end_row, end_col, num_threads;
pthread_mutex_t mutex;

void *runner(void *param);

int main() {
    int i, j;

    for (i = 0; i < MAX_ROW; i++) {
        for (j = 0; j < MAX_COL; j++) {
            grid[i][j] = 1;
        }
    }

    grid[0][0] = 0;
    grid[9][9] = 0;

    start_row = 0;
    start_col = 0;
    end_row = 9;
    end_col = 9;

    num_threads = 4;

    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[num_threads];

    for (i = 0; i < num_threads; i++) {
        int row = start_row + i * (end_row - start_row) / num_threads;
        int col = start_col + i * (end_col - start_col) / num_threads;

        pthread_create(&threads[i], NULL, runner, (void *)&col);
    }

    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void *runner(void *param) {
    int col = *(int *)param;
    int row = 0;

    while (row < 9) {
        if (grid[row][col] == 1) {
            pthread_mutex_lock(&mutex);
            grid[row][col] = 0;
            pthread_mutex_unlock(&mutex);
            row++;
        } else {
            row++;
        }
    }

    return NULL;
}