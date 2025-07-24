//Falcon-180B DATASET v1.0 Category: Log analysis ; Style: complex
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOG_SIZE 1000
#define MAX_WORD_LENGTH 50
#define MAX_LINE_LENGTH 1000

typedef struct {
    char *word;
    int count;
} WordCount;

int compareWords(const void *a, const void *b) {
    WordCount *wc1 = (WordCount *)a;
    WordCount *wc2 = (WordCount *)b;
    
    return strcmp(wc1->word, wc2->word);
}

int getLineWords(char *line, char *words[]) {
    char *word;
    int count = 0;
    
    word = strtok(line, " ");
    while (word!= NULL) {
        words[count] = word;
        count++;
        word = strtok(NULL, " ");
    }
    
    return count;
}

int main() {
    FILE *logFile;
    char logLine[MAX_LINE_LENGTH];
    char *word;
    int lineCount = 0;
    int totalWords = 0;
    WordCount wordsCount[MAX_WORD_LENGTH];
    int numWords = 0;
    
    logFile = fopen("logfile.txt", "r");
    
    if (logFile == NULL) {
        printf("Error: Could not open log file.\n");
        return 1;
    }
    
    while (fgets(logLine, MAX_LINE_LENGTH, logFile)!= NULL) {
        lineCount++;
        totalWords += getLineWords(logLine, wordsCount);
    }
    
    fclose(logFile);
    
    qsort(wordsCount, numWords, sizeof(WordCount), compareWords);
    
    printf("Total lines: %d\n", lineCount);
    printf("Total words: %d\n", totalWords);
    
    for (int i = 0; i < numWords; i++) {
        printf("%s: %d\n", wordsCount[i].word, wordsCount[i].count);
    }
    
    return 0;
}