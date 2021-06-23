#include <stdlib.h>
#include <ncurses.h>

#include "structs.h"

void load_cards(char *cards[], char* filename) {
  return;
}

void draw_card(WINDOW *card_window, int card, WINDOW *dummy) {
    attron(COLOR_PAIR(2));
    FILE* fp;
    char buff[255];
    fp = fopen("assets/cards.txt", "r");
    int i = 0;
    while (i < CARD_H) {
      fgets(buff, 255, fp);
      buff[CARD_W]='\0';
      fprintf(stderr, buff);
      fprintf(stderr, "\n");
      mvwaddstr(card_window, i++, 0, buff);
    }
    fclose(fp);
    wrefresh(card_window); // flush to screen
}
