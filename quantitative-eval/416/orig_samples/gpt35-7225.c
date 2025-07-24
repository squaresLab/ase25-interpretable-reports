//FormAI DATASET v1.0 Category: Palindrome Checker ; Style: funny
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

char removeSpecialChar(char s[], int len) {
  char newString[MAX_LEN];
  int j = 0;
  
  for (int i = 0; i < len; i++) {
    if (isalnum(s[i])) {
      newString[j] = tolower(s[i]);
      j++;
    }
  }
  
  newString[j] = '\0';
  return newString;
}

int isPalindrome(char s[]) {
  int len = strlen(s);
  
  for (int i = 0; i < len / 2; i++) {
    if (s[i] != s[len - i - 1]) {
      return 0;
    }
  }
  
  return 1;
}

int main() {
  char s[MAX_LEN];
  
  printf("Enter a string: ");
  fgets(s, MAX_LEN, stdin);
  
  int len = strlen(s);
  if (s[len - 1] == '\n') {
    s[len - 1] = '\0';
  }
  
  char newString[MAX_LEN];
  strcpy(newString, removeSpecialChar(s, len));
  
  if (isPalindrome(newString)) {
    printf("%s is a palindrome\n", s);
  } else {
    printf("%s is not a palindrome\n", s);
  }
  
  return 0;
}