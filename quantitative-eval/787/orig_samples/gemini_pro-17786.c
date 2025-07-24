//GEMINI-pro DATASET v1.0 Category: Traffic Flow Simulation ; Style: innovative
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the traffic light states
#define RED 0
#define YELLOW 1
#define GREEN 2

// Define the road segments
#define NORTHBOUND 0
#define EASTBOUND 1
#define SOUTHBOUND 2
#define WESTBOUND 3

// Define the car types
#define CAR 0
#define TRUCK 1
#define BUS 2

// Define the road lengths
#define ROAD_LENGTH 100

// Define the traffic light durations
#define RED_DURATION 10
#define YELLOW_DURATION 5
#define GREEN_DURATION 15

// Create a struct to represent a car
typedef struct car {
    int type; // The type of car (car, truck, bus)
    int speed; // The speed of the car
    int position; // The position of the car on the road
} car_t;

// Create a struct to represent a road segment
typedef struct road_segment {
    int length; // The length of the road segment
    int num_cars; // The number of cars on the road segment
    car_t *cars; // An array of cars on the road segment
} road_segment_t;

// Create a struct to represent a traffic light
typedef struct traffic_light {
    int state; // The state of the traffic light (red, yellow, green)
    int duration; // The duration of the traffic light state
} traffic_light_t;

// Create a struct to represent the traffic flow simulation
typedef struct traffic_flow_simulation {
    road_segment_t road_segments[4]; // An array of road segments
    traffic_light_t traffic_lights[4]; // An array of traffic lights
    int num_cars; // The number of cars in the simulation
} traffic_flow_simulation_t;

// Create a function to initialize the traffic flow simulation
void init_simulation(traffic_flow_simulation_t *sim) {
    // Initialize the road segments
    for (int i = 0; i < 4; i++) {
        sim->road_segments[i].length = ROAD_LENGTH;
        sim->road_segments[i].num_cars = 0;
        sim->road_segments[i].cars = NULL;
    }

    // Initialize the traffic lights
    for (int i = 0; i < 4; i++) {
        sim->traffic_lights[i].state = RED;
        sim->traffic_lights[i].duration = RED_DURATION;
    }

    // Initialize the cars
    sim->num_cars = 0;
}

// Create a function to update the traffic flow simulation
void update_simulation(traffic_flow_simulation_t *sim) {
    // Update the traffic lights
    for (int i = 0; i < 4; i++) {
        // Decrement the duration of the traffic light state
        sim->traffic_lights[i].duration--;

        // If the duration of the traffic light state has reached 0, change the state of the traffic light
        if (sim->traffic_lights[i].duration == 0) {
            switch (sim->traffic_lights[i].state) {
                case RED:
                    sim->traffic_lights[i].state = GREEN;
                    sim->traffic_lights[i].duration = GREEN_DURATION;
                    break;
                case GREEN:
                    sim->traffic_lights[i].state = YELLOW;
                    sim->traffic_lights[i].duration = YELLOW_DURATION;
                    break;
                case YELLOW:
                    sim->traffic_lights[i].state = RED;
                    sim->traffic_lights[i].duration = RED_DURATION;
                    break;
            }
        }
    }

    // Update the cars
    for (int i = 0; i < 4; i++) {
        // Loop through the cars on the road segment
        for (int j = 0; j < sim->road_segments[i].num_cars; j++) {
            // Get the car
            car_t *car = &sim->road_segments[i].cars[j];

            // If the car is at the end of the road segment, remove the car from the road segment
            if (car->position >= sim->road_segments[i].length) {
                sim->road_segments[i].num_cars--;
                for (int k = j; k < sim->road_segments[i].num_cars; k++) {
                    sim->road_segments[i].cars[k] = sim->road_segments[i].cars[k + 1];
                }
                j--;
                continue;
            }

            // If the car is stopped by a red traffic light, do not update the car's position
            if (sim->traffic_lights[i].state == RED && car->position < sim->traffic_lights[i].duration) {
                continue;
            }

            // Update the car's position
            car->position += car->speed;
        }
    }
}

// Create a function to draw the traffic flow simulation
void draw_simulation(traffic_flow_simulation_t *sim) {
    // Clear the screen
    printf("\033[2J");

    // Draw the road segments
    for (int i = 0; i < 4; i++) {
        // Draw the road segment
        for (int j = 0; j < sim->road_segments[i].length; j++) {
            // If there is a car on the road segment, draw the car
            if (sim->road_segments[i].num_cars > 0) {
                for (int k = 0; k < sim->road_segments[i].num_cars; k++) {
                    // Get the car
                    car_t *car = &sim->road_segments[i].cars[k];

                    // If the car is at the current position, draw the car
                    if (car->position == j) {
                        switch (car->type) {
                            case CAR:
                                printf("C");
                                break;
                            case TRUCK:
                                printf("T");
                                break;
                            case BUS:
                                printf("B");
                                break;
                        }
                    }
                }
            } else {
                printf(" ");
            }
        }

        // Draw the traffic light
        switch (sim->traffic_lights[i].state) {
            case RED:
                printf("R");
                break;
            case YELLOW:
                printf("Y");
                break;
            case GREEN:
                printf("G");
                break;
        }

        // Draw a newline character
        printf("\n");
    }
}

// Create a function to run the traffic flow simulation
void run_simulation(traffic_flow_simulation_t *sim) {
    // Initialize the random number generator
    srand(time(NULL));

    // Create a car
    car_t car;
    car.type = rand() % 3;
    car.speed = rand() % 5 + 1;
    car.position = 0;

    // Add the car to the road segment
    sim->road_segments[NORTHBOUND].cars = realloc(sim->road_segments[NORTHBOUND].cars, (sim->road_segments[NORTHBOUND].num_cars + 1) * sizeof(car_t));
    sim->road_segments[NORTHBOUND].cars[sim->road_segments[NORTHBOUND].num_cars] = car;
    sim->road_segments[NORTHBOUND].num_cars++;

    // Run the simulation loop
    while (1) {
        // Update the simulation
        update_simulation(sim);

        // Draw the simulation
        draw_simulation(sim);

        // Sleep for 100 milliseconds
        usleep(100000);
    }
}

// Main function
int main() {
    // Create a traffic flow simulation
    traffic_flow_simulation_t sim;

    // Initialize the traffic flow simulation
    init_simulation(&sim);

    // Run the traffic flow simulation
    run_simulation(&sim);

    return 0;
}