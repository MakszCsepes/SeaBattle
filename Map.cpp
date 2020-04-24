//
// Created by max on 22/1/2020.
//

#include "Map.h"

void put_cell (SDL_Renderer* renderer, int& x, int& y ) {
    SDL_Rect r;

    r.x = x;
    r.y = y;
    r.w = CELL_SIZE - 2;
    r.h = CELL_SIZE - 2;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &r);
}

void CMap::create_map()  {
    cells_array = new int*[MAP_CELL_HEIGHT];
    for (int i = 0; i < MAP_CELL_HEIGHT; i++) {
        this->cells_array[i] = new int[MAP_CELL_WIDTH];

        for (int j = 0; j < MAP_CELL_WIDTH; j++) {
            cells_array[i][j] = EMPTY_CELL;
        }
    }
}

void CMap::delete_map() {
    for(int i = 0 ; i < MAP_CELL_HEIGHT ; i++) {
        delete[] cells_array[i] ;
    }
}

void CMap::draw_mishit (SDL_Renderer *renderer, int& x, int& y) {
    SDL_SetRenderDrawColor(renderer ,  93 , 102 , 111 , 255);;

    SDL_Rect low = {MAP_OFFSET_X + x * CELL_SIZE + offset_x + 2, MAP_OFFSET_Y + y * CELL_SIZE + offset_y + 18 , 3 , 3};
    SDL_Rect low_1 = {MAP_OFFSET_X + x * CELL_SIZE + offset_x + 2, MAP_OFFSET_Y + y * CELL_SIZE + offset_y + 10 , 3 , 3};;
    SDL_Rect mid = {MAP_OFFSET_X + x * CELL_SIZE + offset_x + 2, MAP_OFFSET_Y + y * CELL_SIZE + offset_y + 2 , 3 , 3};
    SDL_Rect up_1 = {MAP_OFFSET_X + x * CELL_SIZE + offset_x + 10, MAP_OFFSET_Y + y * CELL_SIZE + offset_y + 2 , 3 , 3};
    SDL_Rect up = {MAP_OFFSET_X + x * CELL_SIZE + offset_x + 18, MAP_OFFSET_Y + y * CELL_SIZE + offset_y + 2 , 3 , 3};;

    for (int i = 0 ; i < 10 ; i++ , mid.x += mid.h , mid.y += mid.w ) {
        SDL_RenderFillRect(renderer , &mid);
    }

    for (int i = 0 ; i < 8 ; i++ , up_1.x += up_1.h , up_1.y += up_1.w) {
        SDL_RenderFillRect(renderer , &up_1);
    }

    for (int i = 0 ; i < 5 ; i++ , up.x += up.h , up.y += up.w) {
        SDL_RenderFillRect(renderer , &up);
    }

    for (int i = 0 ; i < 8 ; i++ , low_1.x += low_1.h , low_1.y += low_1.w) {
        SDL_RenderFillRect(renderer , &low_1);
    }

    for (int i = 0 ; i < 5 ; i++ , low.x += low.h , low.y += low.w) {
        SDL_RenderFillRect(renderer , &low);
    }
}

void CMap::draw(SDL_Renderer* renderer) {

    for (int y = MAP_OFFSET_Y + offset_y, i = 0 ; i < MAP_CELL_WIDTH ; i++, y += CELL_SIZE) {
        for (int x = MAP_OFFSET_X + offset_x, j = 0 ; j < MAP_CELL_HEIGHT ; j++, x += CELL_SIZE) {

            put_cell(renderer, x, y);
            if (cells_array[i][j] == MISHIT_CELL) {
                draw_mishit(renderer, j, i);
            }
        }
    }

    if ( !cursor.get_hidden()) {
        cursor.draw(renderer);
    }
}
