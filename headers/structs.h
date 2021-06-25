#ifndef STRUCT_HEADER
#define STRUCT_HEADER

#define CARD_W 19 
#define CARD_H 7 
#define RED 1
#define CYAN 2
#define MAGENTA 3
#define WHITE 4
#define GREEN 5
#define BLUE 6
#define YELLOW 7


enum direction{North, South, East, West};

enum speed {slow, normal, fast};

struct loc {
    int row;
    int col;
};

typedef struct loc loc;

struct wloc {
    int y;
    int x;
    int cols;
    int rows;
    int usefulcols;
};

typedef struct wloc wloc;

struct node {
    struct node* next;
    struct node* prev;
    loc* position;
};

struct linked_list {
    struct node* head;
    struct node* tail;  
    int length;
};

#endif
