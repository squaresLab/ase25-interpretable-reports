//Falcon-180B DATASET v1.0 Category: Building a HTTP Client ; Style: recursive
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    char *data = (char*)malloc(realsize + 1);
    memcpy(data, contents, realsize);
    data[realsize] = '\0';
    printf("%s", data);
    free(data);
    return realsize;
}

int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    int id = *((int*)clientp);
    printf("Thread %d: Download progress: %ld of %ld bytes (%d%%)\n", id, dlnow, dltotal, (int)(dlnow * 100 / dltotal));
    return 0;
}

void *thread_func(void *arg)
{
    CURL *curl;
    CURLcode res;
    int id = *((int*)arg);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        char url[100];
        sprintf(url, "https://example.com/file_%d.txt", id);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &id);
        res = curl_easy_perform(curl);

        if(res!= CURLE_OK)
            fprintf(stderr, "Thread %d: Download failed: %s\n", id, curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    return NULL;
}

int main(int argc, char** argv)
{
    int num_threads = 5;
    pthread_t threads[num_threads];

    for(int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread_func, &i);
    }

    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    curl_global_cleanup();
    return 0;
}