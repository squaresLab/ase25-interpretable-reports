//Gemma-7B DATASET v1.0 Category: Building a CSV Reader ; Style: happy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

typedef struct CSVReader {
    FILE* file;
    char** buffer;
    int rows;
    int cols;
} CSVReader;

CSVReader* csv_reader_init(char* filename) {
    CSVReader* reader = malloc(sizeof(CSVReader));
    reader->file = fopen(filename, "r");
    reader->buffer = NULL;
    reader->rows = 0;
    reader->cols = 0;

    return reader;
}

void csv_reader_parse(CSVReader* reader) {
    char line[MAX_BUFFER_SIZE];
    char** data = NULL;
    int i = 0;

    while (fgets(line, MAX_BUFFER_SIZE, reader->file) != NULL) {
        reader->rows++;
        data = realloc(reader->buffer, reader->rows * sizeof(char*));
        reader->buffer = data;

        reader->cols = 0;
        data[reader->rows - 1] = strdup(line);
        while (line[i] != '\n') {
            reader->cols++;
            data[reader->rows - 1] = realloc(data[reader->rows - 1], (reader->cols + 1) * sizeof(char));
            data[reader->rows - 1][reader->cols] = line[i];
            i++;
        }
    }

    fclose(reader->file);
}

int main() {
    CSVReader* reader = csv_reader_init("data.csv");
    csv_reader_parse(reader);

    printf("Number of rows: %d\n", reader->rows);
    printf("Number of columns: %d\n", reader->cols);

    for (int i = 0; i < reader->rows; i++) {
        printf("Row %d: %s\n", i + 1, reader->buffer[i]);
    }

    free(reader->buffer);
    free(reader);

    return 0;
}