//FormAI DATASET v1.0 Category: Chess AI ; Style: retro
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8

// Chess Piece Types
typedef enum PieceType {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} PieceType;

// Chess Pieces
typedef struct ChessPiece {
    char symbol;
    PieceType type;
} ChessPiece;

// Chess Board Square
typedef struct ChessSquare {
    int x, y;
    ChessPiece piece;
} ChessSquare;

// Chess Board
typedef struct ChessBoard {
    ChessSquare squares[BOARD_SIZE][BOARD_SIZE];
    ChessPiece white_pieces[16];
    ChessPiece black_pieces[16];
} ChessBoard;

// Function to initialize the chess board
void init_board(ChessBoard* board) {
    // Set up each square with an empty piece
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            board->squares[x][y].x = x;
            board->squares[x][y].y = y;
            board->squares[x][y].piece.type = EMPTY;
            board->squares[x][y].piece.symbol = ' ';
        }
    }

    // Set up the starting position for each piece
    for (int i = 0; i < 8; i++) {
        board->squares[i][1].piece.type = PAWN;
        board->squares[i][1].piece.symbol = 'P';
        board->white_pieces[i].type = PAWN;
        board->white_pieces[i].symbol = 'P';
    }

    board->squares[0][0].piece.type = ROOK;
    board->squares[0][0].piece.symbol = 'R';
    board->white_pieces[8].type = ROOK;
    board->white_pieces[8].symbol = 'R';

    board->squares[1][0].piece.type = KNIGHT;
    board->squares[1][0].piece.symbol = 'N';
    board->white_pieces[9].type = KNIGHT;
    board->white_pieces[9].symbol = 'N';

    board->squares[2][0].piece.type = BISHOP;
    board->squares[2][0].piece.symbol = 'B';
    board->white_pieces[10].type = BISHOP;
    board->white_pieces[10].symbol = 'B';

    board->squares[3][0].piece.type = QUEEN;
    board->squares[3][0].piece.symbol = 'Q';
    board->white_pieces[11].type = QUEEN;
    board->white_pieces[11].symbol = 'Q';

    board->squares[4][0].piece.type = KING;
    board->squares[4][0].piece.symbol = 'K';
    board->white_pieces[12].type = KING;
    board->white_pieces[12].symbol = 'K';

    board->squares[5][0].piece.type = BISHOP;
    board->squares[5][0].piece.symbol = 'B';
    board->white_pieces[13].type = BISHOP;
    board->white_pieces[13].symbol = 'B';

    board->squares[6][0].piece.type = KNIGHT;
    board->squares[6][0].piece.symbol = 'N';
    board->white_pieces[14].type = KNIGHT;
    board->white_pieces[14].symbol = 'N';

    board->squares[7][0].piece.type = ROOK;
    board->squares[7][0].piece.symbol = 'R';
    board->white_pieces[15].type = ROOK;
    board->white_pieces[15].symbol = 'R';

    for (int i = 0; i < 8; i++) {
        board->squares[i][6].piece.type = PAWN;
        board->squares[i][6].piece.symbol = 'p';
        board->black_pieces[i].type = PAWN;
        board->black_pieces[i].symbol = 'p';
    }

    board->squares[0][7].piece.type = ROOK;
    board->squares[0][7].piece.symbol = 'r';
    board->black_pieces[8].type = ROOK;
    board->black_pieces[8].symbol = 'r';

    board->squares[1][7].piece.type = KNIGHT;
    board->squares[1][7].piece.symbol = 'n';
    board->black_pieces[9].type = KNIGHT;
    board->black_pieces[9].symbol = 'n';

    board->squares[2][7].piece.type = BISHOP;
    board->squares[2][7].piece.symbol = 'b';
    board->black_pieces[10].type = BISHOP;
    board->black_pieces[10].symbol = 'b';

    board->squares[3][7].piece.type = QUEEN;
    board->squares[3][7].piece.symbol = 'q';
    board->black_pieces[11].type = QUEEN;
    board->black_pieces[11].symbol = 'q';

    board->squares[4][7].piece.type = KING;
    board->squares[4][7].piece.symbol = 'k';
    board->black_pieces[12].type = KING;
    board->black_pieces[12].symbol = 'k';

    board->squares[5][7].piece.type = BISHOP;
    board->squares[5][7].piece.symbol = 'b';
    board->black_pieces[13].type = BISHOP;
    board->black_pieces[13].symbol = 'b';

    board->squares[6][7].piece.type = KNIGHT;
    board->squares[6][7].piece.symbol = 'n';
    board->black_pieces[14].type = KNIGHT;
    board->black_pieces[14].symbol = 'n';

    board->squares[7][7].piece.type = ROOK;
    board->squares[7][7].piece.symbol = 'r';
    board->black_pieces[15].type = ROOK;
    board->black_pieces[15].symbol = 'r';
}

// Function to display the chess board
void display_board(ChessBoard* board) {
    printf("\n   A  B  C  D  E  F  G  H\n");
    printf("  -------------------------\n");

    for (int y = 7; y >= 0; y--) {
        printf("%d |", y + 1);
        for (int x = 0; x < 8; x++) {
            printf("%c |", board->squares[x][y].piece.symbol);
        }
        printf(" %d\n", y + 1);
        printf("  -------------------------\n");
    }

    printf("   A  B  C  D  E  F  G  H\n");
}

// Function to get the user input of the move
void get_move_input(int* from_x, int* from_y, int* to_x, int* to_y) {
    printf("Enter move (ex. 'e2 e4'): ");
    scanf("%d %d %d %d", from_x, from_y, to_x, to_y);
    printf("\n");
}

// Function to make a move on the chess board
void make_move(ChessBoard* board, int from_x, int from_y, int to_x, int to_y) {
    board->squares[to_x][to_y].piece = board->squares[from_x][from_y].piece;
    board->squares[from_x][from_y].piece.type = EMPTY;
    board->squares[from_x][from_y].piece.symbol = ' ';
}

int main() {
    ChessBoard board;
    init_board(&board);
    display_board(&board);

    int from_x, from_y, to_x, to_y;

    while (1) {
        get_move_input(&from_x, &from_y, &to_x, &to_y);
        if ((from_x < 1 || from_x > 8) || (to_x < 1 || to_x > 8) || (from_y < 1 || from_y > 8) || (to_y < 1 || to_y > 8)) {
            printf("Invalid move!!!\n");
            continue;
        }
        make_move(&board, from_x - 1, from_y - 1, to_x - 1, to_y - 1);
        display_board(&board);
    }
    return 0;
}