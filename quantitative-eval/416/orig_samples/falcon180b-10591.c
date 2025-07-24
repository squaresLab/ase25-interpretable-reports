//Falcon-180B DATASET v1.0 Category: Building a IMAP Client ; Style: authentic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>
#include <openssl/ssl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    char *data = (char*)contents;
    fwrite(data, realsize, 1, stdout);
    return realsize;
}

static int ProgressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    int id = *((int*)clientp);
    printf("Thread %d: Download progress: %ld of %ld bytes (%ld%%)\n", id, dlnow, dltotal, (dlnow * 100) / dltotal);
    return 0;
}

void *ThreadFunction(void *param)
{
    CURL *curl;
    CURLcode res;
    char url[1024];
    int id = *((int*)param);

    sprintf(url, "imap://%s:%s@imap.gmail.com", "your_username", "your_password");

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &id);

        res = curl_easy_perform(curl);

        if(res!= CURLE_OK)
            fprintf(stderr, "Thread %d: Download failed: %s\n", id, curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}

int main()
{
    int num_threads = 4;
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, ThreadFunction, (void*)&i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}