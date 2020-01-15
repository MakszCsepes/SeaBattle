#include <SDL2/SDL_ttf.h>
#include "game.h"
#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000
#define HORIZONTAL true
#define VERTICAL false

const int SHIP_SIZE = 4;
const int MAP_OFFSET_X = 100;
const int  MAP_OFFSET_Y = 300;
const int FRAME_SIZE = 37;

void draw_world (SDL_Renderer *renderer , const world &game) ;
void draw_current_ship (SDL_Renderer *renderer , const ship &ship1) ;

void draw_ship (SDL_Renderer *renderer  ,const ship &ship1) ;
bool check_neighbours(int cursor_row , int cursor_column , ship *head) ;
bool check_neighbours_down(ship *head , ship ship1) ;
