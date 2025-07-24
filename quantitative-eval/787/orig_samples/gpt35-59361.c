//FormAI DATASET v1.0 Category: Game ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NUM_ROOMS 20 // number of rooms on the map
#define MAX_ITEMS 5 // maximum number of items per room
#define MAX_PLAYERS 10 // maximum number of players in the game

// item struct to represent items on map
typedef struct Item {
    char* name; // name of item
    int value; // value of item
} Item;

// room struct to represent rooms on map
typedef struct Room {
    char* name; // name of room
    Item items[MAX_ITEMS]; // array of items in room
    int num_items; // number of items currently in room
} Room;

// player struct to represent each player in the game
typedef struct Player {
    char* name; // name of player
    int score; // score of player
    Room* current_room; // current room the player is in
} Player;

// initialize all rooms on map with random items
void initialize_rooms(Room* rooms) {
    Item items[] = {
        {"sword", 20},
        {"potion", 10},
        {"shield", 15},
        {"key", 5},
        {"gem", 25},
        {"book", 5}
    };

    for (int i = 0; i < NUM_ROOMS; i++) {
        rooms[i].name = "Room";
        rooms[i].num_items = rand() % MAX_ITEMS;
        for (int j = 0; j < rooms[i].num_items; j++) {
            int index = rand() % 6;
            rooms[i].items[j] = items[index];
        }
    }
}

// print out all items in a room
void print_items(Room room) {
    printf("Items in %s:\n", room.name);
    for (int i = 0; i < room.num_items; i++) {
        printf("- %s (%d points)\n", room.items[i].name, room.items[i].value);
    }
}

// move player to a new room
void move_player(Player* player, Room* new_room) {
    printf("%s moved from %s to %s\n", player->name, player->current_room->name, new_room->name);
    player->current_room = new_room;
}

int main() {
    srand(time(NULL)); // seed random number generator

    Room rooms[NUM_ROOMS];
    initialize_rooms(rooms);

    // create array of players
    Player players[MAX_PLAYERS];
    int num_players = 0;

    // ask for player names until user enters "done"
    char name[20];
    while (true) {
        printf("Enter player name (or \"done\" to finish): ");
        scanf("%s", name);
        if (strcmp(name, "done") == 0) {
            break;
        }
        Player player;
        player.name = name;
        player.score = 0;
        player.current_room = &rooms[0];
        players[num_players++] = player;
    }

    // game loop
    bool game_over = false;
    while (!game_over) {
        // print out map and all items in current room
        printf("\nMap:\n");
        for (int i = 0; i < NUM_ROOMS; i++) {
            printf("%s%d: ", rooms[i].name, i);
            for (int j = 0; j < rooms[i].num_items; j++) {
                printf("%s ", rooms[i].items[j].name);
            }
            printf("\n");
        }
        printf("\n");

        // print out all player stats
        printf("Players:\n");
        for (int i = 0; i < num_players; i++) {
            printf("%s: score %d, in %s\n", players[i].name, players[i].score, players[i].current_room->name);
        }

        // ask player which direction they want to move in
        char direction[10];
        printf("\nWhich direction do you want to move in? (north, south, east, or west): ");
        scanf("%s", direction);

        // if player enters "quit", end game
        if (strcmp(direction, "quit") == 0) {
            game_over = true;
            printf("Thanks for playing!\n");
            break;
        }

        // randomly select a room in the direction entered by the player
        Room* new_room = NULL;
        int current_room_index = players[0].current_room - rooms;
        if (strcmp(direction, "north") == 0) {
            if (current_room_index >= 5) {
                new_room = &rooms[current_room_index - 5];
            }
        } else if (strcmp(direction, "south") == 0) {
            if (current_room_index <= 14) {
                new_room = &rooms[current_room_index + 5];
            }
        } else if (strcmp(direction, "east") == 0) {
            if (current_room_index % 5 != 4) {
                new_room = &rooms[current_room_index + 1];
            }
        } else if (strcmp(direction, "west") == 0) {
            if (current_room_index % 5 != 0) {
                new_room = &rooms[current_room_index - 1];
            }
        }

        if (new_room == NULL) {
            printf("Invalid direction entered!\n");
        } else {
            // update all players current rooms, print out items in new room
            for (int i = 0; i < num_players; i++) {
                move_player(&players[i], new_room);
            }
            print_items(*new_room);

            // ask each player if they want to pick up any items in the room
            for (int i = 0; i < num_players; i++) {
                printf("%s, do you want to pick up any items? (enter item name or \"none\"): ", players[i].name);
                char item_name[20];
                scanf("%s", item_name);
                if (strcmp(item_name, "none") != 0) {
                    for (int j = 0; j < new_room->num_items; j++) {
                        if (strcmp(item_name, new_room->items[j].name) == 0) {
                            players[i].score += new_room->items[j].value;
                            printf("%s picked up %s (+%d points)!\n", players[i].name, new_room->items[j].name, new_room->items[j].value);
                            // remove item from room
                            for (int k = j; k < new_room->num_items-1; k++) {
                                new_room->items[k] = new_room->items[k+1];
                            }
                            new_room->num_items--;
                            break;
                        }
                    }
                }
            }
        }
    }

    return 0;
}