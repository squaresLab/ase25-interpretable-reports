//Gemma-7B DATASET v1.0 Category: Ebook reader ; Style: shocked
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main()
{
    system("clear");
    system("figlet -f pixel -c 'BOOK READ'");
    system("toilet -f");

    printf("Welcome to the Book Reading Experience, my dear.\n\n");
    printf("Please select a book from the list below:\n");

    char book_choice;
    scanf(" %c", &book_choice);

    switch (book_choice)
    {
        case 'a':
            system("clear");
            system("figlet -f pixel -c 'The Hobbit'");
            printf("You have chosen The Hobbit by J.R.R. Tolkien.\n\n");
            printf("Would you like to read the book summary? (Y/N): ");
            char summary_choice;
            scanf(" %c", &summary_choice);

            if (summary_choice == 'y')
            {
                system("clear");
                system("figlet -f pixel -c 'Book Summary'");
                printf("The Hobbit is a fantasy novel... blah blah blah.\n\n");
            }
            break;

        case 'b':
            system("clear");
            system("figlet -f pixel -c 'The Lord of the Flies'");
            printf("You have chosen The Lord of the Flies by William Golding.\n\n");
            printf("Would you like to read the book summary? (Y/N): ");
            scanf(" %c", &summary_choice);

            if (summary_choice == 'y')
            {
                system("clear");
                system("figlet -f pixel -c 'Book Summary'");
                printf("The Lord of the Flies is a coming-of-age novel... blah blah blah.\n\n");
            }
            break;

        default:
            printf("Invalid book selection.\n");
    }

    system("pause");
}