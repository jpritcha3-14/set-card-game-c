#include <stdio.h> 
#include "helper_functions.h"

void draw_card(wloc* ga, WINDOW* dummy) {
    if (ga != NULL) {
      attron(COLOR_PAIR(2));
      FILE* fp;
      char buff[255];
      fp = fopen("assets/cards.txt", "r");
      int i = ga->y;
      while (i < (ga->y) + 7) {
        fgets(buff, 255, fp);
        buff[19]='\0';
        fprintf(stderr, "%d %d\n", i, ga->x);
        fprintf(stderr, buff);
        mvwaddstr(stdscr, i++, ga->x, buff);
      }
      fclose(fp);
      refresh(); // flush to screen
      wgetch(dummy); // wait for key press
    }
}
