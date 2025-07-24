//GEMINI-pro DATASET v1.0 Category: Database querying ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sqlite3.h>

// Database file name
#define DB_FILE "mydb.db"

// Number of threads
#define NUM_THREADS 4

// Query to execute
#define QUERY "SELECT * FROM users"

// Mutex to protect shared data
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function
void *thread_func(void *arg) {
    // Get the thread number
    int thread_num = *(int *)arg;

    // Connect to the database
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    // Prepare the query
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, QUERY, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    // Execute the query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Get the column values
        int id = sqlite3_column_int(stmt, 0);
        char *name = (char *)sqlite3_column_text(stmt, 1);

        // Print the results
        pthread_mutex_lock(&mutex);
        printf("Thread %d: id=%d, name=%s\n", thread_num, id, name);
        pthread_mutex_unlock(&mutex);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return NULL;
}

int main() {
    // Create the threads
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, &i);
    }

    // Wait for the threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}