//
// Created by max on 21/1/2020.
//

#include "player.h"

void CPlayer::set_name(char *new_name) {
    strcpy(name, new_name);
}

CShip* CPlayer::get_new_array(CShip* added_ship) {
    CShip* new_ship_array = new CShip[ship_quantity + 1];

    for(int i = 0 ; i < ship_quantity ; i++) {
        new_ship_array[i] = ships[i];
    }

    *(new_ship_array + ship_quantity) = *current_ship;
    ship_quantity++;

    return new_ship_array;
}
void CPlayer::add_ship_to_player_array() {
    current_ship->change_selected();
    ships = get_new_array(current_ship);

}

void CPlayer::change_inited() {
    inited = !inited;
}

bool CPlayer::get_player_init_status() {
    return inited;
}

void CPlayer::draw(SDL_Renderer *renderer) {
    map.draw(renderer);
    for(int i = 0 ; i < ship_quantity ; i++) {
        if (ships[i].get_hidden() == false) {
            ships[i].draw(renderer);
        } else if ( (ships[i].get_hidden() == true) && (ships[i].get_X() != 0) ) {
            ships[i].draw(renderer);
        }
    }

}

// get 4-cells ship
CShip* CPlayer::get_battleship() {
    return new CShip(BATTLESHIP_SIZE, map.offset_x, map.offset_y);
}
// get 3-cells ship
CShip* CPlayer::get_cruiser() {
    return new CShip(CRUISER_SIZE, map.offset_x, map.offset_y);
}
// get 2-cells ship
CShip* CPlayer::get_destroyer() {
    return new CShip(DESTROYER_SIZE, map.offset_x, map.offset_y);
}
// get 1-cell ship
CShip* CPlayer::get_submarine() {
    return new CShip(SUBMARINE_SIZE, map.offset_x, map.offset_y);
}

CShip* CPlayer::get_new_ship() {
    if(ship_quantity < BATTLESHIP_QUANTITY) {
        return get_battleship();
    } else if (ship_quantity < BATTLESHIP_QUANTITY + CRUISER_QUANTITY) {
        return get_cruiser();
    } else if (ship_quantity < BATTLESHIP_QUANTITY + CRUISER_QUANTITY + DESTROYER_QUANTITY) {
        return get_destroyer();
    } else if (ship_quantity < BATTLESHIP_QUANTITY + CRUISER_QUANTITY + DESTROYER_QUANTITY + SUBMARINE_QUANTITY){
        return get_submarine();
    }

    return nullptr;
}

bool CPlayer::check_collision (CShip& ship1 , CShip ship2) {
    const int left_top_x = ship2.head_coordinate_x - 1;
    const int left_top_y = ship2.head_coordinate_y - 1;

    const int right_bottom_x = ship2.get_inverse() ? ship2.head_coordinate_x + ship2.get_size() : ship2.head_coordinate_x + 1;
    const int right_bottom_y = ship2.get_inverse() ? ship2.head_coordinate_y + 1 : ship2.head_coordinate_y + ship2.get_size();

    if (ship1.get_inverse()) {
        for (int i = ship1.head_coordinate_x ; i < ship1.head_coordinate_x + ship1.get_size() ; i++) {
            const int x = i;
            const int y = ship1.head_coordinate_y;

            if ( (x >= left_top_x && x <= right_bottom_x) && (y >= left_top_y && y <= right_bottom_y) ) {
                return true;
            }
        }
    } else {
        for (int i = ship1.head_coordinate_y ; i < ship1.head_coordinate_y + ship1.get_size() ; i++) {
            const int x = ship1.head_coordinate_x;
            const int y = i;

            if ( (x >= left_top_x && x <= right_bottom_x) && (y >= left_top_y && y <= right_bottom_y) ) {
                return true;
            }
        }
    }

    return false;
}
bool CPlayer::can_put_ship() {

    if (current_ship->get_inverse()) {
        if (current_ship->head_coordinate_x + current_ship->get_size() > MAP_CELL_WIDTH) {
            return false;
        }
    } else {
        if (current_ship->head_coordinate_y + current_ship->get_size() > MAP_CELL_HEIGHT) {
            return false;
        }
    }

    for(int i = 0 ; i < ship_quantity ; i++) {
        if (check_collision(*current_ship, ships[i])) {
            return false;
        }
    }
    return true;
}
void CPlayer::increase_points() {
    points++;
}

void CPlayer::do_hit(int x, int y) {
    for(int i = 0; i < ship_quantity ; i++) {
        if(ships[i].coordinate_belong_to_ship(x, y)) {
            map[y][x] = HIT_PALUBA_CELL;
            return;
        }
    }
    map[y][x] = MISHIT_CELL;
}

int** CPlayer::generate_state_map() {
    int** state_array = new int*[MAP_CELL_HEIGHT];
    for(int i = 0 ; i < MAP_CELL_HEIGHT ; i++) {
        state_array[i] = new int[MAP_CELL_WIDTH];
        for(int j = 0 ; j < MAP_CELL_WIDTH ; j++) {
            state_array[i][j] = map[i][j];
        }
    }

    for(int i = 0 ; i < SHIP_QUANTITY ; i++) {
        ships->put_ship_on_map(state_array);
    }

    return state_array;
}

int CPlayer::get_points() {
    return points;
}

bool CPlayer::any_ship_damaged_on_position(int& x, int& y) {
    for (int i = 0 ; i < ship_quantity ; i++) {
        if (ships[i].coordinate_belong_to_ship(x, y)) {
            ships[i].add_hit_palub(x, y);
            return true;
        }
    }

    return false;
}