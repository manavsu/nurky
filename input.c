#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "app_collector.h"
#include "app.h"
#include "autocomplete.h"

#define ESCAPE_KEY 27

#define INPUT_MIN_CHAR 32
#define INPUT_MAX_CHAR 126

#define MAX_INPUT_LENGTH 32
#define MAX_COMPLETION_LENGTH 32

void cleanup_and_exit()
{
    endwin();
    exit(0);
}

void handle_input(char input[], bool *init)
{
    int input_char = getch();
    if (input_char == ERR)
    {
        return;
    }

    if (input_char == ESCAPE_KEY)
    {
        cleanup_and_exit();
    }
    else if (input_char == KEY_BACKSPACE)
    {
        if (*init)
        {
            strcpy(input, "");
            *init = false;
        }
        else
        {
            size_t len = strlen(input);
            if (len > 0)
            {
                input[len - 1] = '\0';
            }
        }
    }
    else if (input_char >= 32 && input_char <= 126)
    {
        if (*init)
        {
            strcpy(input, "");
            *init = false;
        }
        size_t len = strlen(input);
        if (len < MAX_INPUT_LENGTH - 2)
        {
            input[len] = input_char;
            input[len + 1] = '\0';
        }
    }
    else if (input_char = KEY_ENTER) {
        
    }
}

int main()
{
    App **apps = collect_apps();
    char **app_names = get_app_names(apps);

    initscr();

    // Make input non-blocking
    nodelay(stdscr, FALSE);
    noecho();
    keypad(stdscr, TRUE);

    int height, width;

    // Get terminal dimensions
    getmaxyx(stdscr, height, width);

    char message[32] = "drun";
    bool init = true;

    while (true)
    {
        // clear();
        getmaxyx(stdscr, height, width);
        int msg_length = strlen(message);
        int start_x = (width - 32) / 2;
        int start_y = height / 2 - 5;

        WINDOW *completions_win = newwin(10, MAX_COMPLETION_LENGTH + 3, start_y + 3, start_x - 2);
        box(completions_win, 0, 0);
        if (!init)
        {
            char **completions = get_completions(app_names, message, msg_length, 8);

            for (int i = 0; completions[i] != NULL; i++)
            {
                mvwprintw(completions_win, 1+i, 2, "%s", completions[i]);
            }

            for (int i = 0; completions[i] != NULL; i++)
            {
                free(completions[i]);
            }
            free(completions);
        }

        WINDOW *input_win = newwin(3, MAX_INPUT_LENGTH + 3, start_y, start_x - 2);
        box(input_win, 0, 0);
        mvwprintw(input_win, 1, 2, "%s", message);

        refresh();
        wrefresh(completions_win);
        wrefresh(input_win);
        handle_input(message, &init);
    }
}
