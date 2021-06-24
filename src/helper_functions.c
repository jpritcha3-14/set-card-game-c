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
