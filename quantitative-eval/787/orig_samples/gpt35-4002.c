//FormAI DATASET v1.0 Category: Rogue-like Game with Procedural Generation ; Style: realistic
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define WIDTH 40
#define HEIGHT 20

void wait_for_enter() {
    while (getchar() != '\n');
}

typedef struct {
    int x;
    int y;
} position_t;

typedef struct {
    position_t pos;
    char chr;
} entity_t;

typedef struct {
    bool visited;
    bool occupied;
} cell_t;

typedef struct {
    cell_t cells[HEIGHT][WIDTH];
    entity_t player;
    entity_t enemies[5];
} map_t;

void init_map(map_t* map) {
    // initialize cells
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            map->cells[i][j].visited = false;
            map->cells[i][j].occupied = false;
        }
    }
    
    // place player in center
    map->player.pos.x = WIDTH/2;
    map->player.pos.y = HEIGHT/2;
    map->player.chr = '@';
    map->cells[map->player.pos.y][map->player.pos.x].occupied = true;
    
    // place enemies
    srand(time(NULL));
    for (int i = 0; i < 5; i++) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        // check if occupied, if so, try again
        while (map->cells[y][x].occupied) {
            x = rand() % WIDTH;
            y = rand() % HEIGHT;
        }
        map->enemies[i].pos.x = x;
        map->enemies[i].pos.y = y;
        map->enemies[i].chr = 'E';
        map->cells[y][x].occupied = true;
    }
}

void print_map(map_t map) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (map.cells[i][j].occupied) {
                bool print_entity = false;
                char entity_chr = ' ';
                if (i == map.player.pos.y && j == map.player.pos.x) {
                    print_entity = true;
                    entity_chr = map.player.chr;
                } else {
                    for (int k = 0; k < 5; k++) {
                        if (i == map.enemies[k].pos.y && j == map.enemies[k].pos.x) {
                            print_entity = true;
                            entity_chr = map.enemies[k].chr;
                            break;
                        }
                    }
                }
                if (print_entity) {
                    printf("%c", entity_chr);
                } else {
                    printf("#");
                }
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

void move_player(map_t* map, int dx, int dy) {
    int new_x = map->player.pos.x + dx;
    int new_y = map->player.pos.y + dy;
    if (new_x >= 0 && new_x < WIDTH && new_y >= 0 && new_y < HEIGHT && !map->cells[new_y][new_x].occupied) {
        map->cells[map->player.pos.y][map->player.pos.x].occupied = false;
        map->player.pos.x = new_x;
        map->player.pos.y = new_y;
        map->cells[map->player.pos.y][map->player.pos.x].occupied = true;
    }
}

int main() {
    map_t map;
    init_map(&map);
    print_map(map);
    
    char input;
    while (true) {
        input = getchar();
        switch (input) {
            case 'w':
                move_player(&map, 0, -1);
                break;
            case 'a':
                move_player(&map, -1, 0);
                break;
            case 's':
                move_player(&map, 0, 1);
                break;
            case 'd':
                move_player(&map, 1, 0);
                break;
            default:
                continue;
        }
        printf("\033[H\033[J");
        print_map(map);
    }
    
    return 0;
}