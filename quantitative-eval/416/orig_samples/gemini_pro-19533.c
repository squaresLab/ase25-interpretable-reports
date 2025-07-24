//GEMINI-pro DATASET v1.0 Category: Electronic Voting System ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_CANDIDATES 5

// Structure to represent a candidate
typedef struct {
    char *name;
    int votes;
} candidate_t;

// Array of candidates
candidate_t candidates[NUM_CANDIDATES] = {
    {"Alice", 0},
    {"Bob", 0},
    {"Carol", 0},
    {"Dave", 0},
    {"Eve", 0}
};

// Mutex to protect the candidates array
pthread_mutex_t candidates_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to cast a vote for a candidate
void *cast_vote(void *arg) {
    // Get the candidate ID from the argument
    int candidate_id = *(int *)arg;

    // Lock the candidates array
    pthread_mutex_lock(&candidates_mutex);

    // Increment the vote count for the candidate
    candidates[candidate_id].votes++;

    // Unlock the candidates array
    pthread_mutex_unlock(&candidates_mutex);

    return NULL;
}

int main() {
    // Create an array of thread IDs
    pthread_t thread_ids[NUM_CANDIDATES];

    // Create a thread for each candidate
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        pthread_create(&thread_ids[i], NULL, cast_vote, &i);
    }

    // Join all the threads
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    // Print the results of the election
    printf("Election results:\n");
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        printf("%s: %d votes\n", candidates[i].name, candidates[i].votes);
    }

    return 0;
}