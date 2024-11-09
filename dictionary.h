#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <ctype.h>
#include <curl/curl.h>
#include <string.h>

int get_dict();
void get_word(char *word, int word_len);
int check_word(char *word);

#endif
