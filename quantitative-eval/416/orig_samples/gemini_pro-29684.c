//GEMINI-pro DATASET v1.0 Category: Tower of Hanoi Problem ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Number of disks
#define DISKS 3

// Towers
int towers[3] = {0};

// Mutex to protect towers
pthread_mutex_t tower_mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable to signal when a tower is empty
pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;

// Thread function to move a disk
void *move_disk(void *arg) {
  int source = *(int *)arg;
  int dest;

  // Acquire the mutex
  pthread_mutex_lock(&tower_mutex);

  // Wait for the source tower to have a disk
  while (towers[source] == 0) {
    pthread_cond_wait(&empty_cond, &tower_mutex);
  }

  // Move the disk to the destination tower
  for (dest = 0; dest < 3; dest++) {
    if (dest != source && towers[dest] == 0) {
      towers[dest] = towers[source];
      towers[source] = 0;
      break;
    }
  }

  // Signal that the destination tower is no longer empty
  pthread_cond_signal(&empty_cond);

  // Release the mutex
  pthread_mutex_unlock(&tower_mutex);

  return NULL;
}

int main() {
  // Create the threads
  pthread_t threads[DISKS];

  // Initialize the towers
  towers[0] = DISKS;

  // Start the threads
  for (int i = 0; i < DISKS; i++) {
    pthread_create(&threads[i], NULL, move_disk, &i);
  }

  // Join the threads
  for (int i = 0; i < DISKS; i++) {
    pthread_join(threads[i], NULL);
  }

  // Print the final state of the towers
  for (int i = 0; i < DISKS; i++) {
    printf("%d ", towers[i]);
  }
  printf("\n");

  return 0;
}