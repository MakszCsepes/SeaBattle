#include "ship.h"

void CShip::change_inverse() {
    inverse = !inverse;
}

int CShip::get_X() {
    return damage_level;
}
int CShip::get_size() {
    return size;
}
bool CShip::get_inverse() {
    return inverse;
}
bool CShip::get_hidden() {
    return hidden;
}

void CShip::draw_ship_head(SDL_Renderer* renderer) {
    SDL_Rect ship_head;

    ship_head.x = MAP_OFFSET_X + head_coordinate_x * CELL_SIZE - 1 + offset_x + 12;
    ship_head.y = MAP_OFFSET_Y + head_coordinate_y * CELL_SIZE - 1 + offset_y + 12;
    ship_head.w = 12;
    ship_head.h = 12;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer , &ship_head);

}
void CShip::draw_ship_part(SDL_Renderer* renderer, int i) {
    SDL_Rect ship_part;
    ship_part.w = CELL_SIZE;
    ship_part.h = CELL_SIZE;
    ship_part.x = -1 + MAP_OFFSET_X + head_coordinate_x * CELL_SIZE + offset_x;
    ship_part.y = -1 + MAP_OFFSET_Y + head_coordinate_y * CELL_SIZE + offset_y;

    if (is_selected) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    if (inverse) {
        ship_part.x += 1 + i * CELL_SIZE;
    } else {
        ship_part.y += 1 + i * CELL_SIZE;
    }

    SDL_RenderFillRect(renderer, &ship_part);

    if (i == 0) {
        draw_ship_head(renderer);
    }

}
void CShip::draw_hit (SDL_Renderer* renderer, int& sec_number) {
    SDL_Rect hit_left ;

    if (inverse == HORIZONTAL) {
        hit_left.x = (head_coordinate_x + sec_number) * CELL_SIZE + MAP_OFFSET_X + offset_x + 2;
        hit_left.y = head_coordinate_y * CELL_SIZE + MAP_OFFSET_Y + offset_y + 2;
    } else {
        hit_left.x = head_coordinate_x * CELL_SIZE + MAP_OFFSET_X + offset_x + 2;
        hit_left.y = (head_coordinate_y + sec_number) * CELL_SIZE + MAP_OFFSET_Y + offset_y + 2;
    }
    hit_left.h = 3;
    hit_left.w = 3;

    SDL_Rect hit_right;

    if(inverse == HORIZONTAL) {
        hit_right.x = (head_coordinate_x + sec_number) * CELL_SIZE + MAP_OFFSET_X + offset_x + CELL_SIZE - 8;
        hit_right.y = head_coordinate_y * CELL_SIZE + MAP_OFFSET_Y + offset_y + 2;
    } else {
        hit_right.x = head_coordinate_x * CELL_SIZE + MAP_OFFSET_X + offset_x + CELL_SIZE - 8;
        hit_right.y = (head_coordinate_y + sec_number) * CELL_SIZE + MAP_OFFSET_Y + offset_y + 2;
    }
    hit_right.h = 3;
    hit_right.w = 3;

    SDL_SetRenderDrawColor(renderer , 255 , 0 , 0 , 255);

    for (int i = 0 ; i < 10 ; i++ , hit_left.x += hit_left.h , hit_left.y += hit_left.w ) {

        SDL_RenderFillRect(renderer , &hit_left);
        SDL_RenderFillRect(renderer , &hit_right);

        hit_right.x -= hit_right.h;
        hit_right.y += hit_right.w;
    }
}

// draw ships
void draw_submarine(isoEngineT* isoEngine, int i, int j, int offset_x, int offset_y, bool& inv) {
    point2DT point;
    // todo fix +20 and + 11
    point.x = (i * TILESIZE) + isoEngine->scrollX + MAP_OFFSET_X + offset_x + 20;
    point.y = (j * TILESIZE) + isoEngine->scrollY + MAP_OFFSET_Y + offset_y + 10;

    Converter2DToIso(&point);

    texture_renderer_XY_clip(&submarineTex, point.x, point.y, inv ? &submarine_rect[0] : &submarine_rect[1]);
}
void draw_destroyer(isoEngineT* isoEngine, int i, int j, int offset_x, int offset_y, bool& inv) {
    point2DT point;
    point.x = (i * TILESIZE) + isoEngine->scrollX + MAP_OFFSET_X + offset_x + 20;
    point.y = (j * TILESIZE) + isoEngine->scrollY + MAP_OFFSET_Y + offset_y + 10;

    Converter2DToIso(&point);

    texture_renderer_XY_clip(&destroyerTex, inv ? point.x : point.x - 30, point.y, inv ? &destroyer_rect[0] : &destroyer_rect[1]);
}
void draw_cruiser(isoEngineT* isoEngine, int i, int j, int offset_x, int offset_y, bool& inv) {
    point2DT point;

    point.x = (i * TILESIZE) + isoEngine->scrollX + MAP_OFFSET_X + offset_x;
    point.y = (j * TILESIZE) + isoEngine->scrollY + MAP_OFFSET_Y + offset_y;

    Converter2DToIso(&point);
    // todo why -65
    texture_renderer_XY_clip(&cruiserTex, inv ? point.x : point.x - 65, point.y, inv ? &cruiser_rect[0] : &cruiser_rect[1]);
}
void draw_battleship(isoEngineT* isoEngine, int i, int j, int offset_x, int offset_y, bool& inv) {
    point2DT point;

    point.x = (i * TILESIZE) + isoEngine->scrollX + MAP_OFFSET_X + offset_x;
    point.y = (j * TILESIZE) + isoEngine->scrollY + MAP_OFFSET_Y + offset_y;

    Converter2DToIso(&point);

    // todo why -MAP_OFFSET_X
    // check inverse : if it`s HORIZONTAL, then we render horizontal version of the ship
    texture_renderer_XY_clip(&battleshipTex, inv ? point.x : point.x - MAP_OFFSET_X, point.y, inv ? &battleship_rect[0] : &battleship_rect[1]);
}


void CShip::draw(isoEngineT* isoEngine) {
    if (hidden && (damage_level == 0)) {
        return;
    }

    switch (size) {
        case SUBMARINE_SIZE:
            draw_submarine(isoEngine, head_coordinate_x, head_coordinate_y, offset_x, offset_y, inverse);
            break;
        case CRUISER_SIZE:
            draw_cruiser(isoEngine, head_coordinate_x, head_coordinate_y, offset_x, offset_y, inverse);
            break;
        case DESTROYER_SIZE:
            draw_destroyer(isoEngine, head_coordinate_x, head_coordinate_y, offset_x, offset_y, inverse);
            break;
        case BATTLESHIP_SIZE:
            draw_battleship(isoEngine, head_coordinate_x, head_coordinate_y, offset_x, offset_y, inverse);
            break;
    }

    /*for(int i = 0 ; i < size ; i++) {
        if (!hidden || (damage_level & section_array[i])) {
            draw_ship_part(get_renderer(), i);

            if (damage_level & section_array[i]) {
                draw_hit(get_renderer(), i);
            }
        }
    }*/
}

void CShip::change_hidden() {
    hidden = !hidden;
}
void CShip::change_selected() {
    is_selected = !is_selected;
}
void CShip::put_ship_on_map(int** map_array){
    int damage_counter = 0;

    if (inverse == HORIZONTAL) {
        for (int i = 0; i < size; i++) {
            if (damage_level & section_array[i]) {
                map_array[head_coordinate_y][head_coordinate_x + i] = HIT_PALUBA_CELL;
                damage_counter++;
            } else {
                map_array[head_coordinate_y][head_coordinate_x + i] = PALUBA_CELL;
            }
        }

        if(damage_counter == size) {
            for (int i = 0; i < size; i++) {
                map_array[head_coordinate_y][head_coordinate_x + i] = KILLED_PALUBA_CELL;
            }
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (damage_level & section_array[i]) {
                map_array[head_coordinate_y + i][head_coordinate_x] = HIT_PALUBA_CELL;
                damage_counter++;
            } else {
                map_array[head_coordinate_y + i][head_coordinate_x] = PALUBA_CELL;
            }
        }

        if(damage_counter == size) {
            for (int i = 0; i < size; i++) {
                map_array[head_coordinate_y + i][head_coordinate_x] = KILLED_PALUBA_CELL;
            }
        }
    }

}

bool CShip::has_the_coordinate(int& x, int& y) {
    if (inverse == HORIZONTAL) {
        if (y == head_coordinate_y) {
            for(int i = head_coordinate_x ; i < head_coordinate_x + size ; i++) {
                if (i == x) {
                    return true;
                }
            }
        } else {
            return false;
        }
    } else {
        if (x == head_coordinate_x) {
            for(int i = head_coordinate_y ; i < head_coordinate_y + size ; i++) {
                if (i == y) {
                    return true;
                }
            }
        } else {
            return false;
        }
    }
    return false;
}

int CShip::get_palub_number(int x, int y) {
    int palub_number = 1;
    if (inverse == HORIZONTAL) {
        for(int i = head_coordinate_x ; i < head_coordinate_x + size ; i++) {
            if(i == x) {
                break;
            }
            palub_number++;
        }
    } else {
        for(int i = head_coordinate_y ; i < head_coordinate_y + size ; i++) {
            if(i == y) {
                break;
            }
            palub_number++;
        }
    }

    return palub_number;
}

void CShip::add_hit_palub(int x, int y) {
    if (has_the_coordinate(x, y)) {
        switch(get_palub_number(x, y)) {
            case 1:
                damage_level |= SEC_1;
                break;
            case 2:
                damage_level |= SEC_2;
                break;
            case 3:
                damage_level |= SEC_3;
                break;
            case 4:
                damage_level |= SEC_4;
                break;
        }
    }
}

int CShip::section_array[] = {SEC_1, SEC_2, SEC_3, SEC_4};