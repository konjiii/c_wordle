#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <curl/curl.h>

int get_dict();
int get_word(char *word, size_t word_len);
int check_word(char *word);

#endif
