//FormAI DATASET v1.0 Category: Maze Route Finder ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_NODES 100
#define MAX_THREADS 10

typedef struct {
    int x, y, val;
} Node;

typedef struct {
    int idx;
    int n;
    Node *nodes[MAX_NODES];
} ThreadData;

// Global variables
int maze[MAX_NODES][MAX_NODES];
int n, m;
bool visited[MAX_NODES][MAX_NODES] = { false };
Node *path[MAX_NODES * MAX_NODES];
int path_len = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *dfs(void *arg) {
    ThreadData *data = (ThreadData *) arg;

    int idx = data->idx;
    int start = idx * (data->n / MAX_THREADS);
    int end = (idx + 1) * (data->n / MAX_THREADS);

    if (idx == MAX_THREADS - 1) {
        end = data->n;
    }

    for (int i = start; i < end; i++) {
        for (int j = 0; j < m; j++) {
            if (maze[i][j] == 0 && !visited[i][j]) {
                visited[i][j] = true;
                path[path_len++] = data->nodes[i * m + j];

                if (i == n - 1 && j == m - 1) {
                    // We have reached the end of the maze
                    return NULL;
                }

                dfs(arg);

                if (path_len > 0) {
                    path_len--;
                }
                visited[i][j] = false;
            }
        }
    }

    return NULL;
}

void inputMaze() {
    scanf("%i %i", &n, &m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int val;
            scanf("%i", &val);
            maze[i][j] = val;
        }
    }
}

void createThreads(Node *nodes[MAX_NODES], int n) {
    pthread_t threads[MAX_THREADS];
    ThreadData datas[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++) {
        datas[i].n = n;
        datas[i].idx = i;
        for (int j = 0; j < n; j++) {
            datas[i].nodes[j] = nodes[j];
        }
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, dfs, &datas[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

void printPath() {
    printf("Path: ");
    for (int i = 0; i < path_len; i++) {
        printf("(%d,%d)", path[i]->x, path[i]->y);
        if (i < path_len - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

int main() {
    inputMaze();

    Node nodes[MAX_NODES * MAX_NODES];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            nodes[i * m + j].x = i;
            nodes[i * m + j].y = j;
            nodes[i * m + j].val = maze[i][j];
        }
    }

    createThreads(nodes, n * m);

    if (path_len > 0) {
        printPath();
    } else {
        printf("No path found\n");
    }

    return 0;
}