#ifndef HELPER_FUNCTIONS_HEADER
#define HELPER_FUNCTIONS_HEADER
#include <ncurses.h>
#include "structs.h"

void load_cards(char cards[][CARD_H][CARD_W], char *filename);
void draw_card(WINDOW *card_window, char card[][CARD_W], int card_number);
void draw_border(WINDOW *card_window, char top, char side);
int move_cursor(WINDOW* card_windows[], int selected[], int inp, int cur_card);
void select_card(WINDOW *card_window, int slected[], int cur_card);

void shuffle(int cards[], int n);
int get_color(int card_number);
int get_number(int card_number);
int get_shade(int card_number);
int get_shape(int card_number);

#endif
