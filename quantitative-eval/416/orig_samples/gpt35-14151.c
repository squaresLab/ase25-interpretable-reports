//FormAI DATASET v1.0 Category: Smart home light control ; Style: real-life
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_BULBS 10

// Bulb struct definition
typedef struct {
  int id;
  bool isOn;
  int brightness;
} Bulb;

// Function to create a new Bulb
Bulb createBulb(int id) {
  Bulb bulb = {id, false, 0};
  return bulb;
}

// Function to turn on/off a bulb
void toggleBulb(Bulb* b) {
  b->isOn = !b->isOn;
  printf("Bulb %d is now %s\n", b->id, b->isOn ? "on" : "off");
}

// Function to set the brightness level of a bulb
void setBrightness(Bulb* b, int brightness) {
  b->brightness = brightness;
  printf("Bulb %d brightness set to %d%%\n", b->id, b->brightness);
}

int main() {
  Bulb bulbs[MAX_BULBS];

  // Initialize the bulbs
  for (int i = 0; i < MAX_BULBS; i++) {
    bulbs[i] = createBulb(i+1);
  }

  // Welcome message
  printf("Welcome to Smart Home Light Control!\n");

  // Main loop
  while (true) {
    // Print options
    printf("\nChoose an option:\n");
    printf("1 - Turn on/off a bulb\n");
    printf("2 - Set the brightness level of a bulb\n");
    printf("3 - Quit\n");

    // Get user input
    int option;
    scanf("%d", &option);

    switch (option) {
      case 1:
        // Turn on/off a bulb
        printf("\nEnter the bulb ID: ");
        int bulbId;
        scanf("%d", &bulbId);
        if (bulbId < 1 || bulbId > MAX_BULBS) {
          printf("Invalid bulb ID!\n");
          break;
        }
        toggleBulb(&bulbs[bulbId-1]);
        break;
      case 2:
        // Set the brightness level of a bulb
        printf("\nEnter the bulb ID: ");
        scanf("%d", &bulbId);
        if (bulbId < 1 || bulbId > MAX_BULBS) {
          printf("Invalid bulb ID!\n");
          break;
        }
        printf("Enter the brightness level (0-100): ");
        int brightness;
        scanf("%d", &brightness);
        if (brightness < 0 || brightness > 100) {
          printf("Invalid brightness level!\n");
          break;
        }
        setBrightness(&bulbs[bulbId-1], brightness);
        break;
      case 3:
        // Quit
        printf("\nGoodbye!\n\n");
        exit(0);
      default:
        printf("Invalid option!\n");
        break;
    }
  }

  return 0;
}