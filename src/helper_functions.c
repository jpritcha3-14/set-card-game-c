#include <stdlib.h>
#include <ncurses.h>

#include "structs.h"

void swap(int a, int b, int arr[]) {
  int temp = arr[b];
  arr[b] = arr[a];
  arr[a] = temp;
}

int sum(int arr[], int n) {
  int total = 0;
  for (int i=0; i<n; i++) {
    total += arr[i];
  }
  return total;
}

void clear_message(WINDOW* window, int length) {
  for (int i=0; i<length; i++) {
    mvwaddch(window, 0, i, ' ');
  }
  wrefresh(window);
}

void load_cards(char cards[][CARD_H][CARD_W], char* filename) {
  FILE* fp;
  char garbage[10];
  fp = fopen(filename, "r");
  for (int i=0; i<27; i++) {
    for (int j=0; j<CARD_H; j++) {
      fgets(cards[i][j], CARD_W+1, fp);   

      // replace '$' with null terminator
      cards[i][j][CARD_W-1]='\0';
      // move fp past newline
      fgets(garbage, CARD_W, fp);   
    } 
  }
  fclose(fp);
}

int get_color(int card_number) {
  return card_number / 27 + 1;
}

int get_number(int card_number) {
  return (card_number % 3) + 1;
}

int get_shade(int card_number) {
  return (card_number / 9) % 3;
}

int get_shape(int card_number) {
  return (card_number % 9) / 3;
}

void draw_card(WINDOW *card_window, char card[][CARD_W], int color) {
  wattron(card_window, COLOR_PAIR(color));
  //fprintf(stderr, "%d\n", color);
  for (int i=0; i<CARD_H; i++) {
    mvwaddstr(card_window, i+1, 1, card[i]);
  }
  wrefresh(card_window); // flush to screen
}

void draw_border(WINDOW *card_window, char top, char side) {
  for (int i=0; i<CARD_H+2; i++) {
    if (i == 0 || i == CARD_H+1) {
      // Top and bottom
      for (int j=0; j<CARD_W+2; j++) {
        mvwaddch(card_window, i, j, top);
      }
    } else {
      // Left and right
      mvwaddch(card_window, i, 0, side); 
      mvwaddch(card_window, i, CARD_W+1, side); 
    }
  }
  wrefresh(card_window); // flush to screen
}

int move_cursor(WINDOW* card_windows[], int selected[], int inp, int cur_card) {
  int next_card;
  if (inp == 'w' && cur_card > 3) {
    next_card = cur_card - 4;
  } else if (inp == 'a' && cur_card % 4 != 0) {
    next_card = cur_card - 1;
  } else if (inp == 's' && cur_card < 8) {
    next_card = cur_card + 4;
  } else if (inp == 'd' && cur_card % 4 != 3) {
    next_card = cur_card + 1;
  } else {
    return cur_card;
  }
  
  // Do these steps only if the cursor was actually moved
  if (selected[next_card]) {
    wattron(card_windows[next_card], COLOR_PAIR(YELLOW));
  } else {
    wattron(card_windows[next_card], COLOR_PAIR(WHITE));
  }
  draw_border(card_windows[next_card], '@', '@');

  if (selected[cur_card]) {
    wattron(card_windows[cur_card], COLOR_PAIR(YELLOW));
    draw_border(card_windows[cur_card], '-', '|');
  } else {
    wattron(card_windows[cur_card], COLOR_PAIR(WHITE));
    draw_border(card_windows[cur_card], ' ', ' ');
  }
  wrefresh(card_windows[cur_card]);
  wrefresh(card_windows[next_card]);
  return next_card;  
}

void select_card(WINDOW *card_window, int selected[], int cur_card) {
  if (selected[cur_card]) {
    selected[cur_card] = 0; 
    wattron(card_window, COLOR_PAIR(WHITE));
  } else {
    if (sum(selected, 12) < 3) {
      selected[cur_card] = 1;
      wattron(card_window, COLOR_PAIR(YELLOW));
    }
  }
  draw_border(card_window, '@', '@');
  wrefresh(card_window);
}

void shuffle(int cards[], int n) {
  for (int i=0; i<n; i++) {
    int j = rand() % n;  
    int temp = cards[i];
    cards[i] = cards[j];
    cards[j] = temp;
  }
}

int check_set(int a, int b, int c, const int deck[], const card_props props[]) { 
  const card_props *candidate_props[3];
  candidate_props[0] = &props[deck[a]];
  candidate_props[1] = &props[deck[b]];
  candidate_props[2] = &props[deck[c]];
  if (candidate_props[0]->color == candidate_props[1]->color && candidate_props[1]->color != candidate_props[2]->color) return 0;
  if (candidate_props[1]->color == candidate_props[2]->color && candidate_props[2]->color != candidate_props[0]->color) return 0;
  if (candidate_props[2]->color == candidate_props[0]->color && candidate_props[0]->color != candidate_props[1]->color) return 0;

  if (candidate_props[0]->number == candidate_props[1]->number && candidate_props[1]->number != candidate_props[2]->number) return 0;
  if (candidate_props[1]->number == candidate_props[2]->number && candidate_props[2]->number != candidate_props[0]->number) return 0;
  if (candidate_props[2]->number == candidate_props[0]->number && candidate_props[0]->number != candidate_props[1]->number) return 0;

  if (candidate_props[0]->shade == candidate_props[1]->shade && candidate_props[1]->shade != candidate_props[2]->shade) return 0;
  if (candidate_props[1]->shade == candidate_props[2]->shade && candidate_props[2]->shade != candidate_props[0]->shade) return 0;
  if (candidate_props[2]->shade == candidate_props[0]->shade && candidate_props[0]->shade != candidate_props[1]->shade) return 0;

  if (candidate_props[0]->shape == candidate_props[1]->shape && candidate_props[1]->shape != candidate_props[2]->shape) return 0;
  if (candidate_props[1]->shape == candidate_props[2]->shape && candidate_props[2]->shape != candidate_props[0]->shape) return 0;
  if (candidate_props[2]->shape == candidate_props[0]->shape && candidate_props[0]->shape != candidate_props[1]->shape) return 0;

  return 1;
}

void get_selected_cards(int candidates[], const int selected[]) {
  int i = 0;
  for (int j=0; j<12; j++) {
    if (selected[j] > 0) {
      candidates[i++] = j;
      if (i >= 3) return;
    } 
  } 
}
