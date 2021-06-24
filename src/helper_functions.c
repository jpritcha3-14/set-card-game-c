#include <stdlib.h>
#include <ncurses.h>

#include "structs.h"

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

      //fprintf(stderr, cards[i][j]);
      //fprintf(stderr, "\n");
    } 
  }
  fclose(fp);
}

void draw_card(WINDOW *card_window, char card[][CARD_W], WINDOW *dummy) {
  wattron(card_window, COLOR_PAIR(2));
  for (int i=0; i<CARD_H; i++) {
    mvwaddstr(card_window, i+1, 1, card[i]);
  }
  wrefresh(card_window); // flush to screen
}

void draw_border(WINDOW *card_window, char ch) {
  for (int i=0; i<CARD_H+2; i++) {
    if (i == 0 || i == CARD_H+1) {
      // Top and bottom
      for (int j=0; j<CARD_W+2; j++) {
        mvwaddch(card_window, i, j, ch);
      }
    } else {
      // Left and right
      mvwaddch(card_window, i, 0, ch); 
      mvwaddch(card_window, i, CARD_W+1, ch); 
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
    wattron(card_windows[next_card], COLOR_PAIR(2));
  } else {
    wattron(card_windows[next_card], COLOR_PAIR(1));
  }
  draw_border(card_windows[next_card], '!');

  if (selected[cur_card]) {
    wattron(card_windows[cur_card], COLOR_PAIR(2));
    draw_border(card_windows[cur_card], '*');
  } else {
    wattron(card_windows[cur_card], COLOR_PAIR(1));
    draw_border(card_windows[cur_card], ' ');
  }
  wrefresh(card_windows[cur_card]);
  wrefresh(card_windows[next_card]);
  return next_card;  
}

void select_card(WINDOW *card_window, int selected[], int cur_card) {
  if (selected[cur_card]) {
    selected[cur_card] = 0; 
    wattron(card_window, COLOR_PAIR(1));
  } else {
    selected[cur_card] = 1; 
    wattron(card_window, COLOR_PAIR(2));
  }
  draw_border(card_window, '!');
  wrefresh(card_window);
}
