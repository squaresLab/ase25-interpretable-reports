//GEMINI-pro DATASET v1.0 Category: Bitwise operations ; Style: medieval
#include <stdio.h>
#include <stdlib.h>

int main() {

  // Declare ye variables, brave adventurer!
  unsigned int n, m, result;
  int choice;

  // A hearty welcome to the land of bitwise operations!
  printf("Welcome to the realm of bitwise operations, fair traveler!\n");
  printf("Choose thy weapon (bitwise operation):\n");
  printf("1. AND (&)\n");
  printf("2. OR (|)\n");
  printf("3. XOR (^)\n");
  printf("4. NOT (~)\n");
  printf("5. Left Shift (<<)\n");
  printf("6. Right Shift (>>)\n");
  printf("Enter thy choice, valiant one: ");
  scanf("%d", &choice);

  // Receive the numbers thou wishest to operate upon
  printf("Enter the first number, brave soul: ");
  scanf("%u", &n);
  printf("Enter the second number, valiant warrior: ");
  scanf("%u", &m);

  // Perform the chosen operation upon the numbers
  switch (choice) {
    case 1:
      result = n & m;
      printf("The AND of %u and %u is %u\n", n, m, result);
      break;
    case 2:
      result = n | m;
      printf("The OR of %u and %u is %u\n", n, m, result);
      break;
    case 3:
      result = n ^ m;
      printf("The XOR of %u and %u is %u\n", n, m, result);
      break;
    case 4:
      result = ~n;
      printf("The NOT of %u is %u\n", n, result);
      break;
    case 5:
      printf("Enter the number of bits to shift left: ");
      int shift;
      scanf("%d", &shift);
      result = n << shift;
      printf("The left shift of %u by %d is %u\n", n, shift, result);
      break;
    case 6:
      printf("Enter the number of bits to shift right: ");
      scanf("%d", &shift);
      result = n >> shift;
      printf("The right shift of %u by %d is %u\n", n, shift, result);
      break;
    default:
      printf("Invalid choice, brave soul. Try again.\n");
  }

  // Bid farewell to the valiant adventurer
  printf("May thy quest for knowledge be fruitful, brave adventurer!\n");
  return 0;
}