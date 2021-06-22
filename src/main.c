#include <ncurses.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "helper_functions.h" 
#include "structs.h"

int main() {
    initscr();
    noecho();
    start_color();
    // Dummy window to for input, avoids extra refresh on wgetch call
    WINDOW* dummy = newwin(1, 1, 1000, 1000); 
    keypad(dummy, true);
    curs_set(0);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    wloc *ga = get_game_area(stdscr);

    draw_card(ga, dummy);

    free(ga);
    endwin();
    return EXIT_SUCCESS;
}
