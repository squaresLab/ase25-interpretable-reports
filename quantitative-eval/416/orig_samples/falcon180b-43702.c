//Falcon-180B DATASET v1.0 Category: Haunted House Simulator ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_ROOMS 4

struct room {
    char *name;
    int num_ghosts;
};

struct ghost {
    char *name;
    int room_index;
};

struct room rooms[NUM_ROOMS] = {
    {"Living Room", 0},
    {"Kitchen", 0},
    {"Bedroom", 0},
    {"Bathroom", 0}
};

struct ghost ghosts[NUM_ROOMS] = {
    {"Living Room Ghost", 0},
    {"Kitchen Ghost", 1},
    {"Bedroom Ghost", 2},
    {"Bathroom Ghost", 3}
};

void *ghost_thread(void *arg) {
    int index = *(int *)arg;
    struct ghost *ghost = &ghosts[index];

    printf("Ghost %s is haunting the %s.\n", ghost->name, rooms[ghost->room_index].name);

    for (int i = 0; i < 5; i++) {
        printf("Boo! ");
        sleep(1);
    }

    pthread_exit(0);
}

int main() {
    srand(time(0));

    for (int i = 0; i < NUM_ROOMS; i++) {
        rooms[i].num_ghosts = rand() % 3 + 1;
    }

    pthread_t threads[NUM_ROOMS];

    for (int i = 0; i < NUM_ROOMS; i++) {
        pthread_create(&threads[i], NULL, ghost_thread, &i);
    }

    for (int i = 0; i < NUM_ROOMS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}