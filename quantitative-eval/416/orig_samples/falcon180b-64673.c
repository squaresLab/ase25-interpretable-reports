//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: Donald Knuth
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int id;
    void *(*func)(void *);
    void *arg;
} thread_data_t;

void *thread_func(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    data->func(data->arg);
    return NULL;
}

int create_thread(pthread_t *thread, const char *name,
                  void *(*func)(void *), void *arg) {
    int ret;
    thread_data_t data;

    data.id = *thread;
    data.func = func;
    data.arg = arg;

    ret = pthread_create(thread, NULL, thread_func, &data);
    if (ret!= 0) {
        fprintf(stderr, "Error: unable to create thread, %s\n", strerror(ret));
        return ret;
    }

    ret = pthread_setname_np(*thread, name);
    if (ret!= 0) {
        fprintf(stderr, "Error: unable to set thread name, %s\n", strerror(ret));
        return ret;
    }

    return 0;
}

int join_thread(pthread_t thread) {
    int ret;

    ret = pthread_join(thread, NULL);
    if (ret!= 0) {
        fprintf(stderr, "Error: unable to join thread, %s\n", strerror(ret));
        return ret;
    }

    return 0;
}

int main() {
    pthread_t threads[3];
    char names[3][16] = {"thread1", "thread2", "thread3"};
    int i, ret;

    for (i = 0; i < 3; i++) {
        ret = create_thread(&threads[i], names[i], thread_func, (void *)i);
        if (ret!= 0) {
            return ret;
        }
    }

    for (i = 0; i < 3; i++) {
        ret = join_thread(threads[i]);
        if (ret!= 0) {
            return ret;
        }
    }

    return 0;
}