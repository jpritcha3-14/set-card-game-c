#ifndef HELPER_FUNCTIONS_HEADER
#define HELPER_FUNCTIONS_HEADER
#include <ncurses.h>
#include "structs.h"

void swap(int a, int b, int arr[]);
int min(int a, int b);
int max(int a, int b);
void clear_message(WINDOW* window, int length);
void load_logo(char logo_letters[][LOGO_LETTER_H][LOGO_LETTER_W], char* filename);
void load_cards(char cards[][CARD_H][CARD_W], char *filename);
void draw_card(WINDOW *card_window, char card[][CARD_W], int color);
void draw_logo(WINDOW *logo_window, char logo[][LOGO_LETTER_H][LOGO_LETTER_W]);
void draw_blank_card(WINDOW *card_window);
void draw_border(WINDOW *card_window, char top, char side);
int move_cursor(WINDOW* card_windows[], int selected[], int inp, int cur_card);
void select_card(WINDOW *card_window, int slected[], int cur_card);
void clear_screen();

int sum(int arr[], int n);
void shuffle(int cards[], int n);
int get_color(int card_number);
int get_number(int card_number);
int get_shade(int card_number);
int get_shape(int card_number);
int check_set(int a, int b, int c, const int deck[], const card_props props[]);
void get_selected_cards(int candidates[], const int selected[]);
int get_set_count(int num_cards, int deck[], card_props props[]);
int any_set(int num_cards, int deck[], card_props props[]);
int in_set(int num, int set[]);

#endif
