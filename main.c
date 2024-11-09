#include "game.h"
#include "wordapi.h"
#include <stdio.h>
#include <stdlib.h>

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

    while (game(word, word_len)) {
        get_word(word, word_len);
    }

    return 0;
}
