//Gemma-7B DATASET v1.0 Category: Building a CSV Reader ; Style: modular
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
  reader->bufferSize = 0;
  reader->currentLine = 0;
  reader->currentColumn = 0;

  return reader;
}

void csvReader_free(CSVReader *reader) {
  fclose(reader->fp);
  free(reader->buffer);
  free(reader);
}

int csvReader_readNextLine(CSVReader *reader) {
  char *line = NULL;
  int lineSize = 0;

  if (reader->currentLine >= reader->bufferSize - 1) {
    reader->bufferSize *= 2;
    reader->buffer = realloc(reader->buffer, reader->bufferSize * MAX_BUFFER_SIZE);
  }

  line = fgets(reader->buffer, MAX_BUFFER_SIZE, reader->fp);

  if (line) {
    reader->currentLine++;
    return 1;
  } else {
    return 0;
  }
}

char **csvReader_getColumn(CSVReader *reader, int column) {
  char **columnData = NULL;
  int columnDataIndex = 0;

  if (reader->currentColumn >= reader->bufferSize - 1) {
    reader->bufferSize *= 2;
    reader->buffer = realloc(reader->buffer, reader->bufferSize * MAX_BUFFER_SIZE);
  }

  columnData = reader->buffer[reader->currentLine - 1] + column;

  return columnData;
}

int main() {
  CSVReader *reader = csvReader_init("example.csv");

  if (csvReader_readNextLine(reader) > 0) {
    char **columnData = csvReader_getColumn(reader, 2);

    for (int i = 0; columnData[i] != NULL; i++) {
      printf("%s\n", columnData[i]);
    }
  }

  csvReader_free(reader);

  return 0;
}