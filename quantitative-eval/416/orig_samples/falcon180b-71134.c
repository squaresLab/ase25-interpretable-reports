//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: mathematical
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define NUM_LIGHTS 4
#define NUM_SENSORS 2
#define SENSOR_THRESHOLD 100
#define LIGHT_MIN 0
#define LIGHT_MAX 100
#define LIGHT_INCREMENT 5
#define LIGHT_DECREMENT 2
#define LIGHT_DELAY 1000
#define SENSOR_DELAY 500

int light_values[NUM_LIGHTS] = {0};
int sensor_values[NUM_SENSORS] = {0};
pthread_t light_threads[NUM_LIGHTS];
pthread_t sensor_threads[NUM_SENSORS];

void* light_thread(void* arg) {
    int light_number = *((int*) arg);
    while (1) {
        if (light_values[light_number] < LIGHT_MAX) {
            light_values[light_number] += LIGHT_INCREMENT;
        } else {
            light_values[light_number] = LIGHT_MAX;
        }
        usleep(LIGHT_DELAY);
    }
    return NULL;
}

void* sensor_thread(void* arg) {
    int sensor_number = *((int*) arg);
    while (1) {
        sensor_values[sensor_number] = rand() % SENSOR_THRESHOLD;
        usleep(SENSOR_DELAY);
    }
    return NULL;
}

int main() {
    srand(time(0));
    for (int i = 0; i < NUM_LIGHTS; i++) {
        pthread_create(&light_threads[i], NULL, light_thread, &i);
    }
    for (int i = 0; i < NUM_SENSORS; i++) {
        pthread_create(&sensor_threads[i], NULL, sensor_thread, &i);
    }
    while (1) {
        for (int i = 0; i < NUM_LIGHTS; i++) {
            printf("Light %d: %d\n", i+1, light_values[i]);
        }
        printf("\n");
        for (int i = 0; i < NUM_SENSORS; i++) {
            printf("Sensor %d: %d\n", i+1, sensor_values[i]);
        }
        printf("\n");
        fflush(stdout);
        usleep(100000);
    }
    return 0;
}