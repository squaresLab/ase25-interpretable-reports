//GEMINI-pro DATASET v1.0 Category: Movie Rating System ; Style: ultraprecise
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the movie rating system
#define G "G"
#define PG "PG"
#define PG13 "PG-13"
#define R "R"
#define NC17 "NC-17"

// Define the age groups
#define KIDS 0
#define TEENS 1
#define ADULTS 2

// Get the user's age group
int getAgeGroup() {
  int age;
  printf("Enter your age: ");
  scanf("%d", &age);

  if (age < 13) {
    return KIDS;
  } else if (age < 18) {
    return TEENS;
  } else {
    return ADULTS;
  }
}

// Get the movie rating
char* getMovieRating() {
  char rating[4];
  printf("Enter the movie rating: ");
  scanf("%s", rating);

  return rating;
}

// Check if the movie is appropriate for the user's age group
int isMovieAppropriate(int ageGroup, char* rating) {
  if (ageGroup == KIDS) {
    return strcmp(rating, G) == 0;
  } else if (ageGroup == TEENS) {
    return strcmp(rating, G) == 0 || strcmp(rating, PG) == 0 || strcmp(rating, PG13) == 0;
  } else {
    return strcmp(rating, G) == 0 || strcmp(rating, PG) == 0 || strcmp(rating, PG13) == 0 || strcmp(rating, R) == 0;
  }
}

// Print the movie rating system
void printMovieRatingSystem() {
  printf("Movie Rating System:\n");
  printf("G: General audiences. No restrictions.\n");
  printf("PG: Parental guidance suggested. Some material may not be suitable for children.\n");
  printf("PG-13: Parents strongly cautioned. Some material may be inappropriate for children under 13.\n");
  printf("R: Restricted. Under 17 requires accompanying parent or adult guardian.\n");
  printf("NC-17: No one 17 and under admitted.\n");
}

// Main function
int main() {
  // Print the movie rating system
  printMovieRatingSystem();

  // Get the user's age group
  int ageGroup = getAgeGroup();

  // Get the movie rating
  char* rating = getMovieRating();

  // Check if the movie is appropriate for the user's age group
  int isAppropriate = isMovieAppropriate(ageGroup, rating);

  // Print the result
  if (isAppropriate) {
    printf("The movie is appropriate for your age group.\n");
  } else {
    printf("The movie is not appropriate for your age group.\n");
  }

  return 0;
}