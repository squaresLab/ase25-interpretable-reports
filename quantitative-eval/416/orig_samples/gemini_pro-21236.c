//GEMINI-pro DATASET v1.0 Category: Online Examination System ; Style: optimized
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store question and answer
struct Question {
    char question[256];
    char options[4][64];
    char answer[64];
};

// Array of questions
struct Question questions[] = {
    {
        "What is the capital of India?",
        {"Delhi", "Mumbai", "Chennai", "Kolkata"},
        "Delhi"
    },
    {
        "Who is the current Prime Minister of India?",
        {"Narendra Modi", "Rahul Gandhi", "Arvind Kejriwal", "Mamata Banerjee"},
        "Narendra Modi"
    },
    {
        "What is the national animal of India?",
        {"Tiger", "Lion", "Elephant", "Leopard"},
        "Tiger"
    },
    {
        "What is the national bird of India?",
        {"Peacock", "Parrot", "Eagle", "Owl"},
        "Peacock"
    },
    {
        "What is the national flower of India?",
        {"Lotus", "Rose", "Jasmine", "Marigold"},
        "Lotus"
    }
};

// Function to display the question and options
void displayQuestion(struct Question question) {
    printf("%s\n", question.question);
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, question.options[i]);
    }
}

// Function to get the user's answer
char* getAnswer() {
    char answer[64];
    scanf("%s", answer);
    return answer;
}

// Function to check if the answer is correct
int checkAnswer(char* answer, char* correctAnswer) {
    return strcmp(answer, correctAnswer) == 0;
}

// Function to display the result
void displayResult(int score) {
    printf("Your score is %d out of %d\n", score, sizeof(questions) / sizeof(struct Question));
}

// Main function
int main() {
    // Initialize the score
    int score = 0;

    // Loop through the questions
    for (int i = 0; i < sizeof(questions) / sizeof(struct Question); i++) {
        // Display the question and options
        displayQuestion(questions[i]);

        // Get the user's answer
        char* answer = getAnswer();

        // Check if the answer is correct
        if (checkAnswer(answer, questions[i].answer)) {
            // Increment the score
            score++;
        }
    }

    // Display the result
    displayResult(score);

    return 0;
}