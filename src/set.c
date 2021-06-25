#include <stdio.h> 

#include "helper_functions.h"
#include "structs.h"

int play_game(WINDOW *card_windows[], char cards[][CARD_H][CARD_W], WINDOW *dummy) {
  int selected[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int deck[81];
  int cur_card = 0;
  int prev_card = 0;
  char inp;

  for (int i=0; i<81; i++) {
    deck[i] = i; 
  }
  shuffle(deck, 81);

  for (int i=0; i<12; i++) {  
    draw_card(card_windows[i], cards[deck[i] % 27], deck[i]);
  }

  // draw cursor start
  wattron(card_windows[cur_card], COLOR_PAIR(WHITE));
  draw_border(card_windows[cur_card], '@', '@');
  wrefresh(card_windows[cur_card]);

  while (1) {
    inp = wgetch(dummy);
    if (inp == 'q') break;
    if (inp == ' ') {
      select_card(card_windows[cur_card], selected, cur_card);
    } else {
      prev_card = cur_card;
      cur_card = move_cursor(card_windows, selected, inp, prev_card);
    }
  }
  return 0;
}
