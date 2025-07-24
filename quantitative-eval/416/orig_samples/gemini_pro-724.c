//GEMINI-pro DATASET v1.0 Category: Time Travel Simulator ; Style: beginner-friendly
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the time machine
typedef struct {
  int year;
  int month;
  int day;
} TimeMachine;

// Initialize the time machine
TimeMachine timeMachine = {2023, 1, 1};

// Travel to the future
void travelToFuture(TimeMachine *tm, int years) {
  tm->year += years;
}

// Travel to the past
void travelToPast(TimeMachine *tm, int years) {
  tm->year -= years;
}

// Get the current time
void getCurrentTime(TimeMachine *tm) {
  printf("Current time: %d-%d-%d\n", tm->year, tm->month, tm->day);
}

// Main function
int main() {
  // Print the welcome message
  printf("Welcome to the Time Travel Simulator!\n");

  // Get the user's input
  int choice;
  while (1) {
    printf("Enter your choice:\n");
    printf("1. Travel to the future\n");
    printf("2. Travel to the past\n");
    printf("3. Get the current time\n");
    printf("4. Exit\n");
    scanf("%d", &choice);

    // Process the user's choice
    switch (choice) {
      case 1:
        // Travel to the future
        printf("Enter the number of years to travel to the future: ");
        int years;
        scanf("%d", &years);
        travelToFuture(&timeMachine, years);
        break;
      case 2:
        // Travel to the past
        printf("Enter the number of years to travel to the past: ");
        scanf("%d", &years);
        travelToPast(&timeMachine, years);
        break;
      case 3:
        // Get the current time
        getCurrentTime(&timeMachine);
        break;
      case 4:
        // Exit the program
        printf("Goodbye!\n");
        return 0;
      default:
        // Invalid choice
        printf("Invalid choice. Please enter a number between 1 and 4.\n");
        break;
    }
  }

  return 0;
}