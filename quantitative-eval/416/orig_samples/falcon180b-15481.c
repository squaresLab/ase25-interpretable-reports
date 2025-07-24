//Falcon-180B DATASET v1.0 Category: Random Conspiracy Theory Generator ; Style: Dennis Ritchie
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 20

typedef struct {
    char *word;
    int frequency;
} WordFrequency;

int main() {
    srand(time(NULL));

    char *words[MAX_WORDS];
    int numWords = 0;
    int totalFrequency = 0;

    while (numWords < MAX_WORDS && totalFrequency < 100) {
        char word[MAX_WORD_LENGTH];
        int length = rand() % MAX_WORD_LENGTH + 1;
        for (int i = 0; i < length; i++) {
            word[i] = rand() % 26 + 'a';
        }
        word[length] = '\0';

        WordFrequency *frequency = (WordFrequency *)malloc(sizeof(WordFrequency));
        frequency->word = strdup(word);
        frequency->frequency = rand() % 100 + 1;
        totalFrequency += frequency->frequency;

        for (int i = 0; i < numWords; i++) {
            if (strcmp(words[i], word) == 0) {
                free(frequency->word);
                free(frequency);
                i--;
            }
        }

        words[numWords++] = word;
    }

    printf("The top 10 most frequent words are:\n");
    for (int i = 0; i < numWords; i++) {
        printf("%s: %d\n", words[i], rand() % 100 + 1);
    }

    return 0;
}