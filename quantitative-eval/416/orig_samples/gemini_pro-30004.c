//GEMINI-pro DATASET v1.0 Category: Ebook reader ; Style: thoughtful
#include <stdio.h>
#include <stdlib.h>

// Define the structure of a book
typedef struct book {
    char *title;
    char *author;
    int num_pages;
    char *text;
} book;

// Create a new book
book *create_book(char *title, char *author, int num_pages, char *text) {
    book *new_book = malloc(sizeof(book));
    new_book->title = title;
    new_book->author = author;
    new_book->num_pages = num_pages;
    new_book->text = text;
    return new_book;
}

// Print the details of a book
void print_book(book *book) {
    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("Number of pages: %d\n", book->num_pages);
    printf("Text: %s\n", book->text);
}

// Read a book from a file
book *read_book_from_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Read the title of the book
    char title[256];
    if (fgets(title, sizeof(title), file) == NULL) {
        perror("Error reading title");
        fclose(file);
        return NULL;
    }

    // Read the author of the book
    char author[256];
    if (fgets(author, sizeof(author), file) == NULL) {
        perror("Error reading author");
        fclose(file);
        return NULL;
    }

    // Read the number of pages in the book
    int num_pages;
    if (fscanf(file, "%d", &num_pages) != 1) {
        perror("Error reading number of pages");
        fclose(file);
        return NULL;
    }

    // Read the text of the book
    char *text = malloc(sizeof(char) * (num_pages + 1));
    if (fread(text, sizeof(char), num_pages, file) != num_pages) {
        perror("Error reading text");
        fclose(file);
        return NULL;
    }

    // Close the file
    fclose(file);

    // Create a new book
    book *new_book = create_book(title, author, num_pages, text);
    return new_book;
}

// Write a book to a file
void write_book_to_file(char *filename, book *book) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write the title of the book
    fprintf(file, "%s\n", book->title);

    // Write the author of the book
    fprintf(file, "%s\n", book->author);

    // Write the number of pages in the book
    fprintf(file, "%d\n", book->num_pages);

    // Write the text of the book
    fwrite(book->text, sizeof(char), book->num_pages, file);

    // Close the file
    fclose(file);
}

// Free the memory allocated for a book
void free_book(book *book) {
    free(book->title);
    free(book->author);
    free(book->text);
    free(book);
}

// Main function
int main(int argc, char **argv) {
    // Check if a filename was provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Read the book from the file
    book *book = read_book_from_file(argv[1]);
    if (book == NULL) {
        fprintf(stderr, "Error reading book from file\n");
        return EXIT_FAILURE;
    }

    // Print the details of the book
    print_book(book);

    // Free the memory allocated for the book
    free_book(book);

    return EXIT_SUCCESS;
}