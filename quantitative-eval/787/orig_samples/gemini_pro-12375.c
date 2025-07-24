//GEMINI-pro DATASET v1.0 Category: Building a CSV Reader ; Style: curious
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// A custom CSV reader that's both efficient and easy to use!

// Our trusty CSV reader struct, holding all the juicy data
typedef struct CSVReader {
    FILE *fp; // The file we're reading from
    char *line; // The current line we're parsing
    char **fields; // An array of strings representing the fields in the current line
    int num_fields; // The number of fields in the current line
} CSVReader;

// Initializes a new CSV reader for the given file
CSVReader* csv_reader_init(FILE *fp) {
    CSVReader *reader = malloc(sizeof(CSVReader));
    reader->fp = fp;
    reader->line = NULL;
    reader->fields = NULL;
    reader->num_fields = 0;
    return reader;
}

// Frees the resources associated with the CSV reader
void csv_reader_free(CSVReader *reader) {
    if (reader->line != NULL) free(reader->line);
    if (reader->fields != NULL) free(reader->fields);
    free(reader);
}

// Reads the next line from the CSV file and parses it into fields
int csv_reader_next_line(CSVReader *reader) {
    // Free the old line and fields
    if (reader->line != NULL) free(reader->line);
    if (reader->fields != NULL) free(reader->fields);

    // Get the next line from the file
    size_t len = 0;
    if (getline(&reader->line, &len, reader->fp) == -1) return 0; // EOF

    // Split the line into fields
    char *start = reader->line;
    int num_fields = 0;
    while (*start != '\0') {
        // Skip leading whitespace
        while (*start == ' ') start++;

        // Find the end of the field
        char *end = start;
        while (*end != '\0' && *end != ',' && *end != '\n') end++;

        // Copy the field into the array
        if (num_fields == reader->num_fields) {
            reader->num_fields++;
            reader->fields = realloc(reader->fields, reader->num_fields * sizeof(char*));
        }
        reader->fields[num_fields] = malloc(end - start + 1);
        strncpy(reader->fields[num_fields], start, end - start);
        reader->fields[num_fields][end - start] = '\0';
        num_fields++;

        // Skip trailing whitespace and the comma
        start = end;
        while (*start == ' ' || *start == ',') start++;
    }

    // Update the number of fields
    reader->num_fields = num_fields;
    return 1;
}

// Gets the value of the specified field in the current line
char* csv_reader_get_field(CSVReader *reader, int index) {
    if (index < 0 || index >= reader->num_fields) return NULL;
    return reader->fields[index];
}

// Prints the current line
void csv_reader_print_line(CSVReader *reader) {
    for (int i = 0; i < reader->num_fields; i++) {
        printf("%s", reader->fields[i]);
        if (i < reader->num_fields - 1) printf(",");
    }
    printf("\n");
}

// Our main function, where the magic happens!
int main() {
    // Open the CSV file
    FILE *fp = fopen("data.csv", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Create a CSV reader
    CSVReader *reader = csv_reader_init(fp);

    // Read and print each line
    while (csv_reader_next_line(reader)) {
        csv_reader_print_line(reader);
    }

    // Free the reader and close the file
    csv_reader_free(reader);
    fclose(fp);
    return 0;
}