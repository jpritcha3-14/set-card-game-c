#ifndef SET_HEADER
#define SET_HEADER
#include <ncurses.h>
#include "structs.h"

int play_game(WINDOW *card_windows[], char cards[][CARD_H][CARD_W], WINDOW *dummy);

#endif
