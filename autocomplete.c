#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct {
    char *completion;
    int score;
} ScoredCompletion;

int compare_scores(const void *a, const void *b) {
    ScoredCompletion *sc1 = (ScoredCompletion *)a;
    ScoredCompletion *sc2 = (ScoredCompletion *)b;
    return sc2->score - sc1->score;
}

int common_prefix_length(const char *str1, const char *str2) {
    int length = 0;
    while (*str1 && *str2 && *str1 == *str2) {
        length++;
        str1++;
        str2++;
    }
    return length;
}

void preprocess_string(const char *src, char *dest) {
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dest++ = tolower((unsigned char)*src);
        }
        src++;
    }
    *dest = '\0';
}

char **get_completions(char **completions, char *input, int input_length, int max_completions_count) { //TODO: Remove hardcoded size
    ScoredCompletion scored_completions[100];
    int count = 0;

    char preprocessed_input[100];
    preprocess_string(input, preprocessed_input);

    for (int i = 0; completions[i] != NULL; i++) {
        char preprocessed_completion[100];
        preprocess_string(completions[i], preprocessed_completion);
        int score = common_prefix_length(preprocessed_completion, preprocessed_input);
        if (score >= 0) {
            scored_completions[count].completion = completions[i];
            scored_completions[count].score = score;
            count++;
        }
    }

    qsort(scored_completions, count, sizeof(ScoredCompletion), compare_scores);

    char **matches = malloc(max_completions_count * sizeof(char *));
    int i;
    for (i = 0; i < count && i < max_completions_count; i++) {
        if (scored_completions[i].score < input_length && input_length != 0) {
            break;
        }
        matches[i] = strdup(scored_completions[i].completion);
    }

    matches[i] = NULL;

    return matches;
}

// int main() {
//     char *completions[] = {
//         "apple",
//         "banana",
//         "cherry",
//         "date",
//         "fig",
//         "grape",
//         "kiwi",
//         "lemon",
//         "mango",
//         "orange",
//         "peach",
//         "pear",
//         "plum",
//         "raspberry",
//         "strawberry",
//         "watermelon",
//         NULL
//     };

//     char *input;
//     char prompt[] = "Enter a fruit: ";
//     int max_completions_count = 5;

//     while ((input = readline(prompt)) != NULL) {
//         int input_length = strlen(input);
//         char **matches = get_completions(completions, input, input_length, max_completions_count);

//         for (int i = 0; matches[i] != NULL; i++) {
//             printf("%s\n", matches[i]);
//         }

//         for (int i = 0; matches[i] != NULL; i++) {
//             free(matches[i]);
//         }
//         free(matches);

//         add_history(input);
//         free(input);
//     }

//     return 0;
// }