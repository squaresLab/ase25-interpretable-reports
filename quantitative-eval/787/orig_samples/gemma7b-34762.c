//Gemma-7B DATASET v1.0 Category: Building a CSV Reader ; Style: systematic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

typedef struct CSVReader {
  FILE *fp;
  char **buffer;
  int bufferSize;
  int currentLine;
  int currentColumn;
} CSVReader;

CSVReader *csvReader_init(const char *filename) {
  CSVReader *reader = malloc(sizeof(CSVReader));
  reader->fp = fopen(filename, "r");
  reader->buffer = NULL;
  reader->bufferSize = MAX_BUFFER_SIZE;
  reader->currentLine = 0;
  reader->currentColumn = 0;

  return reader;
}

void csvReader_readCSV(CSVReader *reader) {
  reader->buffer = realloc(reader->buffer, reader->bufferSize * (reader->currentLine + 1) * sizeof(char *));
  reader->buffer[reader->currentLine] = malloc(MAX_BUFFER_SIZE * sizeof(char));

  fgets(reader->buffer[reader->currentLine], MAX_BUFFER_SIZE, reader->fp);

  reader->currentLine++;
}

char **csvReader_getColumn(CSVReader *reader, int column) {
  char **columnData = malloc(reader->currentLine * sizeof(char *));

  for (int i = 0; i < reader->currentLine; i++) {
    columnData[i] = strchr(reader->buffer[i], ',') + 1;
  }

  return columnData;
}

int main() {
  CSVReader *reader = csvReader_init("data.csv");

  csvReader_readCSV(reader);

  char **columnData = csvReader_getColumn(reader, 1);

  for (int i = 0; i < reader->currentLine; i++) {
    printf("%s\n", columnData[i]);
  }

  fclose(reader->fp);
  free(reader);

  return 0;
}