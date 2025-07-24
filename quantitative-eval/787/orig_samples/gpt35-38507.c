//FormAI DATASET v1.0 Category: Rogue-like Game with Procedural Generation ; Style: grateful
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define MAX_ROOMS 6
#define MAX_MONSTERS 10

typedef struct {
    int x, y;
    char symbol;
} Entity;

typedef struct {
    int x, y, w, h;
} Room;

typedef struct {
    int map[MAP_WIDTH][MAP_HEIGHT];
    Entity player;
    Entity monsters[MAX_MONSTERS];
    int monster_count;
} GameState;

void draw_map(GameState *state) {
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            printf("%c", state->map[x][y] ? '#' : ' ');
        }
        printf("\n");
    }
}

void place_entity(GameState *state, Entity *entity) {
    int x, y;

    do {
        x = rand() % MAP_WIDTH;
        y = rand() % MAP_HEIGHT;
    } while (state->map[x][y]);

    entity->x = x;
    entity->y = y;
    state->map[x][y] = entity->symbol;
}

void generate_map(GameState *state) {
    int i, j, k;
    Room rooms[MAX_ROOMS];

    for (i = 0; i < MAX_ROOMS; i++) {
        rooms[i].w = rand() % 6 + 5;
        rooms[i].h = rand() % 4 + 3;
        rooms[i].x = rand() % (MAP_WIDTH - rooms[i].w - 2) + 1;
        rooms[i].y = rand() % (MAP_HEIGHT - rooms[i].h - 2) + 1;

        for (j = rooms[i].x; j < rooms[i].x + rooms[i].w; j++) {
            for (k = rooms[i].y; k < rooms[i].y + rooms[i].h; k++) {
                state->map[j][k] = 1;
            }
        }
    }

    for (i = 0; i < MAX_ROOMS - 1; i++) {
        int x1 = rooms[i].x + rooms[i].w / 2;
        int y1 = rooms[i].y + rooms[i].h / 2;
        int x2 = rooms[i+1].x + rooms[i+1].w / 2;
        int y2 = rooms[i+1].y + rooms[i+1].h / 2;

        while (x1 != x2 || y1 != y2) {
            if (x1 < x2) x1++;
            else if (x1 > x2) x1--;
            else if (y1 < y2) y1++;
            else if (y1 > y2) y1--;

            state->map[x1][y1] = 1;
        }
    }

    state->monster_count = MAX_MONSTERS;
    for (i = 0; i < state->monster_count; i++) {
        state->monsters[i].symbol = 'm';
        place_entity(state, &state->monsters[i]);
    }

    state->player.symbol = '@';
    place_entity(state, &state->player);
}

void update(GameState *state) {
    int i;

    for (i = 0; i < state->monster_count; i++) {
        int dx = rand() % 3 - 1;
        int dy = rand() % 3 - 1;

        if (dx || dy) {
            if (!state->map[state->monsters[i].x+dx][state->monsters[i].y+dy]) {
                state->map[state->monsters[i].x][state->monsters[i].y] = 0;
                state->monsters[i].x += dx;
                state->monsters[i].y += dy;
                state->map[state->monsters[i].x][state->monsters[i].y] = state->monsters[i].symbol;
            }
        }
    }
}

int main() {
    GameState state;
    srand(time(NULL));

    generate_map(&state);
    draw_map(&state);

    while (1) {
        char input;

        printf("Enter a command (w/a/s/d): ");
        scanf(" %c", &input);

        if (input == 'w' && !state.map[state.player.x][state.player.y-1]) {
            state.map[state.player.x][state.player.y] = 0;
            state.player.y--;
            state.map[state.player.x][state.player.y] = state.player.symbol;
        }
        else if (input == 's' && !state.map[state.player.x][state.player.y+1]) {
            state.map[state.player.x][state.player.y] = 0;
            state.player.y++;
            state.map[state.player.x][state.player.y] = state.player.symbol;
        }
        else if (input == 'a' && !state.map[state.player.x-1][state.player.y]) {
            state.map[state.player.x][state.player.y] = 0;
            state.player.x--;
            state.map[state.player.x][state.player.y] = state.player.symbol;
        }
        else if (input == 'd' && !state.map[state.player.x+1][state.player.y]) {
            state.map[state.player.x][state.player.y] = 0;
            state.player.x++;
            state.map[state.player.x][state.player.y] = state.player.symbol;
        }

        update(&state);
        draw_map(&state);
    }

    return 0;
}