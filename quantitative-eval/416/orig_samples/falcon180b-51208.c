//Falcon-180B DATASET v1.0 Category: Building a IMAP Client ; Style: surprised
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <curl/curl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    char *buffer = (char *)malloc(realsize + 1);
    if (buffer == NULL) {
        return 0;
    }
    memcpy(buffer, contents, realsize);
    buffer[realsize] = '\0';
    printf("%s", buffer);
    free(buffer);
    return realsize;
}

static int ProgressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    int id = *((int *)clientp);
    printf("Thread %d: Download progress %ld of %ld bytes.\n", id, dlnow, dltotal);
    return 0;
}

void *thread_function(void *arg)
{
    int id = *((int *)arg);
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        char url[100];
        sprintf(url, "imap.gmail.com:993");
        curl_easy_setopt(curl, CURLOPT_URL, url);

        res = curl_easy_perform(curl);
        if (res!= CURLE_OK) {
            printf("Thread %d: curl_easy_perform() failed: %s\n", id, curl_easy_strerror(res));
        } else {
            printf("Thread %d: Download completed successfully.\n", id);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return NULL;
}

int main()
{
    int num_threads = 5;
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *)&i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}