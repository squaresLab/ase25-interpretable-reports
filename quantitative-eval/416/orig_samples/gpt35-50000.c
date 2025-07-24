//FormAI DATASET v1.0 Category: Text processing ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#define BUF_SIZE 1024

typedef struct {
    int fd;
    char *buf;
    size_t len;
} read_request;

typedef struct {
    int fd;
    char *buf;
    ssize_t len;
} read_response;

bool should_stop = false;

void signal_handler(int sig) {
    should_stop = true;
}

void *read_file(void *arg) {
    read_request req = *(read_request*)arg;
    read_response res;
    res.fd = req.fd;
    res.buf = (char*) malloc(req.len);
    res.len = 0;
    
    while (true) {
        ssize_t read_count = read(res.fd, res.buf + res.len, req.len - res.len);
        if (read_count < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("read");
            break;
        }
        if (read_count == 0) {
            break;
        }
        res.len += read_count;
    }
    
    return (void*) &res;
}

void *process_data(void *arg) {
    read_response res = *(read_response*)arg;
    char *buf = res.buf;
    ssize_t len = res.len;
    
    // Process data here
    printf("Data read from file:\n%s\n", buf);
    
    free(buf);
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    signal(SIGINT, signal_handler);
    
    pthread_t read_thread;
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }
    read_request req = { fd, (char*) malloc(BUF_SIZE), BUF_SIZE };
    if (pthread_create(&read_thread, NULL, read_file, (void*) &req) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }
    
    while (!should_stop) {
        void *response;
        if (pthread_tryjoin_np(read_thread, &response) == 0) {
            pthread_t process_thread;
            if (pthread_create(&process_thread, NULL, process_data, response) != 0) {
                perror("pthread_create");
                return EXIT_FAILURE;
            }
            
            close(fd);
            return EXIT_SUCCESS;
        }
        
        sleep(1);
    }
    
    close(fd);
    return EXIT_SUCCESS;
}