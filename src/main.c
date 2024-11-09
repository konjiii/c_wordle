#include "dictionary.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // download the whole dictionary from the api
    if (get_dict() == 1) {
        return 1;
    }

    size_t word_len = 5;

    if (argc > 1) {
        word_len = atoi(argv[1]);
    }

    if (word_len < 2 || word_len > 15) {
        fprintf(stderr, "A word of length %zu does not exist\n", word_len);
        return 1;
    }

    char word[word_len + 1];
    get_word(word, word_len);

    while (game(word, word_len)) {
        get_word(word, word_len);
    }

    return 0;
}
