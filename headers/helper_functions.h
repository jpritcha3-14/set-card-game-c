#ifndef HELPER_FUNCTIONS_HEADER
#define HELPER_FUNCTIONS_HEADER
#include <ncurses.h>
#include "structs.h"

void load_cards(char **cards, char *filename);
void draw_card(WINDOW *card_window, int card, WINDOW *dummy);

#endif
