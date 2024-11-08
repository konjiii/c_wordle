#include "game.h"
#include <ctype.h>
#include <ncurses.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

char *TITLE[42] = {
    "__        _____  ____  ____  _     _____ ",
    "\\ \\      / / _ \\|  _ \\|  _ \\| |   | ____|",
    " \\ \\ /\\ / / | | | |_) | | | | |   |  _|  ",
    "  \\ V  V /| |_| |  _ <| |_| | |___| |___ ",
    "   \\_/\\_/  \\___/|_| \\_\\____/|_____|_____|",
};

int DONE = FALSE;
int CURR_GUESS = 0;
int RESIZE = FALSE;

void init_ncurses(void);
void print_guesses(int maxx, int word_len, char used[6][word_len + 1]);
void handle_resize(int sig);

void game(char *word, int word_len) {
    char used[6][word_len + 1];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < word_len + 1; j++) {
            used[i][j] = '\0';
        }
    }

    // Initialize the screen
    init_ncurses();
    signal(SIGWINCH, handle_resize);

    // add colored text
    start_color();

    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    // for the title
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    while (!DONE) {
        RESIZE = FALSE;
        int maxy, maxx;
        getmaxyx(stdscr, maxy, maxx);
        printw("%i %i", maxy, maxx);

        attron(COLOR_PAIR(3));
        for (int i = 0; i < 5; i++) {
            mvprintw(i, (maxx / 2) - (42 / 2), "%s", TITLE[i]);
        }
        attroff(COLOR_PAIR(3));

        move(5, 0);
        hline('_', maxx);

        while (true) {
            if (RESIZE) {
                break;
            }

            print_guesses(maxx, word_len, used);

            int input = getch();
            if (input == ERR) {
                continue;
            }

            if (input == 10 && strlen(used[CURR_GUESS]) == word_len) {
                CURR_GUESS += 1;
                continue;
            }

            if (input == 263 && strlen(used[CURR_GUESS]) > 0) {
                used[CURR_GUESS][strlen(used[CURR_GUESS]) - 1] = '\0';
                deleteln();
                continue;
            }

            if (!isalpha(input) || strlen(used[CURR_GUESS]) == word_len)
                continue;

            input = toupper(input);

            used[CURR_GUESS][strlen(used[CURR_GUESS])] = input;
        }

        clear();
    }

    // End curses mode
    endwin();

    return;
}

void init_ncurses(void) {
    // initialize ncurses screen
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    return;
}

void print_guesses(int maxx, int word_len, char used[6][word_len + 1]) {
    // print the guesses with colored letters
    for (int i = 0; i <= CURR_GUESS; i++) {
        mvprintw(6 + i, (maxx / 2) - (word_len / 2), "%s", used[i]);
    }
}

void handle_resize(int sig) {
    RESIZE = TRUE;
    // Refresh ncurses' idea of the window size
    endwin();
    init_ncurses();
    refresh();
}
