#ifndef LEADERBOARD_HEADER
#define LEADERBOARD_HEADER

#include <ncurses.h>

void show_leaderboard(WINDOW* lbw, WINDOW* dummy);
void add_leaderboard_time(WINDOW* lbw, int time);

#endif
