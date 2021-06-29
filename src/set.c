#include <stdio.h> 

#include "helper_functions.h"
#include "structs.h"

int play_game(WINDOW *card_windows[], WINDOW* messages, WINDOW* set_count, char cards[][CARD_H][CARD_W], WINDOW *dummy) {
  int selected[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int max_card = 80; 
  int deck[81];
  card_props props[81];
  int cur_card = 0;
  int prev_card = 0;
  char inp;

  // Initialize deck and props
  for (int i=0; i<81; i++) {
    deck[i] = i; 
    props[i].color = get_color(i); 
    props[i].number = get_number(i); 
    props[i].shade = get_shade(i); 
    props[i].shape = get_shape(i); 
    //fprintf(stderr, "%d %d %d %d\n", props[i].color, props[i].number, props[i].shade, props[i].shape);
  }

  shuffle(deck, 81);

  // Draw first 12 cards
  for (int i=0; i<12; i++) {  
    draw_card(card_windows[i], cards[deck[i] % 27], props[deck[i]].color);
  }

  // draw cursor start
  wattron(card_windows[cur_card], COLOR_PAIR(WHITE));
  draw_border(card_windows[cur_card], '@', '@');
  wrefresh(card_windows[cur_card]);

  mvwaddstr(messages, 0, 0, "TEST TEST TEST");
  mvwaddstr(set_count, 0, 0, "TEST TEST TEST");
  wrefresh(messages);
  wrefresh(set_count);


  for (;;) {
    inp = wgetch(dummy);
    if (inp == 'q') break;
    if (inp == ' ') {
      select_card(card_windows[cur_card], selected, cur_card);
      if (sum(selected, 12) == 3) {
        int candidates[3];
        get_selected_cards(candidates, selected);
        if (check_set(candidates[0], candidates[1], candidates[2], deck, props) > 0)  {
          for (int i=0; i<3; i++) {
            int card_loc = candidates[i];
            swap(max_card - i, card_loc, deck); 
            int card_num = deck[card_loc];
            draw_card(card_windows[card_loc], cards[card_num % 27], props[card_num].color);
            selected[card_loc] = 0;
            draw_border(card_windows[card_loc], ' ', ' ');
          }
          wattron(card_windows[cur_card], COLOR_PAIR(WHITE));
          draw_border(card_windows[cur_card], '@', '@');
          max_card -= 3;
          //fprintf(stderr, "THATS A SET!\n");
        } else {
          fprintf(stderr, "NOT A SET!\n");
        }
      }
    } else {
      prev_card = cur_card;
      cur_card = move_cursor(card_windows, selected, inp, prev_card);
    }
  }
  return 0;
}
