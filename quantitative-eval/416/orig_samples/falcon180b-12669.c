//Falcon-180B DATASET v1.0 Category: Traffic Light Controller ; Style: rigorous
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_LIGHTS 4
#define NUM_THREADS 4
#define RED_LIGHT 0
#define YELLOW_LIGHT 1
#define GREEN_LIGHT 2
#define PEDESTRIAN_LIGHT 3

sem_t light_sem[NUM_LIGHTS];
int light_state[NUM_LIGHTS];

void init_lights() {
    for (int i = 0; i < NUM_LIGHTS; i++) {
        sem_init(&light_sem[i], 0, 0);
        light_state[i] = RED_LIGHT;
    }
}

void *traffic_light(void *arg) {
    int light_num = *(int *) arg;
    while (1) {
        int next_state = (light_state[light_num] + 1) % NUM_LIGHTS;
        if (next_state == RED_LIGHT) {
            printf("Light %d is now red\n", light_num);
        } else if (next_state == YELLOW_LIGHT) {
            printf("Light %d is now yellow\n", light_num);
        } else if (next_state == GREEN_LIGHT) {
            printf("Light %d is now green\n", light_num);
        } else if (next_state == PEDESTRIAN_LIGHT) {
            printf("Light %d is now pedestrian\n", light_num);
        }
        sem_wait(&light_sem[light_num]);
        sem_post(&light_sem[next_state]);
        sleep(1);
    }
    return NULL;
}

int main() {
    init_lights();
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, traffic_light, &i);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}