//Gemma-7B DATASET v1.0 Category: Building a CSV Reader ; Style: irregular
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CSV_Reader {
    FILE* file;
    char** data;
    int rows;
    int cols;
} CSV_Reader;

CSV_Reader* csv_reader_init(char* filename) {
    CSV_Reader* reader = (CSV_Reader*)malloc(sizeof(CSV_Reader));
    reader->file = fopen(filename, "r");
    reader->data = NULL;
    reader->rows = 0;
    reader->cols = 0;

    return reader;
}

void csv_reader_parse(CSV_Reader* reader) {
    char line[1024];
    char* token;
    int i = 0;

    while (fgets(line, 1024, reader->file) != NULL) {
        reader->rows++;
        token = strtok(line, ",");
        reader->cols = (token ? atoi(token) : 0) + 1;
        reader->data = realloc(reader->data, reader->rows * sizeof(char*));
        reader->data[reader->rows - 1] = malloc(reader->cols * sizeof(char));
        strcpy(reader->data[reader->rows - 1], line);
        i = 0;
        token = strtok(line, ",");
        while (token) {
            reader->data[reader->rows - 1][i++] = token;
            token = strtok(NULL, ",");
        }
    }

    fclose(reader->file);
}

int main() {
    CSV_Reader* reader = csv_reader_init("data.csv");
    csv_reader_parse(reader);

    printf("Number of rows: %d\n", reader->rows);
    printf("Number of columns: %d\n", reader->cols);

    for (int r = 0; r < reader->rows; r++) {
        for (int c = 0; c < reader->cols; c++) {
            printf("%s ", reader->data[r][c]);
        }
        printf("\n");
    }

    free(reader->data);
    free(reader);

    return 0;
}