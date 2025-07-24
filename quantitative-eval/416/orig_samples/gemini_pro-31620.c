//GEMINI-pro DATASET v1.0 Category: Scientific Calculator Implementation ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#define MAX_PLAYERS 4
#define MAX_OPERATIONS 5

typedef struct {
    int id;
    int score;
    char name[32];
} player_t;

typedef struct {
    int num1;
    int num2;
    char op;
} operation_t;

player_t players[MAX_PLAYERS];
operation_t operations[MAX_OPERATIONS];

int current_player = 0;
int current_operation = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *player_thread(void *arg) {
    int id = *(int *)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (current_player != id) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("Player %d's turn\n", id);

        operation_t op = operations[current_operation];

        int result;
        switch (op.op) {
            case '+':
                result = op.num1 + op.num2;
                break;
            case '-':
                result = op.num1 - op.num2;
                break;
            case '*':
                result = op.num1 * op.num2;
                break;
            case '/':
                result = op.num1 / op.num2;
                break;
            case '%':
                result = op.num1 % op.num2;
                break;
        }

        int answer;
        printf("What is the answer to %d %c %d? ", op.num1, op.op, op.num2);
        scanf("%d", &answer);

        if (answer == result) {
            players[id].score++;
            printf("Correct!\n");
        } else {
            printf("Incorrect!\n");
        }

        current_player = (current_player + 1) % MAX_PLAYERS;
        current_operation = (current_operation + 1) % MAX_OPERATIONS;

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].id = i;
        players[i].score = 0;
        strcpy(players[i].name, "Player X"); // Replace with real names
    }

    for (int i = 0; i < MAX_OPERATIONS; i++) {
        operations[i].num1 = rand() % 100;
        operations[i].num2 = rand() % 100;
        operations[i].op = "+-*/%"[rand() % 5];
    }

    pthread_t threads[MAX_PLAYERS];

    for (int i = 0; i < MAX_PLAYERS; i++) {
        pthread_create(&threads[i], NULL, player_thread, &i);
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final scores:\n");

    for (int i = 0; i < MAX_PLAYERS; i++) {
        printf("%s: %d\n", players[i].name, players[i].score);
    }

    return 0;
}