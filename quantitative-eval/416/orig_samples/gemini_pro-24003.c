//GEMINI-pro DATASET v1.0 Category: Cryptographic hash function ; Style: artistic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

// Function to convert a string to lowercase
char *toLower(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 32;
        }
    }
    return str;
}

// Function to calculate the MD5 hash of a string
unsigned char *md5(char *str) {
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, str, strlen(str));
    MD5_Final(hash, &ctx);
    return hash;
}

// Function to print the MD5 hash of a string
void printHash(unsigned char *hash) {
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main() {
    // Get the input string from the user
    char str[100];
    printf("Enter a string: ");
    scanf("%s", str);

    // Convert the input string to lowercase
    toLower(str);

    // Calculate the MD5 hash of the input string
    unsigned char *hash = md5(str);

    // Print the MD5 hash of the input string
    printf("MD5 hash: ");
    printHash(hash);

    return 0;
}