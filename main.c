#include <curl/curl.h>
#include <curl/easy.h>
#include <curses.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *TITLE[42] = {
    "__        _____  ____  ____  _     _____ ",
    "\\ \\      / / _ \\|  _ \\|  _ \\| |   | ____|",
    " \\ \\ /\\ / / | | | |_) | | | | |   |  _|  ",
    "  \\ V  V /| |_| |  _ <| |_| | |___| |___ ",
    "   \\_/\\_/  \\___/|_| \\_\\____/|_____|_____|",
};

int get_word(char *word, int word_len);
size_t write_callback(void *ptr, size_t size, size_t nmemb, char *data);
void game(char *word, int word_len);

int main(int argc, char *argv[]) {
    int word_len = 5;

    if (argc > 1) {
        word_len = atoi(argv[1]);
    }
    if (word_len < 2 || word_len > 15) {
        fprintf(stderr, "A word of length %i does not exist\n", word_len);
        return 1;
    }
    char word[word_len + 1];
    if (get_word(word, word_len) == 1) {
        return 1;
    }

    game(word, word_len);

    return 0;
}

int get_word(char *word, int word_len) {
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

    return 0;
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, char *data) {
    size_t total_size = size * nmemb;
    strncat(data, ptr, total_size);
    return total_size;
}

void game(char *word, int word_len) {
    bool DONE = false;

    char used[6][5];

    // Initialize the screen
    initscr();

    // add colored text
    start_color();

    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);

    while (!DONE) {
        int maxy, maxx;
        getmaxyx(stdscr, maxy, maxx);

        move(0, maxx / 2);

        printw("%i %i\n", maxy, maxx);

        getch();

        clear();
    }

    // End curses mode
    endwin();

    return;
}
