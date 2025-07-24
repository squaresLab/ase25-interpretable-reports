//GEMINI-pro DATASET v1.0 Category: Checkers Game ; Style: unmistakable
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BOARD_SIZE 8
#define NUM_PIECES_PER_PLAYER 12

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    char type;
    Position position;
} Piece;

typedef struct {
    Piece pieces[NUM_PIECES_PER_PLAYER];
    int num_pieces;
} Player;

typedef struct {
    Player players[2];
    Piece board[BOARD_SIZE][BOARD_SIZE];
} Game;

void init_game(Game *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j].type = ' ';
        }
    }

    for (int i = 0; i < NUM_PIECES_PER_PLAYER; i++) {
        game->players[0].pieces[i].type = 'b';
        game->players[0].pieces[i].position.x = i % 4 + 1;
        game->players[0].pieces[i].position.y = 1 + (i / 4);

        game->players[1].pieces[i].type = 'w';
        game->players[1].pieces[i].position.x = i % 4 + 1;
        game->players[1].pieces[i].position.y = BOARD_SIZE - (1 + (i / 4));
    }
}

void print_board(Game *game) {
    printf("  ");
    for (int i = 1; i <= BOARD_SIZE; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    for (int i = 1; i <= BOARD_SIZE; i++) {
        printf("%2d ", i);
        for (int j = 1; j <= BOARD_SIZE; j++) {
            printf("%c  ", game->board[i - 1][j - 1].type);
        }
        printf("\n");
    }
}

int main() {
    Game game;
    init_game(&game);
    print_board(&game);

    for (int i = 1; i <= BOARD_SIZE; i++) {
        printf("A   B   C   D   E   F   G   H\n");
    }

    return 0;
}