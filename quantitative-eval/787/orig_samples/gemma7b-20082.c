//Gemma-7B DATASET v1.0 Category: Building a CSV Reader ; Style: systematic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

typedef struct CSVReader
{
    FILE *file;
    char **buffer;
    int size;
    int currentLine;
} CSVReader;

CSVReader *csvReader_init(char *filename)
{
    CSVReader *reader = malloc(sizeof(CSVReader));

    reader->file = fopen(filename, "r");
    reader->buffer = NULL;
    reader->size = 0;
    reader->currentLine = 0;

    return reader;
}

void csvReader_free(CSVReader *reader)
{
    fclose(reader->file);
    free(reader->buffer);
    free(reader);
}

int csvReader_getLine(CSVReader *reader)
{
    char *line = NULL;
    int lineSize = 0;

    if (reader->currentLine >= reader->size)
    {
        reader->buffer = realloc(reader->buffer, MAX_BUFFER_SIZE * (reader->size + 1));
        reader->size++;
    }

    line = fgets(reader->buffer[reader->currentLine], MAX_BUFFER_SIZE, reader->file);

    if (line)
    {
        reader->currentLine++;
        return lineSize = strlen(line) + 1;
    }

    return 0;
}

int main()
{
    CSVReader *reader = csvReader_init("example.csv");

    while (csvReader_getLine(reader) > 0)
    {
        // Process the line
    }

    csvReader_free(reader);

    return 0;
}