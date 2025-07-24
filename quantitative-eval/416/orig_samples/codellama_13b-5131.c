//Code Llama-13B DATASET v1.0 Category: Hexadecimal Converter ; Style: futuristic
// C Hexadecimal Converter
// Written in a futuristic style

#include <stdio.h>
#include <string.h>

// Define a function to convert hexadecimal to decimal
int hex_to_dec(char* hex) {
  int dec = 0;
  int i = 0;
  int len = strlen(hex);
  while (i < len) {
    int digit = hex[i] - '0';
    if (digit < 0 || digit > 9) {
      digit = (hex[i] - 'A' + 10) % 16;
    }
    dec = dec * 16 + digit;
    i++;
  }
  return dec;
}

// Define a function to convert decimal to hexadecimal
char* dec_to_hex(int dec) {
  char hex[10];
  int i = 0;
  while (dec > 0) {
    int digit = dec % 16;
    hex[i] = (digit < 10) ? '0' + digit : 'A' + digit - 10;
    i++;
    dec = dec / 16;
  }
  hex[i] = '\0';
  return hex;
}

// Define a function to convert hexadecimal to binary
char* hex_to_bin(char* hex) {
  char bin[10];
  int i = 0;
  int len = strlen(hex);
  while (i < len) {
    int digit = hex[i] - '0';
    if (digit < 0 || digit > 9) {
      digit = (hex[i] - 'A' + 10) % 16;
    }
    bin[i] = (digit < 2) ? '0' + digit : 'A' + digit - 2;
    i++;
  }
  bin[i] = '\0';
  return bin;
}

// Define a function to convert binary to hexadecimal
char* bin_to_hex(char* bin) {
  char hex[10];
  int i = 0;
  int len = strlen(bin);
  while (i < len) {
    int digit = bin[i] - '0';
    if (digit < 0 || digit > 1) {
      digit = (bin[i] - 'A' + 2) % 4;
    }
    hex[i] = (digit < 10) ? '0' + digit : 'A' + digit - 10;
    i++;
  }
  hex[i] = '\0';
  return hex;
}

// Define a function to convert hexadecimal to binary
char* hex_to_oct(char* hex) {
  char oct[10];
  int i = 0;
  int len = strlen(hex);
  while (i < len) {
    int digit = hex[i] - '0';
    if (digit < 0 || digit > 9) {
      digit = (hex[i] - 'A' + 10) % 16;
    }
    oct[i] = (digit < 8) ? '0' + digit : 'A' + digit - 8;
    i++;
  }
  oct[i] = '\0';
  return oct;
}

// Define a function to convert binary to hexadecimal
char* oct_to_hex(char* oct) {
  char hex[10];
  int i = 0;
  int len = strlen(oct);
  while (i < len) {
    int digit = oct[i] - '0';
    if (digit < 0 || digit > 7) {
      digit = (oct[i] - 'A' + 8) % 16;
    }
    hex[i] = (digit < 10) ? '0' + digit : 'A' + digit - 10;
    i++;
  }
  hex[i] = '\0';
  return hex;
}

// Main function
int main() {
  // Take input from the user
  char hex[10];
  printf("Enter a hexadecimal number: ");
  scanf("%s", hex);

  // Convert hexadecimal to decimal
  int dec = hex_to_dec(hex);
  printf("Decimal equivalent: %d\n", dec);

  // Convert decimal to hexadecimal
  char* hex2 = dec_to_hex(dec);
  printf("Hexadecimal equivalent: %s\n", hex2);

  // Convert hexadecimal to binary
  char* bin = hex_to_bin(hex);
  printf("Binary equivalent: %s\n", bin);

  // Convert binary to hexadecimal
  char* hex3 = bin_to_hex(bin);
  printf("Hexadecimal equivalent: %s\n", hex3);

  // Convert hexadecimal to octal
  char* oct = hex_to_oct(hex);
  printf("Octal equivalent: %s\n", oct);

  // Convert octal to hexadecimal
  char* hex4 = oct_to_hex(oct);
  printf("Hexadecimal equivalent: %s\n", hex4);

  return 0;
}