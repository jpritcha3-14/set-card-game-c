#ifndef HELPER_FUNCTIONS_HEADER
#define HELPER_FUNCTIONS_HEADER
#include <ncurses.h>
#include "structs.h"

void draw_card(wloc* ga, WINDOW* dummy);
wloc* get_game_area(WINDOW* w);

#endif
