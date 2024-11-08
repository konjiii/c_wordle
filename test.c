#include <curses.h> // In most systems, this is the same as #include <ncurses.h>

// Alternatively:
// #include <ncurses.h>  // Ensures ncurses features

int main() {
    initscr();
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        printw("This is red text on a black background.");
        attroff(COLOR_PAIR(1));
    } else {
        printw("Your terminal does not support colors.");
    }
    refresh();
    getch();
    endwin();
    return 0;
}
