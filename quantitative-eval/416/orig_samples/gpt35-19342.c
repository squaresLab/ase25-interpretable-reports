//FormAI DATASET v1.0 Category: Password management ; Style: lively
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

// Function to encrypt the given password
char* encrypt_password(char password[])
{
    int i = 0;
    while (password[i] != '\0')
    {
        password[i] = toupper(password[i]);
        password[i] += 3;
        i++;
    }
    return password;
}

// Function to decrypt the given password
char* decrypt_password(char password[])
{
    int i = 0;
    while (password[i] != '\0')
    {
        password[i] = password[i] - 3;
        password[i] = tolower(password[i]);
        i++;
    }
    return password;
}

// Function to store the given password in a file
void store_password(char username[], char password[])
{
    FILE *fp;
    fp = fopen("passwords.txt", "a");
    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);
}

// Function to retrieve the password for the given username
char* retrieve_password(char username[])
{
    FILE *fp;
    char stored_username[MAX_USERNAME_LENGTH];
    char stored_password[MAX_PASSWORD_LENGTH];
    char* decrypted_password = (char*)malloc(MAX_PASSWORD_LENGTH * sizeof(char));
    fp = fopen("passwords.txt", "r");

    while (fscanf(fp, "%s %s", stored_username, stored_password) != EOF)
    {
        if (strcmp(username, stored_username) == 0)
        {
            decrypted_password = decrypt_password(stored_password);
            break;
        }
    }
    fclose(fp);
    return decrypted_password;
}

int main()
{
    int choice;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Welcome to Password Manager!\n");

    do 
    {
        printf("\n1. Store Password\n");
        printf("2. Get Password\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            {
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter password: ");
                scanf("%s", password);
                store_password(username, encrypt_password(password));
                printf("Password stored successfully!\n");
                break;
            }
            case 2:
            {
                printf("Enter username: ");
                scanf("%s", username);
                char* retrieved_password = retrieve_password(username);
                if (strlen(retrieved_password) != 0)
                {
                    printf("Password for %s is %s\n", username, retrieved_password);
                }
                else
                {
                    printf("Password not found for %s\n", username);
                }
                break;
            }
            case 3:
            {
                printf("Thank you for using Password Manager!\n");
                exit(0);
            }
            default:
            {
                printf("Invalid choice! Please try again.\n");
                break;
            }
        }

    } while(1);

    return 0;
}