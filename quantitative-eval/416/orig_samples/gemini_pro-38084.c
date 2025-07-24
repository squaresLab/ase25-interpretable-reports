//GEMINI-pro DATASET v1.0 Category: Online Examination System ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Structure to store the question and answer
typedef struct {
    char question[100];
    char answer[100];
} Question;

// Array of questions
Question questions[] = {
    {"What is the capital of France?", "Paris"},
    {"What is the largest ocean in the world?", "Pacific Ocean"},
    {"What is the name of the tallest mountain in the world?", "Mount Everest"},
    {"What is the chemical symbol for gold?", "Au"},
    {"Who is the author of the Harry Potter series?", "J.K. Rowling"}
};

// Number of questions
int num_questions = sizeof(questions) / sizeof(Question);

// Mutex to protect the shared variable
pthread_mutex_t mutex;

// Thread function
void *thread_func(void *arg) {
    int thread_id = *(int *)arg;

    // Acquire the mutex lock
    pthread_mutex_lock(&mutex);

    // Get the question and answer for this thread
    Question question = questions[thread_id];

    // Release the mutex lock
    pthread_mutex_unlock(&mutex);

    // Print the question and answer
    printf("Thread %d: %s\n", thread_id, question.question);
    printf("Thread %d: %s\n", thread_id, question.answer);

    return NULL;
}

int main() {
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create an array of threads
    pthread_t threads[num_questions];

    // Create the threads
    for (int i = 0; i < num_questions; i++) {
        pthread_create(&threads[i], NULL, thread_func, &i);
    }

    // Join the threads
    for (int i = 0; i < num_questions; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}