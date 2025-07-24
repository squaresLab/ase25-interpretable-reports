//Code Llama-13B DATASET v1.0 Category: Hexadecimal Converter ; Style: accurate
// Hexadecimal Converter
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to convert hexadecimal to decimal
int hex_to_dec(char *hex) {
  int len = strlen(hex);
  int dec = 0;
  int i;

  for (i = 0; i < len; i++) {
    if (hex[i] >= '0' && hex[i] <= '9') {
      dec += (hex[i] - '0') * pow(16, len - i - 1);
    } else if (hex[i] >= 'A' && hex[i] <= 'F') {
      dec += (hex[i] - 'A' + 10) * pow(16, len - i - 1);
    } else if (hex[i] >= 'a' && hex[i] <= 'f') {
      dec += (hex[i] - 'a' + 10) * pow(16, len - i - 1);
    }
  }

  return dec;
}

// Function to convert decimal to hexadecimal
char *dec_to_hex(int dec) {
  int i;
  char hex[10];
  int len = 0;

  while (dec > 0) {
    if (dec % 16 <= 9) {
      hex[len++] = dec % 16 + '0';
    } else {
      hex[len++] = (dec % 16 - 10) + 'A';
    }
    dec /= 16;
  }

  hex[len] = '\0';
  return hex;
}

int main() {
  int dec;
  char hex[10];

  // Convert hexadecimal to decimal
  printf("Enter a hexadecimal number: ");
  scanf("%s", hex);
  dec = hex_to_dec(hex);
  printf("Decimal value: %d\n", dec);

  // Convert decimal to hexadecimal
  printf("Enter a decimal number: ");
  scanf("%d", &dec);
  strcpy(hex, dec_to_hex(dec));
  printf("Hexadecimal value: %s\n", hex);

  return 0;
}