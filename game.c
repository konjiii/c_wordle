#include "game.h"
#include <ctype.h>
#include <ncurses.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

char *LOGO[42] = {
    "__        _____  ____  ____  _     _____ ",
    "\\ \\      / / _ \\|  _ \\|  _ \\| |   | ____|",
    " \\ \\ /\\ / / | | | |_) | | | | |   |  _|  ",
    "  \\ V  V /| |_| |  _ <| |_| | |___| |___ ",
    "   \\_/\\_/  \\___/|_| \\_\\____/|_____|_____|",
};

int DONE = FALSE;
int CURR_GUESS = 0;
int RESIZE = FALSE;
char *RESTART_Q = "Restart or Quit?(r/q)";

void init_ncurses(void);
void init_used(int word_len, char used[6][word_len + 1],
               int used_colors[6][word_len]);
void handle_resize(int sig);
void print_guesses(int maxx, int word_len, char used[6][word_len + 1],
                   int used_colors[6][word_len]);
int check_guess(char *word, int word_len, char used[6][word_len + 1],
                int used_colors[6][word_len]);
int count(char *word, char chr, int beg, int end);

int game(char *word, int word_len) {
    DONE = FALSE;
    char used[6][word_len + 1];
    int used_colors[6][word_len];
    init_used(word_len, used, used_colors);

    // Initialize the screen
    init_ncurses();
    signal(SIGWINCH, handle_resize);

    // add colored text
    start_color();

    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    // for the title
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    // game loop
    while (TRUE) {
        CURR_GUESS = 0;
        RESIZE = FALSE;
        int maxy, maxx;
        getmaxyx(stdscr, maxy, maxx);

        // print logo
        attron(COLOR_PAIR(3));
        for (int i = 0; i < 5; i++) {
            mvprintw(i, (maxx / 2) - (42 / 2), "%s", LOGO[i]);
        }
        attroff(COLOR_PAIR(3));

        move(5, 0);
        hline('_', maxx);

        // input loop
        while (true) {
            if (RESIZE) {
                endwin();
                init_ncurses();
                clear();
                refresh();
                break;
            }

            // print the guesses with color
            print_guesses(maxx, word_len, used, used_colors);

            // handle ending
            if (CURR_GUESS > 5 || DONE) {
                if (DONE)
                    mvprintw(12, (maxx / 2) - (strlen(RESTART_Q) / 2),
                             "You won!");
                else
                    mvprintw(12, (maxx / 2) - (strlen(RESTART_Q) / 2),
                             "The word was %s", word);
                mvprintw(13, (maxx / 2) - (strlen(RESTART_Q) / 2), "%s",
                         RESTART_Q);

                int answ = getch();

                if (!isalpha(answ))
                    continue;

                answ = toupper(answ);

                if (answ == 'Q') {
                    endwin();
                    return 0;
                } else if (answ == 'R') {
                    return 1;
                }
            }

            // handle input
            int input = getch();
            if (input == ERR) {
                continue;
            } else if (input == 10 && strlen(used[CURR_GUESS]) == word_len) {
                DONE = check_guess(word, word_len, used, used_colors);
                CURR_GUESS += 1;
                continue;
            } else if (input == 263 && strlen(used[CURR_GUESS]) > 0) {
                used[CURR_GUESS][strlen(used[CURR_GUESS]) - 1] = '\0';
                deleteln();
                continue;
            } else if (!isalpha(input) ||
                       strlen(used[CURR_GUESS]) == word_len) {
                continue;
            }

            input = toupper(input);

            used[CURR_GUESS][strlen(used[CURR_GUESS])] = input;
        }
    }
}

void init_ncurses(void) {
    // initialize ncurses screen
    clear();
    refresh();
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

void init_used(int word_len, char used[6][word_len + 1],
               int used_colors[6][word_len]) {
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < word_len + 1; j++)
            used[i][j] = '\0';

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < word_len; j++)
            used_colors[i][j] = 0;
}

void handle_resize(int sig) { RESIZE = TRUE; }

void print_guesses(int maxx, int word_len, char used[6][word_len + 1],
                   int used_colors[6][word_len]) {
    // print the guesses with colored letters
    for (int i = 0; i <= CURR_GUESS; i++) {
        for (int j = 0; j < word_len; j++) {
            move(6 + i, (maxx / 2) - (word_len / 2) + j);
            if (used[i][j] == '\0')
                break;
            attron(COLOR_PAIR(used_colors[i][j]));
            addch(used[i][j]);
            attroff(COLOR_PAIR(used_colors[i][j]));
        }
    }
}

int check_guess(char *word, int word_len, char used[6][word_len + 1],
                int used_colors[6][word_len]) {
    char gotten[word_len + 1];
    for (int i = 0; i < word_len + 1; i++)
        gotten[i] = '\0';

    for (int i = 0; i < word_len; i++) {
        if (word[i] == used[CURR_GUESS][i]) {
            used_colors[CURR_GUESS][i] = 2;
            gotten[i] = used[CURR_GUESS][i];
        }
    }

    if (strlen(gotten) == word_len)
        return 1;

    for (int i = 0; i < word_len; i++) {
        if (count(used[CURR_GUESS], used[CURR_GUESS][i], 0, i) +
                count(gotten, used[CURR_GUESS][i], 0, word_len) <
            count(word, used[CURR_GUESS][i], 0, word_len)) {
            used_colors[CURR_GUESS][i] = 1;
        }
    }

    return 0;
}

int count(char *word, char chr, int beg, int end) {
    int counter = 0;
    for (int i = beg; i < end; i++) {
        if (word[i] == chr) {
            counter++;
        }
    }
    return counter;
}
