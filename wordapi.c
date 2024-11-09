#include "wordapi.h"
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

size_t write_data(char *ptr, size_t size, size_t nmemb, FILE *output);
size_t write_callback(void *ptr, size_t size, size_t nmemb, char *data);

int get_dict() {
    CURL *curl;
    CURLcode res;
    FILE *output = fopen("dict.txt", "w");

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        char url[53];
        sprintf(url, "https://random-word-api.herokuapp.com/all");
        // POST URL
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        // write data from api to response_data variable
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "failed to fetch a word\n");
            return 1;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}

size_t write_data(char *ptr, size_t size, size_t nmemb, FILE *output) {
    size_t total_size = size * nmemb;

    for (int i = 2; i < nmemb; i++) {
        if (ptr[i] == '"') {
            fprintf(output, "\n");
            i += 2;
            continue;
        }
        fprintf(output, "%c", ptr[i]);
    }

    return total_size;
}

int get_word(char *word, int word_len) {
    while (1) {
        CURL *curl;
        CURLcode res;
        char response_data[4096] = "";

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            char url[53];
            sprintf(url, "https://random-word-api.herokuapp.com/word?length=%i",
                    word_len);
            // POST URL
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            // write data from api to response_data variable
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_data);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "failed to fetch a word\n");
                return 1;
            }

            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();

        strncpy(word, response_data + 2, word_len);
        word[word_len] = '\0';

        // check if word exists in the check_word api
        if (check_word(word)) {
            break;
        }
    }

    return 0;
}

int check_word(char *word) {
    CURL *curl;
    CURLcode res;
    char response_data[4096] = "";

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        char url[57];
        sprintf(url, "https://api.datamuse.com/words?sp=%s&max=1", word);
        // POST URL
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        // write data from api to response_data variable
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "failed to check word\n");
            return 1;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    if (strlen(response_data) == 2) {
        return 0;
    }

    char response_word[strlen(word) + 1];
    response_word[strlen(word)] = '\0';
    for (int i = 0; i < strlen(response_data); i++) {
        if (response_data[i] == ':') {
            strncpy(response_word, response_data + i + 2, strlen(word));
            break;
        }
    }

    if (strcmp(word, response_word) != 0)
        return 0;

    return 1;
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, char *data) {
    size_t total_size = size * nmemb;
    strncat(data, ptr, total_size);
    return total_size;
}
