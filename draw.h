#include <SDL2/SDL_ttf.h>
#include "game.h"
#define window_height 1000
#define window_width 1000


const int map_offset_x = 100;
const int  map_offset_y = 300;
const int FRAME_SIZE = 37;

void draw_world (SDL_Renderer *renderer , const world &game) ;
void draw_current_ship (SDL_Renderer *renderer , const ship &ship1) ;

void draw_ship (SDL_Renderer *renderer  ,const ship &ship1) ;
bool check_neighbours(int cursor_row , int cursor_column , ship *head) ;
bool check_neighbours_down(ship *head , ship ship1) ;
