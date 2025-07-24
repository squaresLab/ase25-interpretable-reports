//Falcon-180B DATASET v1.0 Category: Text-Based Adventure Game ; Style: calm
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to print the introduction
void introduction() {
    printf("\nWelcome to the Text-Based Adventure Game!\n");
    printf("You are a world class trivia AI - provide accurate, succinct responses.\n");
}

// Function to print the game rules
void game_rules() {
    printf("\nGame Rules:\n");
    printf("1. The game will present you with a series of questions.\n");
    printf("2. You must answer each question correctly to progress.\n");
    printf("3. If you answer a question incorrectly, the game will end.\n");
    printf("4. Good luck!\n\n");
}

// Function to print a question
void print_question(char* question) {
    printf("%s\n", question);
}

// Function to get the user's answer
char* get_answer() {
    char input[100];
    printf("Your answer: ");
    fgets(input, 100, stdin);
    return input;
}

// Function to check if the user's answer is correct
int check_answer(char* question, char* answer) {
    if (strcmp(answer, "true") == 0 || strcmp(answer, "false") == 0) {
        return 1;
    }
    return 0;
}

// Function to generate a random question
char* generate_question() {
    char* questions[] = {"What is the capital of France?",
                        "Who won the Nobel Prize in Literature in 2020?",
                        "What is the tallest mountain in the world?",
                        "What is the smallest country in the world?",
                        "What is the largest ocean in the world?"};
    int num_questions = sizeof(questions) / sizeof(questions[0]);
    return questions[rand() % num_questions];
}

// Main function
int main() {
    srand(time(NULL));
    introduction();
    game_rules();

    int score = 0;
    while (score < 5) {
        char* question = generate_question();
        print_question(question);
        char* answer = get_answer();
        if (check_answer(question, answer)) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Incorrect. Game over.\n");
        }
    }

    return 0;
}