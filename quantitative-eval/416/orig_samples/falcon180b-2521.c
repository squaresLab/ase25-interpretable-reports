//Falcon-180B DATASET v1.0 Category: Text-Based Adventure Game ; Style: careful
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(0));
    int choice;

    printf("Welcome to the Text-Based Adventure Game!\n");
    printf("You are a brave adventurer in search of treasure.\n");
    printf("Guide me through the adventure.\n");

    int health = 100;
    int treasure = 0;

    while(health > 0) {
        system("clear");
        printf("You are in a dark, damp cave.\n");
        printf("There are three paths in front of you:\n");
        printf("1. Path to the left\n");
        printf("2. Path in the middle\n");
        printf("3. Path to the right\n");
        printf("Which path do you choose? ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("You take the path to the left.\n");
                printf("You find yourself in a small room with a chest.\n");
                printf("Do you open the chest? (y/n) ");
                char answer;
                scanf(" %c", &answer);

                if(answer == 'y' || answer == 'Y') {
                    printf("You open the chest and find a potion!\n");
                    health += 50;
                }
                else {
                    printf("You decide not to open the chest.\n");
                }
                break;

            case 2:
                printf("You take the path in the middle.\n");
                printf("You find yourself in a large room with many doors.\n");
                printf("Which door do you choose? ");
                scanf("%d", &choice);

                if(choice == 1) {
                    printf("You open the door and find a pile of gold!\n");
                    treasure += 100;
                }
                else if(choice == 2) {
                    printf("You open the door and find a trap!\n");
                    health -= 50;
                }
                else {
                    printf("You open the door and find nothing.\n");
                }
                break;

            case 3:
                printf("You take the path to the right.\n");
                printf("You find yourself in a dark room with a monster!\n");
                printf("Do you fight the monster? (y/n) ");
                scanf(" %c", &answer);

                if(answer == 'y' || answer == 'Y') {
                    printf("You fight the monster and win!\n");
                    health += 25;
                }
                else {
                    printf("You decide not to fight the monster.\n");
                }
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    if(health <= 0) {
        printf("You have died.\n");
    }
    else if(treasure > 0) {
        printf("You have found %d pieces of treasure!\n", treasure);
    }
    else {
        printf("You have not found any treasure.\n");
    }

    return 0;
}