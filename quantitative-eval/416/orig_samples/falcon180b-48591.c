//Falcon-180B DATASET v1.0 Category: Traffic Light Controller ; Style: systematic
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define RED_LIGHT 1
#define YELLOW_LIGHT 2
#define GREEN_LIGHT 3
#define PEDESTRIAN_LIGHT 4
#define NUM_LIGHTS 4
#define LIGHT_DURATION 10 // seconds

int light_state[NUM_LIGHTS] = {RED_LIGHT, RED_LIGHT, RED_LIGHT, RED_LIGHT};

void *change_light(void *arg) {
    int light_num = *(int *) arg;
    int current_state = light_state[light_num];

    while (1) {
        switch (current_state) {
            case RED_LIGHT:
                printf("Light %d is red\n", light_num);
                current_state = YELLOW_LIGHT;
                break;
            case YELLOW_LIGHT:
                printf("Light %d is yellow\n", light_num);
                current_state = GREEN_LIGHT;
                break;
            case GREEN_LIGHT:
                printf("Light %d is green\n", light_num);
                current_state = RED_LIGHT;
                break;
            case PEDESTRIAN_LIGHT:
                printf("Light %d is pedestrian\n", light_num);
                current_state = RED_LIGHT;
                break;
            default:
                printf("Invalid light state for light %d\n", light_num);
                return NULL;
        }

        sleep(LIGHT_DURATION);
    }

    return NULL;
}

int main() {
    pthread_t thread_id[NUM_LIGHTS];

    for (int i = 0; i < NUM_LIGHTS; i++) {
        int *light_num = &i;
        pthread_create(&thread_id[i], NULL, change_light, light_num);
    }

    for (int i = 0; i < NUM_LIGHTS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    return 0;
}