//Falcon-180B DATASET v1.0 Category: Haunted House Simulator ; Style: visionary
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void hauntedHouse();

int main()
{
    srand(time(NULL));
    hauntedHouse();
    return 0;
}

void hauntedHouse()
{
    int choice;
    printf("Welcome to the Haunted House!\n");
    printf("What would you like to do?\n");
    printf("1. Explore the house\n");
    printf("2. Solve a puzzle\n");
    printf("3. Leave\n");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            printf("You enter the house and hear strange noises.\n");
            printf("Do you want to investigate? (y/n)\n");
            char input;
            scanf(" %c", &input);
            if(input == 'y')
            {
                printf("You find a secret passage!\n");
            }
            else
            {
                printf("You leave the house.\n");
            }
            break;

        case 2:
            printf("You find a mysterious puzzle.\n");
            printf("Do you want to solve it? (y/n)\n");
            scanf(" %c", &input);
            if(input == 'y')
            {
                printf("You solve the puzzle and find a hidden treasure!\n");
            }
            else
            {
                printf("You leave the house.\n");
            }
            break;

        case 3:
            printf("You leave the house.\n");
            break;

        default:
            printf("Invalid choice.\n");
            break;
    }
}