//Falcon-180B DATASET v1.0 Category: Prime Number Generator ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define MAX_NUM 1000000

int is_prime(int num) {
    if (num < 2) {
        return 0;
    }
    int sqrt_num = sqrt(num);
    for (int i = 2; i <= sqrt_num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

void *generate_primes(void *arg) {
    int start = *(int *) arg;
    int end = start + 100;
    int count = 0;

    for (int i = start; i < end; i++) {
        if (is_prime(i)) {
            printf("%d ", i);
            count++;
        }
    }

    pthread_exit(0);
}

int main() {
    pthread_t threads[10];
    int ids[10];
    int num_threads = 10;
    int chunk_size = MAX_NUM / num_threads;

    for (int i = 0; i < num_threads; i++) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1)? MAX_NUM : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, generate_primes, &start);
        ids[i] = i;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}