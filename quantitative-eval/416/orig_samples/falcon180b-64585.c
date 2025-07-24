//Falcon-180B DATASET v1.0 Category: URL Sanitizer ; Style: post-apocalyptic
/* =======================================================================
   Post-Apocalyptic C URL Sanitizer
   -----------------------------------------------------------------------
   This program will take a URL as input and sanitize it to remove any
   potentially malicious characters. In this post-apocalyptic world, we
   can't afford to take any chances with our security. */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* =======================================================================
   Sanitize a single character
   -----------------------------------------------------------------------
   This function will take a single character as input and return a
   sanitized version of it, or NULL if the character is not valid. */

char* sanitize_char(char c) {
    if (isalnum(c)) {
        return &c;
    } else if (c == '.' || c == '-' || c == '_') {
        return &c;
    } else {
        return NULL;
    }
}

/* =======================================================================
   Sanitize a URL
   -----------------------------------------------------------------------
   This function will take a URL as input and return a sanitized version
   of it, or NULL if the URL is not valid. */

char* sanitize_url(char* url) {
    int i, j;
    char* sanitized_url = malloc(strlen(url) + 1);

    if (sanitized_url == NULL) {
        return NULL;
    }

    for (i = 0, j = 0; url[i]!= '\0'; i++) {
        char* sanitized_char = sanitize_char(url[i]);

        if (sanitized_char!= NULL) {
            sanitized_url[j++] = *sanitized_char;
        }
    }

    sanitized_url[j] = '\0';

    return sanitized_url;
}

/* =======================================================================
   Main function
   -----------------------------------------------------------------------
   This is the entry point of our program. It will take a URL as input,
   sanitize it, and print the sanitized version to the console. */

int main(int argc, char* argv[]) {
    if (argc!= 2) {
        printf("Usage: %s <URL>\n", argv[0]);
        return 1;
    }

    char* url = argv[1];
    char* sanitized_url = sanitize_url(url);

    if (sanitized_url!= NULL) {
        printf("Sanitized URL: %s\n", sanitized_url);
        free(sanitized_url);
    } else {
        printf("Invalid URL\n");
    }

    return 0;
}