//Falcon-180B DATASET v1.0 Category: Random Conspiracy Theory Generator ; Style: Romeo and Juliet
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_CONSPIRACY_LENGTH 100
#define MAX_CONSPIRACY_WORDS 20
#define MAX_WORD_LENGTH 20

typedef struct {
    char *word;
    int length;
} ConspiracyWord;

void generate_conspiracy(char *conspiracy, int length) {
    srand(time(NULL));
    ConspiracyWord words[MAX_CONSPIRACY_WORDS];
    int num_words = 0;

    // Add random words to the conspiracy
    while (num_words < length) {
        char word[MAX_WORD_LENGTH] = {0};
        int word_length = rand() % MAX_WORD_LENGTH + 1;

        for (int i = 0; i < word_length; i++) {
            char c = rand() % 26 + 'a';
            strncat(word, &c, 1);
        }

        ConspiracyWord new_word = {word, word_length};
        words[num_words++] = new_word;
    }

    // Shuffle the words
    for (int i = 0; i < num_words; i++) {
        ConspiracyWord temp = words[i];
        int j = rand() % num_words;
        words[i] = words[j];
        words[j] = temp;
    }

    // Concatenate the words into a conspiracy
    strcpy(conspiracy, words[0].word);
    for (int i = 1; i < num_words; i++) {
        strcat(conspiracy, " ");
        strcat(conspiracy, words[i].word);
    }
}

int main() {
    char conspiracy[MAX_CONSPIRACY_LENGTH] = {0};
    int length = rand() % MAX_CONSPIRACY_LENGTH + 1;

    generate_conspiracy(conspiracy, length);
    printf("A new conspiracy theory has emerged:\n");
    printf("%s\n", conspiracy);

    return 0;
}