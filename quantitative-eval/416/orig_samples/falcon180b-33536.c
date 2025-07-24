//Falcon-180B DATASET v1.0 Category: Browser Plugin ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <curl/curl.h>

#define MAX_THREADS 10
#define MAX_URL_SIZE 2048

struct thread_data {
    char url[MAX_URL_SIZE];
    FILE *file;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void *download_url(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, data->url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data->file);

        res = curl_easy_perform(curl);
        if(res!= CURLE_OK) {
            fprintf(stderr, "Thread %d: Download failed: %s\n", (int)pthread_self(), curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    FILE *input_file;
    char line[MAX_URL_SIZE];
    pthread_t threads[MAX_THREADS];
    int num_threads = 0;

    if(argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    input_file = fopen(argv[1], "r");
    if(!input_file) {
        fprintf(stderr, "Error opening input file.\n");
        return 1;
    }

    while(fgets(line, MAX_URL_SIZE, input_file)!= NULL) {
        if(num_threads >= MAX_THREADS) {
            fprintf(stderr, "Maximum number of threads reached.\n");
            break;
        }

        struct thread_data data;
        strcpy(data.url, line);
        data.file = input_file;

        pthread_create(&threads[num_threads], NULL, download_url, &data);
        num_threads++;
    }

    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(input_file);
    return 0;
}