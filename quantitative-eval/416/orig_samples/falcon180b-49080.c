//Falcon-180B DATASET v1.0 Category: Haunted House Simulator ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_ROOMS 5
#define NUM_THREADS 10

struct room {
    int id;
    char *name;
    char *description;
};

struct room haunted_house[NUM_ROOMS] = {
    {1, "Foyer", "You enter the haunted house through the creaky front door and find yourself in the dimly lit foyer. The air is thick with dust and cobwebs hang from the ceiling."},
    {2, "Living Room", "The living room is filled with antique furniture covered in sheets. A fireplace crackles in the corner, casting eerie shadows on the walls."},
    {3, "Kitchen", "The kitchen is dark and musty. Pots and pans hang from the ceiling, swaying slightly as if moved by an unseen force."},
    {4, "Library", "The library is lined with bookshelves that stretch to the ceiling. A single candle flickers on a desk, illuminating an old tome bound in human skin."},
    {5, "Basement", "The basement stairs groan under your weight as you descend into the darkness. The only light comes from a single bare bulb swinging overhead."}
};

void *haunt_room(void *arg) {
    int room_id = *(int *) arg;
    printf("You enter room %d...\n", room_id);
    sleep(1);
    printf("A chill runs down your spine as you feel a ghostly presence.\n");
    sleep(2);
    printf("The temperature drops suddenly, and your breath fogs in the air.\n");
    sleep(1);
    printf("You hear whispers echoing through the room, but you can't make out what they're saying.\n");
    sleep(2);
    printf("Suddenly, a gust of wind blows out your flashlight, plunging you into darkness.\n");
    sleep(1);
    printf("As your eyes adjust, you see a spectral figure materialize before you.\n");
    sleep(2);
    printf("It reaches out with icy fingers, and you feel a shiver run through your entire body.\n");
    sleep(1);
    printf("Just when you think you can't take it anymore, the ghostly apparition vanishes, leaving you alone in the darkness.\n");
    sleep(2);
    printf("You quickly exit the room, heart pounding in your chest.\n");
    pthread_exit(0);
}

int main() {
    srand(time(NULL));

    int num_threads = NUM_THREADS;
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < num_threads; i++) {
        int room_id = rand() % NUM_ROOMS;
        pthread_create(&threads[i], NULL, haunt_room, &room_id);
        sleep(1);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        sleep(1);
    }

    return 0;
}