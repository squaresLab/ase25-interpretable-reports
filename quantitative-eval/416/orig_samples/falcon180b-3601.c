//Falcon-180B DATASET v1.0 Category: Cryptography Implementation ; Style: futuristic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 100

char *encrypt(char *message, char *key) {
    char result[SIZE];
    int i, j, k;
    for (i = 0, j = 0; message[i]!= '\0'; i++) {
        result[i] = message[i] + key[j];
        if (result[i] > 'z') {
            result[i] -= 26;
        } else if (result[i] < 'a') {
            result[i] += 26;
        }
        j++;
        if (j == strlen(key)) {
            j = 0;
        }
    }
    result[i] = '\0';
    return result;
}

char *decrypt(char *message, char *key) {
    char result[SIZE];
    int i, j, k;
    for (i = 0, j = 0; message[i]!= '\0'; i++) {
        result[i] = message[i] - key[j];
        if (result[i] > 'z') {
            result[i] -= 26;
        } else if (result[i] < 'a') {
            result[i] += 26;
        }
        j++;
        if (j == strlen(key)) {
            j = 0;
        }
    }
    result[i] = '\0';
    return result;
}

int main() {
    char message[SIZE];
    char key[SIZE];
    char *encrypted;
    char *decrypted;
    int choice;
    do {
        printf("\nWelcome to the Future Cryptography System!\n");
        printf("1. Encrypt Message\n2. Decrypt Message\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter your message: ");
                scanf("%s", message);
                printf("Enter your key: ");
                scanf("%s", key);
                encrypted = encrypt(message, key);
                printf("\nEncrypted Message: %s\n", encrypted);
                break;
            case 2:
                printf("Enter your encrypted message: ");
                scanf("%s", encrypted);
                printf("Enter your key: ");
                scanf("%s", key);
                decrypted = decrypt(encrypted, key);
                printf("\nDecrypted Message: %s\n", decrypted);
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    } while (1);
    return 0;
}