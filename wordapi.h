#ifndef WORDAPI_H
#define WORDAPI_H
#include <ctype.h>
#include <curl/curl.h>
#include <string.h>

int get_word(char *word, int word_len);
int check_word(char *word);
int get_dict();

#endif
