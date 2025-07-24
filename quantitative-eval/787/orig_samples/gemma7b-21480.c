//Gemma-7B DATASET v1.0 Category: Building a CSV Reader ; Style: introspective
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

typedef struct CSVReader {
  FILE *fp;
  char **buffer;
  int bufferSize;
  int lineNumber;
  int columnNumber;
} CSVReader;

CSVReader *csvReader_init(char *filename) {
  CSVReader *reader = malloc(sizeof(CSVReader));
  reader->fp = fopen(filename, "r");
  reader->buffer = NULL;
  reader->bufferSize = 0;
  reader->lineNumber = 0;
  reader->columnNumber = 0;
  return reader;
}

void csvReader_readCSV(CSVReader *reader) {
  char line[MAX_BUFFER_SIZE];
  reader->lineNumber++;
  fgets(line, MAX_BUFFER_SIZE, reader->fp);
  reader->bufferSize = 0;
  reader->buffer = realloc(reader->buffer, (reader->lineNumber + 1) * MAX_BUFFER_SIZE);
  reader->buffer[reader->lineNumber - 1] = strdup(line);
  char *token = strtok(line, ",");
  reader->columnNumber = 0;
  while (token) {
    reader->columnNumber++;
    token = strtok(NULL, ",");
  }
}

void csvReader_finalize(CSVReader *reader) {
  fclose(reader->fp);
  free(reader->buffer);
  free(reader);
}

int main() {
  CSVReader *reader = csvReader_init("example.csv");
  csvReader_readCSV(reader);
  csvReader_readCSV(reader);
  csvReader_finalize(reader);
  return 0;
}