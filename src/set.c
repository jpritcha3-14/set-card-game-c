#include <stdio.h> 

#include "helper_functions.h"
#include "structs.h"

int play_game(WINDOW *card_windows[], char cards[][CARD_H][CARD_W], WINDOW *dummy) {
  int cur_card = 0;
  int prev_card = 0;
  char inp;
  while (1) {
    wattron(card_windows[cur_card], COLOR_PAIR(3));
    draw_border(card_windows[prev_card], ' ');
    draw_border(card_windows[cur_card], '!');
    wrefresh(card_windows[cur_card]);
    inp = wgetch(dummy);
    prev_card = cur_card;
    if (inp == 'w' && cur_card > 3) {
      cur_card -= 4;
    } else if (inp == 'a' && cur_card % 4 != 0) {
      cur_card -= 1;
    } else if (inp == 's' && cur_card < 8) {
      cur_card += 4;
    } else if (inp == 'd' && cur_card % 4 != 3) {
      cur_card += 1;
    } else if (inp == 'q') {
      break;  
    }
  }
  return 0;
}
