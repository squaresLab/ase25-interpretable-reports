//FormAI DATASET v1.0 Category: Airport Baggage Handling Simulation ; Style: Dennis Ritchie
#include <stdio.h>
#include <stdlib.h>

#define MAX_BAGGAGE 100 // Maximum number of bags
#define MAX_FLIGHTS 10  // Maximum number of flights
#define MAX_TERMINALS 5 // Maximum number of terminals

// Define the Baggage structure
typedef struct Baggage {
    int id;             // Unique identifier of the baggage
    int flight_number;  // Flight number of the baggage
    int terminal_id;    // ID of the terminal where the baggage will be loaded
    int weight;         // Weight of the baggage in kilograms
} Baggage;

// Define the Terminal structure
typedef struct Terminal {
    int id;             // Unique identifier of the terminal
    int capacity;       // Maximum number of bags that can be loaded to the terminal
    int current_load;   // Current load of the terminal in kilograms
    Baggage *baggage_list[MAX_BAGGAGE]; // List of baggage loaded to the terminal
} Terminal;

// Define the Flight structure
typedef struct Flight {
    int number;         // Unique identifier of the flight
    int capacity;       // Maximum number of bags that can be loaded to the flight
    int current_load;   // Current load of the flight in kilograms
    Terminal *terminal_list[MAX_TERMINALS]; // List of terminals where the flight will stop
} Flight;

// Load the baggage to the given terminal
void load_baggage(Baggage *baggage, Terminal *terminal) {
    if (terminal->current_load + baggage->weight <= terminal->capacity) {
        terminal->baggage_list[terminal->current_load] = baggage;
        terminal->current_load += baggage->weight;
        printf("Baggage %d loaded to Terminal %d, Current Load: %d/%d\n", baggage->id, terminal->id, terminal->current_load, terminal->capacity);
    } else {
        printf("Terminal %d is full for Baggage %d\n", terminal->id, baggage->id);
    }
}

int main() {
    // Initialize the flights
    Flight flight_list[MAX_FLIGHTS];
    flight_list[0].number = 1;
    flight_list[0].capacity = 1000;
    flight_list[0].current_load = 0;

    // Initialize the terminals
    Terminal terminal_list[MAX_TERMINALS];
    terminal_list[0].id = 1;
    terminal_list[0].capacity = 500;
    terminal_list[0].current_load = 0;
    terminal_list[1].id = 2;
    terminal_list[1].capacity = 800;
    terminal_list[1].current_load = 0;

    // Load the baggage to the terminals
    Baggage baggage_list[MAX_BAGGAGE];
    baggage_list[0].id = 1;
    baggage_list[0].flight_number = 1;
    baggage_list[0].terminal_id = 1;
    baggage_list[0].weight = 100;
    baggage_list[1].id = 2;
    baggage_list[1].flight_number = 1;
    baggage_list[1].terminal_id = 2;
    baggage_list[1].weight = 200;
    baggage_list[2].id = 3;
    baggage_list[2].flight_number = 1;
    baggage_list[2].terminal_id = 1;
    baggage_list[2].weight = 300;
    baggage_list[3].id = 4;
    baggage_list[3].flight_number = 1;
    baggage_list[3].terminal_id = 2;
    baggage_list[3].weight = 400;
    baggage_list[4].id = 5;
    baggage_list[4].flight_number = 1;
    baggage_list[4].terminal_id = 1;
    baggage_list[4].weight = 500;
    int num_baggages = 5;
    for (int i = 0; i < num_baggages; i++) {
        load_baggage(&baggage_list[i], &terminal_list[baggage_list[i].terminal_id - 1]);
    }

    // Assign the terminals to the flights
    flight_list[0].terminal_list[0] = &terminal_list[0];
    flight_list[0].terminal_list[1] = &terminal_list[1];

    // Display the flight information
    printf("Flight %d\n", flight_list[0].number);
    printf("Maximum Load: %d\n", flight_list[0].capacity);
    printf("Current Load: %d\n", flight_list[0].current_load);
    printf("Terminals:\n");
    for (int i = 0; i < MAX_TERMINALS; i++) {
        if (flight_list[0].terminal_list[i] != NULL) {
            printf("Terminal %d\n", flight_list[0].terminal_list[i]->id);
            printf("- Maximum Load: %d\n", flight_list[0].terminal_list[i]->capacity);
            printf("- Current Load: %d\n", flight_list[0].terminal_list[i]->current_load);
            printf("- Baggage List:\n");
            for (int j = 0; j < MAX_BAGGAGE; j++) {
                if (flight_list[0].terminal_list[i]->baggage_list[j] != NULL) {
                    printf("  - Baggage %d (%dkg)\n", flight_list[0].terminal_list[i]->baggage_list[j]->id, flight_list[0].terminal_list[i]->baggage_list[j]->weight);
                }
            }
        }
    }

    return 0;
}