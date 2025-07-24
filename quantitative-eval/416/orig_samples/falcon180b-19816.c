//Falcon-180B DATASET v1.0 Category: File Encyptor ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 4096
#define NUM_THREADS 4

pthread_t threads[NUM_THREADS];
char buffer[BUFFER_SIZE];

void* encrypt_thread(void* arg) {
  int start = *(int*)arg;
  int end = start + BUFFER_SIZE;

  for (int i = start; i < end; i++) {
    buffer[i] ^= 0xAA;
  }

  return NULL;
}

int main() {
  FILE* input_file = fopen("input.txt", "r");
  if (input_file == NULL) {
    printf("Error opening input file\n");
    return 1;
  }

  FILE* output_file = fopen("output.txt", "w");
  if (output_file == NULL) {
    printf("Error opening output file\n");
    return 1;
  }

  int file_size = 0;
  char c;
  while ((c = fgetc(input_file))!= EOF) {
    buffer[file_size++] = c;
  }

  time_t start_time = time(NULL);
  int chunk_size = file_size / NUM_THREADS;

  for (int i = 0; i < NUM_THREADS; i++) {
    int start = i * chunk_size;
    int end = (i == NUM_THREADS - 1)? file_size : (i + 1) * chunk_size;

    pthread_create(&threads[i], NULL, encrypt_thread, (void*)&start);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  time_t end_time = time(NULL);
  double elapsed_time = difftime(end_time, start_time);

  fprintf(output_file, "Encryption completed in %.2f seconds\n", elapsed_time);

  fclose(input_file);
  fclose(output_file);

  return 0;
}