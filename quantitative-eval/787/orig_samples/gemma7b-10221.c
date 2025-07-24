//Gemma-7B DATASET v1.0 Category: Building a CSV Reader ; Style: peaceful
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

typedef struct CSVReader
{
    FILE* file;
    char** buffer;
    int rows;
    int cols;
} CSVReader;

CSVReader* csv_reader_init(const char* filename)
{
    CSVReader* reader = malloc(sizeof(CSVReader));
    reader->file = fopen(filename, "r");
    reader->buffer = NULL;
    reader->rows = 0;
    reader->cols = 0;

    return reader;
}

void csv_reader_read(CSVReader* reader)
{
    char* line = NULL;
    size_t line_size = 0;
    int i = 0;

    while (getline(&line, &line_size, reader->file) != -1)
    {
        reader->rows++;
        reader->buffer = realloc(reader->buffer, reader->rows * MAX_BUFFER_SIZE);
        reader->buffer[reader->rows - 1] = strdup(line);

        for (i = 0; i < line_size; i++)
        {
            if (line[i] == ',')
            {
                reader->cols++;
            }
        }
    }

    free(line);
}

void csv_reader_destroy(CSVReader* reader)
{
    fclose(reader->file);
    free(reader->buffer);
    free(reader);
}

int main()
{
    CSVReader* reader = csv_reader_init("data.csv");
    csv_reader_read(reader);
    csv_reader_destroy(reader);

    return 0;
}