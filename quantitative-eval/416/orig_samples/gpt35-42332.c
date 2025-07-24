//FormAI DATASET v1.0 Category: Database querying ; Style: configurable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[30];
    int age;
    char gender;
    char address[100];
} Person;

int main() {
    FILE* fp;
    char filename[100];
    int choice;
    int id;
    char name[30], address[100];
    int age;
    char gender;

    printf("Enter the name of the file to use: ");
    scanf("%s", filename);

    fp = fopen(filename, "rb+");
    if (fp == NULL) {
        printf("Failed to open file\n");
        exit(1);
    }

    while (1) {
        printf("\nMENU\n");
        printf("1. Add a person\n");
        printf("2. Search for a person by ID\n");
        printf("3. Search for a person by name\n");
        printf("4. Search for a person by age\n");
        printf("5. Search for a person by gender\n");
        printf("6. Search for a person by address\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the person's ID: ");
                scanf("%d", &id);
                printf("Enter the person's name: ");
                scanf("%s", name);
                printf("Enter the person's age: ");
                scanf("%d", &age);
                printf("Enter the person's gender: ");
                scanf(" %c", &gender);
                printf("Enter the person's address: ");
                scanf("%s", address);

                fseek(fp, 0, SEEK_END);

                Person p = { id, name, age, gender, address };

                fwrite(&p, sizeof(Person), 1, fp);
                printf("Person added successfully\n");
                break;

            case 2:
                printf("Enter the person's ID: ");
                scanf("%d", &id);
                rewind(fp);

                while (fread(&p, sizeof(Person), 1, fp)) {
                    if (p.id == id) {
                        printf("ID: %d\n", p.id);
                        printf("Name: %s\n", p.name);
                        printf("Age: %d\n", p.age);
                        printf("Gender: %c\n", p.gender);
                        printf("Address: %s\n", p.address);
                        break;
                    }
                }
                break;

            case 3:
                printf("Enter the person's name: ");
                scanf("%s", name);
                rewind(fp);

                while (fread(&p, sizeof(Person), 1, fp)) {
                    if (strcmp(p.name, name) == 0) {
                        printf("ID: %d\n", p.id);
                        printf("Name: %s\n", p.name);
                        printf("Age: %d\n", p.age);
                        printf("Gender: %c\n", p.gender);
                        printf("Address: %s\n", p.address);
                    }
                }
                break;

            case 4:
                printf("Enter the person's age: ");
                scanf("%d", &age);
                rewind(fp);

                while (fread(&p, sizeof(Person), 1, fp)) {
                    if (p.age == age) {
                        printf("ID: %d\n", p.id);
                        printf("Name: %s\n", p.name);
                        printf("Age: %d\n", p.age);
                        printf("Gender: %c\n", p.gender);
                        printf("Address: %s\n", p.address);
                    }
                }
                break;

            case 5:
                printf("Enter the person's gender: ");
                scanf(" %c", &gender);
                rewind(fp);

                while (fread(&p, sizeof(Person), 1, fp)) {
                    if (p.gender == gender) {
                        printf("ID: %d\n", p.id);
                        printf("Name: %s\n", p.name);
                        printf("Age: %d\n", p.age);
                        printf("Gender: %c\n", p.gender);
                        printf("Address: %s\n", p.address);
                    }
                }
                break;

            case 6:
                printf("Enter the person's address: ");
                scanf("%s", address);
                rewind(fp);

                while (fread(&p, sizeof(Person), 1, fp)) {
                    if (strcmp(p.address, address) == 0) {
                        printf("ID: %d\n", p.id);
                        printf("Name: %s\n", p.name);
                        printf("Age: %d\n", p.age);
                        printf("Gender: %c\n", p.gender);
                        printf("Address: %s\n", p.address);
                    }
                }
                break;

            case 7:
                return 0;
                break;

            default:
                printf("Invalid choice\n");
                break;
        }
    }

    fclose(fp);
    return 0;
}