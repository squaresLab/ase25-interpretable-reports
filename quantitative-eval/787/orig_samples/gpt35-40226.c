//FormAI DATASET v1.0 Category: Smart home automation ; Style: dynamic
#include <stdio.h>
#include <string.h>

#define MAX_ROOMS 5
#define MAX_DEVICES 10

struct device {
    char* name;
    int state;
};

struct room {
    char* name;
    struct device devices[MAX_DEVICES];
    int n_devices;
};

struct house {
    struct room rooms[MAX_ROOMS];
    int n_rooms;
};

void print_house(struct house h) {
    for (int i = 0; i < h.n_rooms; i++) {
        printf("%s:\n", h.rooms[i].name);
        for (int j = 0; j < h.rooms[i].n_devices; j++) {
            printf("\t%s: %s\n", h.rooms[i].devices[j].name, h.rooms[i].devices[j].state ? "ON" : "OFF");
        }
    }
}

void toggle_device(struct house* h, char* room_name, char* device_name) {
    for (int i = 0; i < h->n_rooms; i++) {
        if (strcmp(h->rooms[i].name, room_name) == 0) {
            for (int j = 0; j < h->rooms[i].n_devices; j++) {
                if (strcmp(h->rooms[i].devices[j].name, device_name) == 0) {
                    h->rooms[i].devices[j].state = !h->rooms[i].devices[j].state;
                    return;
                }
            }
        }
    }
}

int main() {
    struct house my_house = {
        .rooms = {
            {
                .name = "Living Room",
                .devices = {
                    {
                        .name = "TV",
                        .state = 0
                    },
                    {
                        .name = "Lamp",
                        .state = 0
                    }
                },
                .n_devices = 2
            },
            {
                .name = "Bedroom",
                .devices = {
                    {
                        .name = "Bed Lamp",
                        .state = 0
                    },
                    {
                        .name = "Alarm",
                        .state = 0
                    }
                },
                .n_devices = 2
            }
        }, 
        .n_rooms = 2
    };

    printf("Initial state of the house:\n");
    print_house(my_house);

    printf("\nToggling TV in Living Room...\n");
    toggle_device(&my_house, "Living Room", "TV");
    print_house(my_house);

    printf("\nToggling Alarm in Bedroom...\n");
    toggle_device(&my_house, "Bedroom", "Alarm");
    print_house(my_house);

    return 0;
}