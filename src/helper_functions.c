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

      fprintf(stderr, cards[i][j]);
      fprintf(stderr, "\n");
    } 
  }
  fclose(fp);
}

void draw_card(WINDOW *card_window, char card[][CARD_W], WINDOW *dummy) {
    attron(COLOR_PAIR(2));
    for (int i=0; i<CARD_H; i++) {
      mvwaddstr(card_window, i, 0, card[i]);
    }
    wrefresh(card_window); // flush to screen
}
