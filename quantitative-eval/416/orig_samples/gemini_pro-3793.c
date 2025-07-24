//GEMINI-pro DATASET v1.0 Category: Ebook reader ; Style: lively
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Page of the book
typedef struct Page {
    int number;
    char *content;
} Page;

// Book structure
typedef struct Book {
    char *title;
    int num_pages;
    Page *pages;
} Book;

// Create a new book
Book *create_book(char *title, int num_pages) {
    Book *book = malloc(sizeof(Book));
    book->title = title;
    book->num_pages = num_pages;
    book->pages = malloc(sizeof(Page) * num_pages);
    for (int i = 0; i < num_pages; i++) {
        book->pages[i].number = i + 1;
        book->pages[i].content = NULL;
    }
    return book;
}

// Load a book from a file
Book *load_book(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    // Read the title of the book
    char title[256];
    fgets(title, 256, fp);

    // Read the number of pages in the book
    int num_pages;
    fscanf(fp, "%d", &num_pages);

    // Create a new book
    Book *book = create_book(title, num_pages);

    // Read the content of each page
    for (int i = 0; i < num_pages; i++) {
        char content[1024];
        fgets(content, 1024, fp);
        book->pages[i].content = strdup(content);
    }

    fclose(fp);

    return book;
}

// Free the memory allocated for a book
void free_book(Book *book) {
    for (int i = 0; i < book->num_pages; i++) {
        free(book->pages[i].content);
    }
    free(book->pages);
    free(book->title);
    free(book);
}

// Print the content of a page
void print_page(Page *page) {
    printf("Page %d:\n%s", page->number, page->content);
}

// Print the table of contents of a book
void print_toc(Book *book) {
    printf("Table of Contents:\n");
    for (int i = 0; i < book->num_pages; i++) {
        printf("  %d. %s", book->pages[i].number, book->pages[i].content);
    }
}

// Main function
int main() {
    // Load a book from a file
    Book *book = load_book("book.txt");
    if (book == NULL) {
        return -1;
    }

    // Print the table of contents
    print_toc(book);

    // Print the first page of the book
    print_page(&book->pages[0]);

    // Free the memory allocated for the book
    free_book(book);

    return 0;
}