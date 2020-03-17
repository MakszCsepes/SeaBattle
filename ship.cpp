//
// Created by max on 21/1/2020.
//

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
void CShip::draw(SDL_Renderer* renderer) {
    if (hidden && (damage_level == 0)) {
        return;
    }

    for(int i = 0 ; i < size ; i++) {
        if (!hidden || (damage_level & section_array[i])) {
            draw_ship_part(renderer, i);

            if (damage_level & section_array[i]) {
                draw_hit(renderer, i);
            }
        }
    }
}

void CShip::change_hidden() {
    hidden = !hidden;
}
void CShip::change_selected() {
    is_selected = !is_selected;
}
void CShip::put_ship_on_map(int** map){
    int c = 1;
    if (inverse == HORIZONTAL) {
        for (int i = 0; i < size; i++) {
            if (damage_level & section_array[i]) {
                map[head_coordinate_y][head_coordinate_x + i] = HIT_PALUBA_CELL;
                c++;
            } else {
                map[head_coordinate_y][head_coordinate_x + i] = PALUBA_CELL;
            }
        }

        if(c == size) {
            for (int i = 0; i < size; i++) {
                map[head_coordinate_y][head_coordinate_x + i] = KILLED_PALUBA_CELL;
            }
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (damage_level & section_array[i]) {
                map[head_coordinate_y + i][head_coordinate_x] = HIT_PALUBA_CELL;
                c++;
            } else {
                map[head_coordinate_y + i][head_coordinate_x] = PALUBA_CELL;
            }
        }

        if(c == size) {
            for (int i = 0; i < size; i++) {
                map[head_coordinate_y + i][head_coordinate_x] = KILLED_PALUBA_CELL;
            }
        }
    }

}

bool CShip::coordinate_belong_to_ship(int x, int y) {
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
    if (coordinate_belong_to_ship(x, y)) {
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