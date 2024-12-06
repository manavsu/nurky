#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

static WINDOW *main_win;
volatile sig_atomic_t running = 0;

void get_time(char *buffer, size_t buffer_size)
{
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);
    if (!strftime(buffer, buffer_size, "%I:%M %p", time_info))
    {
        buffer = "Error";
    }
}

void handle_resize() {

}

void stop_and_cleanup() {
    running = 0;
    delwin(main_win);
    endwin();
}

int main()
{
    char time[10];
    int height, width, center_x, center_y;
    WINDOW *main_win, *reboot_button;

    signal(SIGWINCH, handle_resize);
    signal(SIGINT, stop_and_cleanup);
    signal(SIGTERM, stop_and_cleanup);

    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    curs_set(0);

    getmaxyx(stdscr, height, width);
    center_x = width / 2;
    center_y = height / 2;

    main_win = newwin(height, width, 0, 0);

    while (running)
    {
        get_time(time, sizeof(time));
        getmaxyx(stdscr, height, width);
        center_x = width / 2;
        center_y = height / 2;

        werase(main_win);
        box(main_win, 0, 0);
        mvwprintw(main_win, center_y, center_x - strlen(time) / 2, "%s", time);
        wrefresh(main_win);
        napms(200);
    }

    delwin(main_win);
    endwin();
    return 0;
}


