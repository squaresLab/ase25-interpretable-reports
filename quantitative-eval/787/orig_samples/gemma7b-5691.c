//Gemma-7B DATASET v1.0 Category: Clustering Algorithm Implementation ; Style: medieval
#include <stdio.h>
#include <stdlib.h>

#define MAX_DISTANCE 10

typedef struct Knight {
    int x;
    int y;
    int loyalty;
} Knight;

int distance(Knight *a, Knight *b) {
    int dx = abs(a->x - b->x);
    int dy = abs(a->y - b->y);
    return dx + dy;
}

void clusterKnights(Knight **knights, int numKnights) {
    for (int i = 0; i < numKnights; i++) {
        Knight *knight = knights[i];
        int minDistance = MAX_DISTANCE;
        int minKnightIndex = -1;
        for (int j = 0; j < numKnights; j++) {
            if (i != j) {
                Knight *otherKnight = knights[j];
                int currentDistance = distance(knight, otherKnight);
                if (currentDistance < minDistance) {
                    minDistance = currentDistance;
                    minKnightIndex = j;
                }
            }
        }
        knight->loyalty = minKnightIndex;
    }
}

int main() {
    Knight knights[] = {
        {1, 2, 0},
        {3, 2, 0},
        {2, 4, 0},
        {4, 4, 0},
        {3, 4, 0}
    };
    int numKnights = 5;

    clusterKnights(knights, numKnights);

    for (int i = 0; i < numKnights; i++) {
        printf("Knight %d's loyalty is %d\n", i, knights[i].loyalty);
    }

    return 0;
}