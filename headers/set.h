#ifndef SET_HEADER
#define SET_HEADER
#include <ncurses.h>
#include "structs.h"

int play_game(WINDOW *card_windows[], WINDOW *messages, WINDOW *card_count, WINDOW *set_count, WINDOW *timer_window, char cards[][CARD_H][CARD_W], WINDOW *dummy);

#endif
