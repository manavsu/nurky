#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

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
}

int main()
{
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

        // Print first to manage cursor position
        mvprintw(start_y + 4, start_x, "--------------------------------");
        mvprintw(start_y + 5, start_x, "Dummy text line 1");
        mvprintw(start_y + 6, start_x, "Dummy text line 2");
        mvprintw(start_y + 7, start_x, "Dummy text line 3");

        WINDOW *completions_win = newwin(5, MAX_COMPLETION_LENGTH + 3, start_y + 3, start_x - 2);
        

        WINDOW *input_win = newwin(3, MAX_INPUT_LENGTH + 3, start_y, start_x - 2);
        box(input_win, 0, 0);
        mvwprintw(input_win, 1, 2, "%s", message);

        refresh();
        wrefresh(input_win);
        handle_input(message, &init);
    }
}
