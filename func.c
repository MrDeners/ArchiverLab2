#define _CRT_SECURE_NO_WARNINGS
#define MAX_WORD_LENGTH 100
#include "func.h"

Word *read_words_from_file(char *filename, int *num_words)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("ERROR!");
        exit(1);
    }
    Word *words = malloc(sizeof(Word));
    char buffer[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", buffer) != EOF)
    {
        int found = 0;
        for (int i = 0; i < *num_words; i++)
        {
            if (strcmp(buffer, words[i].word) == 0)
            {
                words[i].count++;
                found = 1;
                break;
            }
        }
        if (!found)
        {
            words = realloc(words, (*num_words + 1) * sizeof(Word));
            strcpy(words[*num_words].word, buffer);
            words[*num_words].count = 1;
            words[*num_words].was_used = 0;
            (*num_words)++;
        }
    }

    fclose(file);
    return words;
}

void find_and_save_words(Word words[], int num_words, char **most_frequent_long_word_ptr, char **least_frequent_short_word_ptr)
{
    FILE *file = fopen("C:/text_test/library.txt", "a");
    if (file == NULL)
    {
        printf("ERROR!");
        exit(1);
    }

    Word *most_frequent_long_word = NULL;
    Word *least_frequent_short_word = NULL;

    for (int i = 0; i < num_words; i++)
    {
        if (!words[i].was_used)
        {
            if (most_frequent_long_word == NULL || (strlen(words[i].word) > strlen(most_frequent_long_word->word) && words[i].count > most_frequent_long_word->count))
            {
                most_frequent_long_word = &words[i];
            }
            if (least_frequent_short_word == NULL || (strlen(words[i].word) < strlen(least_frequent_short_word->word) && words[i].count < least_frequent_short_word->count))
            {
                least_frequent_short_word = &words[i];
            }
        }
    }

    int already_written = 0;
    char buffer[MAX_WORD_LENGTH];
    rewind(file);
    while (fscanf(file, "%s", buffer) != EOF)
    {
        for (int i = 0; i < num_words; i++)
        {
            if (strcmp(buffer, words[i].word) == 0)
            {
                words[i].was_used = 1;
                break;
            }
        }
        if (strcmp(buffer, most_frequent_long_word->word) == 0 || strcmp(buffer, least_frequent_short_word->word) == 0)
        {
            already_written = 1;
            break;
        }
    }

    if (!already_written && !most_frequent_long_word->was_used && !least_frequent_short_word->was_used)
    {
        fprintf(file, "%s %s\n", most_frequent_long_word->word, least_frequent_short_word->word);
        most_frequent_long_word->was_used = 1;
        least_frequent_short_word->was_used = 1;
        *most_frequent_long_word_ptr = most_frequent_long_word->word;
        *least_frequent_short_word_ptr = least_frequent_short_word->word;
    }

    fclose(file);
}

char *read_line(FILE *file)
{
    size_t buffer_size = 1024;
    char *buffer = malloc(buffer_size);
    size_t len = 0;
    while (fgets(buffer + len, buffer_size - len, file) != NULL)
    {
        len += strlen(buffer + len);
        if (buffer[len - 1] == '\n')
        {
            break;
        }
        buffer_size *= 2;
        buffer = realloc(buffer, buffer_size);
    }
    if (len == 0)
    {
        free(buffer);
        return NULL;
    }
    return buffer;
}
void replace_words_in_file(char *filename, char *word_a, char *word_b)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("ERROR!");
        exit(1);
    }
    FILE *new_file = fopen("compressed.txt", "w");
    if (new_file == NULL)
    {
        printf("ERROR!");
        exit(1);
    }
    char buffer[MAX_WORD_LENGTH];
    char *line = NULL;
    char *next_line = read_line(file);
    while (next_line != NULL)
    {
        line = next_line;
        char *word = strtok(line, " \n");
        while (word != NULL)
        {
            if (strcmp(word, word_a) == 0)
            {
                fprintf(new_file, "%s", word_b);
            }
            else if (strcmp(word, word_b) == 0)
            {
                fprintf(new_file, "%s", word_a);
            }
            else
            {
                fprintf(new_file, "%s", word);
            }
            word = strtok(NULL, " \n");
            if (word != NULL)
            {
                fprintf(new_file, " ");
            }
        }
        next_line = read_line(file);
        if (next_line != NULL)
        {
            fprintf(new_file, "\n");
        }
        free(line);
    }
    fclose(file);
    fclose(new_file);
    int new_size = get_file_size("compressed.txt");
    int old_size = get_file_size(filename);
    if (new_size < old_size)
    {
        remove(filename);
        rename("compressed.txt", filename);
    }
    else
    {
        remove("compressed.txt");
    }
}

int get_file_size(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("ERROR!");
        exit(1);
    }
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    fclose(file);
    return size;
}
void append_config_to_file(char *filename)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("ERROR!");
        exit(1);
    }
    FILE *config_file = fopen("C:/text_test/library.txt", "r");
    if (config_file == NULL)
    {
        printf("ERROR!");
        exit(1);
    }
    fprintf(file, " --- ");
    char buffer[MAX_WORD_LENGTH];
    while (fscanf(config_file, "%s", buffer) != EOF)
    {
        fprintf(file, "%s ", buffer);
    }
    fclose(file);
    fclose(config_file);
}