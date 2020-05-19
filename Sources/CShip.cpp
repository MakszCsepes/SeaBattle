#include <Headers/CWorld.h>
#include "CShip.h"
extern CWorld* world_game;

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

    update_state( world_game->get_current_frame() - start_frame);
    point2DT point;

    point.x = current_x + isoEngine->scrollX;
    point.y = current_y + isoEngine->scrollY;

    Converter2DToIso(&point);

    switch (size) {
        case SUBMARINE_SIZE:
            texture_renderer_XY_clip(&texture_array[size - 1], point.x, point.y, inverse ? &submarine_rect[0] : &submarine_rect[1]);
            break;
        case DESTROYER_SIZE:
            if (!hidden || damage_level == 3)  {
                // todo why -30
                texture_renderer_XY_clip(&texture_array[size - 1], inverse ? point.x : point.x - 30, point.y, inverse ? &destroyer_rect[0] : &destroyer_rect[1]);
            }
            break;
        case CRUISER_SIZE:
            if (!hidden || damage_level == 7) {
                // todo why -65
                texture_renderer_XY_clip(&texture_array[size - 1], inverse ? point.x : point.x - 65, point.y, inverse ? &cruiser_rect[0] : &cruiser_rect[1]);
            }
            break;
        case BATTLESHIP_SIZE:
            if (!hidden || damage_level == 15) {
                // todo why -90
                texture_renderer_XY_clip(&texture_array[size - 1], inverse ? point.x : point.x - 90, point.y, inverse ? &battleship_rect[0] : &battleship_rect[1]);
            }
            break;
    }

    draw_effects(isoEngine);
}

const int t = 250;
double frame_count = 60*t/1000;

void CShip::update_state(int frame_number) {
    if (frame_number < frame_count) {
        if (current_x != end_x && current_y != end_y) {
            return;
        }
    }
    int delta_x = (end_x - old_x) / frame_count;
    int delta_y = (end_y - old_y) / frame_count;
    if(end_x != old_x) {
        current_x += /*old_x + */delta_x/**frame_number*/;

        if (delta_x < 0) {
            if (current_x < end_x) {
                current_x = end_x;
            }
        } else {
            if (current_x > end_x) {
                current_x = end_x;
            }
        }
    }
    if (end_y != old_y) {
        current_y += /*old_y + */delta_y/**frame_number*/;

        if (delta_y < 0) {
            if (current_y < end_y) {
                current_y = end_y;
            }
        } else {
            if (current_y > end_y) {
                current_y = end_y;
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
    if (this->get_inverse() == HORIZONTAL) {
        if (this->head_coordinate_x + this->get_size() > MAP_CELL_WIDTH) {
            this->head_coordinate_x = 0;
        }
        if (this->head_coordinate_x < 0) {
            this->head_coordinate_x = MAP_CELL_WIDTH - this->get_size();
        }
        if (this->head_coordinate_y > MAP_CELL_WIDTH - 1) {
            this->head_coordinate_y = 0;
        }
        if (this->head_coordinate_y < 0) {
            this->head_coordinate_y = MAP_CELL_WIDTH - 1;
        }
    } else if(this->get_inverse() == VERTICAL){
        if (this->head_coordinate_x > MAP_CELL_WIDTH - 1) {
            this->head_coordinate_x = 0;
        }
        if (this->head_coordinate_x < 0) {
            this->head_coordinate_x = MAP_CELL_WIDTH - 1;
        }
        if (this->head_coordinate_y + this->get_size() > MAP_CELL_WIDTH) {
            this->head_coordinate_y = 0;
        }
        if (this->head_coordinate_y < 0) {
            this->head_coordinate_y = MAP_CELL_WIDTH - this->get_size();
        }
    }

    old_x = current_x;
    old_y = current_y;

    end_x = (head_coordinate_x*TILESIZE) + MAP_OFFSET_X + offset_x;
    end_y = (head_coordinate_y*TILESIZE) + MAP_OFFSET_Y + offset_y;

    start_frame = world_game->get_current_frame();
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
void CShip::load_textures() {
    init_tile_clip(submarine_rect, &submarineTex, 60, 50, TEXTURE_NUMBER_OF_SHIP);
    init_tile_clip(destroyer_rect, &destroyerTex, 107, 74, TEXTURE_NUMBER_OF_SHIP);
    init_tile_clip(cruiser_rect, &cruiserTex, 126, 92, TEXTURE_NUMBER_OF_SHIP);
    init_tile_clip(battleship_rect, &battleshipTex, 145, 95, TEXTURE_NUMBER_OF_SHIP);

    if(load_texture(&submarineTex, "Resources/Images/submarine_inv.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/submarine.png");
        exit(0);
    }
    texture_array[0] = submarineTex;
    if(load_texture(&destroyerTex, "Resources/Images/destroyer_inv2.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/destroyer.png");
        exit(0);
    }
    texture_array[1] = destroyerTex;
    if(load_texture(&cruiserTex, "Resources/Images/cruiser_inv.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/cruiser.png");
        exit(0);
    }
    texture_array[2] = cruiserTex;
    if(load_texture(&battleshipTex, "Resources/Images/battleship_inv.png") == 0) {
        fprintf(stderr, "Error, could not load texture : data/battleship.png");
        exit(0);
    }
    texture_array[3] = battleshipTex;
}

int CShip::section_array[] = {SEC_1, SEC_2, SEC_3, SEC_4};
textureT CShip::texture_array[] = {submarineTex, destroyerTex, cruiserTex, battleshipTex};