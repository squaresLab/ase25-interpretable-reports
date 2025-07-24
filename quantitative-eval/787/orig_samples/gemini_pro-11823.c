//GEMINI-pro DATASET v1.0 Category: Table Game ; Style: modular
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 4
#define MAX_CARDS 52
#define MAX_HAND_SIZE 13

typedef struct {
    int value;
    char suit;
} Card;

typedef struct {
    Card cards[MAX_HAND_SIZE];
    int num_cards;
} Hand;

typedef struct {
    Hand hands[MAX_PLAYERS];
    int num_players;
    int current_player;
    Card deck[MAX_CARDS];
    int num_cards_in_deck;
} Game;

void print_card(Card card) {
    char *suits[] = {"C", "D", "H", "S"};
    char *values[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    printf("%s%s", values[card.value - 2], suits[card.suit - 'C']);
}

void print_hand(Hand hand) {
    for (int i = 0; i < hand.num_cards; i++) {
        print_card(hand.cards[i]);
        printf(" ");
    }
    printf("\n");
}

void shuffle_deck(Game *game) {
    for (int i = 0; i < MAX_CARDS; i++) {
        game->deck[i].value = i % 13 + 2;
        game->deck[i].suit = 'C' + i / 13;
    }

    for (int i = 0; i < MAX_CARDS; i++) {
        int j = rand() % MAX_CARDS;
        Card temp = game->deck[i];
        game->deck[i] = game->deck[j];
        game->deck[j] = temp;
    }

    game->num_cards_in_deck = MAX_CARDS;
}

void deal_cards(Game *game) {
    for (int i = 0; i < game->num_players; i++) {
        for (int j = 0; j < MAX_HAND_SIZE; j++) {
            game->hands[i].cards[j] = game->deck[game->num_cards_in_deck - 1];
            game->num_cards_in_deck--;
            game->hands[i].num_cards++;
        }
    }
}

int main() {
    Game game;
    game.num_players = 2;
    game.current_player = 0;

    shuffle_deck(&game);
    deal_cards(&game);

    print_hand(game.hands[0]);
    print_hand(game.hands[1]);

    return 0;
}