#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

// Define a structure for words with popularity scores
typedef struct {
    const char *word;
    int score; // Higher score = more popular
} Word;

// List of predefined words with popularity scores
Word words[] = {
    {"hello", 5},
    {"help", 2},
    {"hell", 8},
    {"hero", 3},
    {"heron", 1},
    {"hope", 6},
    {NULL, 0} // Marks the end of the list
};

// Function to compare words by score (descending order)
int compare_words(const void *a, const void *b) {
    Word *wordA = (Word *)a;
    Word *wordB = (Word *)b;
    return wordB->score - wordA->score;
}

// Function to generate autocomplete suggestions
char *word_generator(const char *text, int state) {
    static int list_index, len;
    static Word sorted_words[sizeof(words) / sizeof(words[0])];
    const char *word;

    if (!state) {
        // Reset index and copy words for sorting
        list_index = 0;
        len = strlen(text);
        memcpy(sorted_words, words, sizeof(words));
        qsort(sorted_words, sizeof(sorted_words) / sizeof(sorted_words[0]) - 1, sizeof(Word), compare_words);
    }

    // Iterate over sorted words to find matches
    while (sorted_words[list_index].word) {
        word = sorted_words[list_index++].word;
        if (strncmp(word, text, len) == 0) {
            return strdup(word); // Return matching word
        }
    }

    return NULL; // No more matches
}

char **get_completions(const char *text, const char[] words, int num_completions) {
    char **completions = NULL;
    int i, j;

    for (i = 0; words[i]; i++) {
        if (strncmp(text, words[i], strlen(text)) == 0) {
            completions = realloc(completions, (num_completions + 1) * sizeof(char *));
            completions[num_completions++] = strdup(words[i]);
        }
    }

    return completions;
}

// Function to initialize Readline autocomplete
char **autocomplete(const char *text, int start, int end) {
    // Use word_generator to provide matches
    return rl_completion_matches(text, word_generator);
}

int main() {
    // Set up Readline to use our autocomplete function
    rl_attempted_completion_function = autocomplete;

    char *input;

    printf("Type something (autocomplete with popularity scores):\n");
    while ((input = readline("> ")) != NULL) {
        if (*input) {
            add_history(input); // Add non-empty input to history
        }

        printf("You typed: %s\n", input);

        free(input); // Free memory allocated by readline
    }

    return 0;
}
