//Falcon-180B DATASET v1.0 Category: Data recovery tool ; Style: multivariable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <sys/mman.h>

#define BUF_SIZE 4096

typedef struct {
    char *filename;
    size_t filesize;
    int fd;
    void *data;
} file_t;

void *read_file(void *arg) {
    file_t *f = arg;
    char buf[BUF_SIZE];
    ssize_t n;
    size_t offset = 0;

    while ((n = read(f->fd, buf, BUF_SIZE)) > 0) {
        if (f->data == NULL) {
            f->data = malloc(f->filesize);
        }
        memcpy(f->data + offset, buf, n);
        offset += n;
    }

    if (n < 0) {
        perror("read");
    }

    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    file_t f;
    f.filename = argv[1];
    f.filesize = 0;
    f.fd = open(f.filename, O_RDONLY);
    if (f.fd == -1) {
        perror("open");
        return 1;
    }

    struct stat st;
    if (fstat(f.fd, &st) == -1) {
        perror("fstat");
        return 1;
    }

    f.filesize = st.st_size;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, read_file, &f);

    void *data = NULL;
    size_t size = 0;

    while (f.data == NULL) {
        usleep(100000);
    }

    data = f.data;
    size = f.filesize;

    close(f.fd);

    printf("Data recovery successful!\n");
    printf("File size: %zu bytes\n", size);

    return 0;
}