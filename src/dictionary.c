#include "dictionary.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **DICT = NULL;

size_t write_data(char *ptr, size_t size, size_t nmemb, FILE *output);

int get_dict() {
    CURL *curl;
    CURLcode res;
    FILE *output = fopen("dict", "w");

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    // get the dictionary data from the api
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

    fclose(output);

    return 0;
}

size_t write_data(char *ptr, size_t size, size_t nmemb, FILE *output) {
    size_t total_size = size * nmemb;

    for (unsigned int i = 2; i < nmemb; i++) {
        if (ptr[i] == '"') {
            fprintf(output, "\n");
            i += 2;
            continue;
        }
        fprintf(output, "%c", ptr[i]);
    }

    return total_size;
}

void get_word(char *word, size_t word_len) {
    FILE *dict = fopen("dict", "r");
    FILE *tmp = fopen("tmp", "w+");

    char dict_word[17];

    int counter = 0;
    while (fgets(dict_word, 17, dict) != NULL) {
        if (strlen(dict_word) == word_len + 1) {
            fprintf(tmp, "%s", dict_word);
            counter++;
        }
    }

    fclose(dict);

    srand(time(NULL));
    int rand_idx = rand() % counter;

    rewind(tmp);

    while (fscanf(tmp, "%s\n", dict_word) != -1 && rand_idx != 0) {
        rand_idx--;
    }

    fclose(tmp);
    remove("tmp");

    strcpy(word, dict_word);
}

int check_word(char *word) {
    FILE *dict = fopen("dict", "r");

    char dict_word[17];

    while (fscanf(dict, "%s\n", dict_word) != -1) {
        if (strcmp(word, dict_word) == 0) {
            return 1;
        }
    }

    fclose(dict);

    return 0;
}
