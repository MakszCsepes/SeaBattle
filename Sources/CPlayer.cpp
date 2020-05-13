#include "CPlayer.h"

void CPlayer::set_name(char *new_name) {
    strcpy(name, new_name);
}
int CPlayer::get_points() {
    return points;
}
CShip* CPlayer::get_new_extended_ship_array(CShip* added_ship) {
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
    ships = get_new_extended_ship_array(current_ship);

}

void CPlayer::change_inited() {
    inited = !inited;
}

bool CPlayer::get_init_status() {
    return inited;
}

bool CPlayer::get_aim_status() {
    return got_the_aim;
}

void CPlayer::draw(isoEngineT* isoEngine) {
    map.draw(isoEngine);

    for(int i = 0 ; i < ship_quantity ; i++) {
        if (ships[i].get_hidden() == false) {
            ships[i].draw(isoEngine);
        } else if ( (ships[i].get_hidden() == true) && (ships[i].get_X() != 0) ) {
            ships[i].draw(isoEngine);
        }
    }

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


bool CPlayer::was_ever_hit_on_the_position(CMap& enemy_map, int &pos_x, int &pos_y) {
    switch (enemy_map[pos_y][pos_x]) {
        case MISHIT_CELL:
            return true;
        case KILLED_PALUBA_CELL:
            return true;
        case HIT_PALUBA_CELL:
            return true;
        default:
            return false;
    }
}
bool CPlayer::check_collision (CShip& ship1 , CShip& ship2) {
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

void CPlayer::do_hit(CPlayer& enemy) {
    for(int i = 0; i < ship_quantity ; i++) {
        if(enemy.ships[i].has_the_coordinate(map.cursor.position_x, map.cursor.position_y)) {

            enemy.ships[i].add_hit_palub(map.cursor.position_x, map.cursor.position_y);

            if(enemy.ships[i].get_X() == 15) {
                enemy.map[map.cursor.position_y][map.cursor.position_x] = KILLED_PALUBA_CELL;
            }

            enemy.map[map.cursor.position_y][map.cursor.position_x] = HIT_PALUBA_CELL;
            CEffect new_effect(map.cursor.position_x, map.cursor.position_y, enemy.map.offset_x, enemy.map.offset_y, "Resources/Images/smokes2.png");
            enemy.ships[i].add_effect(new_effect);

            got_the_aim = true;
            increase_points();
            return;
        }
    }

    got_the_aim = false;
    enemy.map[this->map.cursor.position_y][this->map.cursor.position_x] = MISHIT_CELL;
}

int** CPlayer::generate_state_map(const CPlayer& enemy) {
    int** state_array = new int*[MAP_CELL_HEIGHT];

    for(int i = 0 ; i < MAP_CELL_HEIGHT ; i++) {
        state_array[i] = new int[MAP_CELL_WIDTH];
        for(int j = 0 ; j < MAP_CELL_WIDTH ; j++) {
            state_array[i][j] = enemy.map.cells_array[i][j];
        }
    }

    for(int i = 0 ; i < SHIP_QUANTITY ; i++) {
        enemy.ships[i].put_ship_on_map(state_array);
    }

    return state_array;
}

