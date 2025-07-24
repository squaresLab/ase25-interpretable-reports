//Gemma-7B DATASET v1.0 Category: Building a CSV Reader ; Style: enthusiastic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

typedef struct CSVReader {
    FILE *fp;
    char **buffer;
    int bufferSize;
    int currentLineNo;
    int currentColNo;
} CSVReader;

CSVReader *csvReader_init(char *fileName) {
    CSVReader *reader = malloc(sizeof(CSVReader));
    reader->fp = fopen(fileName, "r");
    reader->buffer = NULL;
    reader->bufferSize = 0;
    reader->currentLineNo = 0;
    reader->currentColNo = 0;

    return reader;
}

void csvReader_readCSV(CSVReader *reader) {
    char line[MAX_BUFFER_SIZE];
    reader->currentLineNo++;

    if (fgets(line, MAX_BUFFER_SIZE, reader->fp) == NULL) {
        return;
    }

    reader->bufferSize++;
    reader->buffer = realloc(reader->buffer, reader->bufferSize * MAX_BUFFER_SIZE);

    strcpy(reader->buffer[reader->currentLineNo - 1], line);
    reader->currentColNo = 0;
}

char *csvReader_getColumnValue(CSVReader *reader, int colNo) {
    char *value = strchr(reader->buffer[reader->currentLineNo - 1], ',') + 1;

    reader->currentColNo++;

    return value;
}

void csvReader_free(CSVReader *reader) {
    fclose(reader->fp);
    free(reader->buffer);
    free(reader);
}

int main() {
    CSVReader *reader = csvReader_init("data.csv");

    csvReader_readCSV(reader);

    char *value = csvReader_getColumnValue(reader, 2);

    printf("%s", value);

    csvReader_free(reader);

    return 0;
}