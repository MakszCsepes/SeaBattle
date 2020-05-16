#include "CShip.h"

void CShip::change_inverse() {
    inverse = !inverse;
}

int CShip::get_damage() {
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
bool CShip::is_dead() {
    switch (size) {
        case SUBMARINE_SIZE:
            if(damage_level & SEC_1) {
                return true;
            }
            break;
        case DESTROYER_SIZE:
            if(damage_level & SEC_1 && damage_level & SEC_2) {
                return true;
            }
            break;
        case CRUISER_SIZE:
            if(damage_level & SEC_1 && damage_level & SEC_2 && damage_level & SEC_3) {
                return true;
            }
            break;
        case BATTLESHIP_SIZE:
            if(damage_level & SEC_1 && damage_level & SEC_2 &&
               damage_level & SEC_3 && damage_level & SEC_4) {
                return true;
            }
            break;
    }

    return false;
}

void CShip::add_effect(const CEffect& new_effect) {
    effects.push_back(new_effect);
}
void CShip::draw_effects(isoEngineT* isoEngine) {
    for(list<CEffect> :: iterator current_effect = effects.begin() ; current_effect != effects.end() ; current_effect++) {
        current_effect->draw(isoEngine);
    }
}

void CShip::draw(isoEngineT* isoEngine) {
    if (hidden && (damage_level == 0)) {
        return;
    }

    point2DT point;
    point.x = (head_coordinate_x * TILESIZE) + isoEngine->scrollX + MAP_OFFSET_X + offset_x;
    point.y = (head_coordinate_y * TILESIZE) + isoEngine->scrollY + MAP_OFFSET_Y + offset_y;

    Converter2DToIso(&point);

    switch (size) {
        case SUBMARINE_SIZE:
            texture_renderer_XY_clip(&submarineTex, point.x, point.y, inverse ? &submarine_rect[0] : &submarine_rect[1]);
            break;
        case DESTROYER_SIZE:
            if (!hidden || damage_level == 3)  {
                // todo why -30
                texture_renderer_XY_clip(&destroyerTex, inverse ? point.x : point.x - 30, point.y, inverse ? &destroyer_rect[0] : &destroyer_rect[1]);
            }
            break;
        case CRUISER_SIZE:
            if (!hidden || damage_level == 7) {
                // todo why -65
                texture_renderer_XY_clip(&cruiserTex, inverse ? point.x : point.x - 65, point.y, inverse ? &cruiser_rect[0] : &cruiser_rect[1]);
            }
            break;
        case BATTLESHIP_SIZE:
            if (!hidden || damage_level == 15) {
                // todo why -90
                texture_renderer_XY_clip(&battleshipTex, inverse ? point.x : point.x - 90, point.y, inverse ? &battleship_rect[0] : &battleship_rect[1]);
            }
            break;
    }

    draw_effects(isoEngine);
}


void CShip::update_state(int frame_number) {

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
int CShip::get_current_x() {
    return this->head_coordinate_x;
}
int CShip::get_current_y() {
    return this->head_coordinate_y;
}

void CShip::move(int x = 0, int y = 0) {
    head_coordinate_x = x;
    head_coordinate_y = y;
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
void CShip::change_effects() {
    for(list<CEffect> :: iterator current_effect = effects.begin() ; current_effect != effects.end() ; current_effect++) {
        current_effect->init_effect("Resources/Images/smokes3.png");
    }
}

int CShip::section_array[] = {SEC_1, SEC_2, SEC_3, SEC_4};