#ifndef INC_5_1_FUNCTIONS_H
#define INC_5_1_FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
    int was_used;
} Word;

Word* read_words_from_file(char* filename, int* num_words);
void find_and_save_words(Word words[], int num_words, char** most_frequent_long_word_ptr, char** least_frequent_short_word_ptr);
char* read_line(FILE* file);
void replace_words_in_file(char* filename, char* word_a, char* word_b);
int get_file_size(char* filename);
void append_config_to_file(char* filename);
#endif //INC_5_1_FUNCTIONS_H