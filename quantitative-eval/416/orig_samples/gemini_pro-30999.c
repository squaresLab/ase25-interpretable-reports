//GEMINI-pro DATASET v1.0 Category: Cat Language Translator ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Define the cat language dictionary
const char *cat_dict[] = {
    "meow", "food",
    "purr", "happy",
    "hiss", "angry",
    "mew", "sad",
    "chirp", "excited"
};

// Define the number of cat words in the dictionary
const int num_cat_words = sizeof(cat_dict) / sizeof(char *);

// Define the maximum length of a cat word
const int max_cat_word_len = 5;

// Define the number of threads to use
const int num_threads = 4;

// Define the mutex to protect the shared data
pthread_mutex_t mutex;

// Define the condition variable to signal when the shared data is ready
pthread_cond_t cond;

// Define the shared data
char *cat_word;
int cat_word_len;

// Define the thread function
void *translate_cat_word(void *arg) {
    // Get the thread number
    int thread_num = *(int *)arg;

    // Loop through the cat words
    for (int i = thread_num; i < num_cat_words; i += num_threads) {
        // Get the cat word
        cat_word = cat_dict[i];

        // Get the cat word length
        cat_word_len = strlen(cat_word);

        // Lock the mutex
        pthread_mutex_lock(&mutex);

        // Signal that the shared data is ready
        pthread_cond_signal(&cond);

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// Define the main function
int main() {
    // Initialize the mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Create the threads
    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, translate_cat_word, (void *)&i);
    }

    // Loop until all the cat words have been translated
    while (1) {
        // Lock the mutex
        pthread_mutex_lock(&mutex);

        // Wait for the shared data to be ready
        pthread_cond_wait(&cond, &mutex);

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);

        // Print the cat word
        printf("%s\n", cat_word);

        // Check if all the cat words have been translated
        if (cat_word_len == 0) {
            break;
        }
    }

    // Join the threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}