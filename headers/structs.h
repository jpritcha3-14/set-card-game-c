#ifndef STRUCT_HEADER
#define STRUCT_HEADER

#define CARD_W 19 
#define CARD_H 7 
#define MESSAGE_W 2*(CARD_W+2) 
#define SET_COUNT_W CARD_W+2 
#define RED 1
#define CYAN 2
#define MAGENTA 3
#define WHITE 4
#define GREEN 5
#define BLUE 6
#define YELLOW 7

typedef struct card_props {
  int color;
  int number;
  int shade;
  int shape;
} card_props;

#endif
