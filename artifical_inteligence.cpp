#include <ctime>
#include "World.h"
#include <iostream>

using namespace std;

// INITIALIZING AI
coordinate get_rand_from_list(list<coordinate>& l) {
    auto iter = l.begin();
    int p = rand () % l.size();
    int i = 0;
    for(coordinate v : l) {
        if(i == p) {
            cout << v.i << " " << v.j << endl;
            return v;
        }
        i++;
        iter++;
    }
}
void delete_by_value(list<coordinate>& l, coordinate val) {
    auto iter = l.begin();
    for(coordinate v : l) {
        if(v.i == val.i && v.j == val.j) {
            l.erase(iter);
            return;
        }
        iter++;
    }
}
void get_rand_coords(int& cursor_row, int& cursor_col, list<coordinate>& l) {

    coordinate rand_coord = get_rand_from_list(l);
    delete_by_value(l, rand_coord);
    cursor_row = rand_coord.i;
    cursor_col = rand_coord.j;
}
void init_computer (world* game)    {
    int cursor_row;
    int cursor_column;
    int inverse;

    ship *new_ship;
    ship *current_ship;

    while (game->ai.inited == false) {

        new_ship = create_ship(game->ai.ships_list_head);

        if (new_ship != NULL) {
            current_ship = new_ship;

            srand(time(0));

            get_rand_coords(cursor_row, cursor_column, game->l);

            inverse = 0 + rand() % (1 - 0 + 1); // 1(true) - HORIZONTAL, 0(false) - VERTICAL

            inverse == HORIZONTAL ? current_ship->inverse = HORIZONTAL : current_ship->inverse = VERTICAL;

            if (can_move(current_ship, cursor_column, cursor_row)) {
                current_ship->coord_x = cursor_column;
                current_ship->coord_y = cursor_row;

                if (can_put_ship(game->ai.ships_list_head, *current_ship)) {
                    add_ship_to_list(game->ai.ships_list_head, current_ship);
                }

            }

        } else {
            game->ai.inited = true;
        }
    }
}

// AI ACTING
bool ship_hit_on_position(int& position_y, int& position_x, ship* ship) {
    int i;
    if(ship->inverse) {
        i = ship->coord_x;

        if(i != position_x) {
            return false;
        }
    } else {
        i = ship->coord_y;

        if(i != position_y) {
            return false;
        }
    }

    for(i ; i < ship->size ; i++) {
        if(ship->inverse) {
            if (i == position_x) {
                return true;
            }
        } else {
            if(i == position_y) {
                return true;
            }
        }
    }

    return false;
}
ship* get_ship_by_position(int& position_y, int& position_x, ship* ships_list_head) {
    ship* s = ships_list_head;

    while(s) {
        if(ship_hit_on_position(position_y, position_x, s)) {
            return s;
        }
        s = s->next;
    }


    return nullptr;
}
bool ship_killed(ship* ship, int** user_map, int& position_y, int& position_x) {
    if (ship->inverse) {
        for(int j = ship->coord_x ; j < ship->size ; j++) {
            if(j == position_x) {
                continue;
            }

            if(user_map[position_y][j] != HIT) {
                return false;
            }
        }
    } else {
        for(int i = ship->coord_x ; i < ship->size ; i++) {
            if(i == position_y) {
                continue;
            }

            if(user_map[i][position_x] != HIT) {
                return false;
            }
        }
    }
    return false;
}
LuaRef get_table_from_stateMap(int** state_map, lua_State* L) {
    LuaRef table = newTable(L);

    for(int i = 0 ; i < MARGIN_HEIGHT; i++) {
        table[i] = newTable(L);
        for(int j = 0 ; j < MARGIN_WIDTH ; j++) {
            table[i][j] = state_map[i][j];
        }
    }

    return table;
}
void do_ai_hit (world& game) {
    LuaRef players_map = newTable(game.lua_state);
    players_map = get_table_from_stateMap(game.user_map, game.lua_state);

    LuaRef state_map = newTable(game.lua_state);

    LuaRef get_coords = getGlobal(game.lua_state, "get_coords");
    LuaRef coords = nullptr;

    do {
        ship* hit_ship;
        state_map = get_table_from_stateMap(game.state_array, game.lua_state);
        coords = get_coords(state_map);
        game.map_position_y = coords["i"];
        game.map_position_x = coords["j"];

        // checking if on this position wasn`t ever hit
        if (game.state_array[game.map_position_y][game.map_position_x] == NO_HIT_HERE) {
            // checking if any ship is damaged on this position
//            if (any_ship_damaged_on_position(game.position_x, game.position_y, game.user.ships_list_head)) {
            if(hit_ship = get_ship_by_position(game.map_position_y, game.map_position_x, game.user.ships_list_head)){
                game.user_map[game.map_position_y][game.map_position_x] = HIT;
                if (ship_killed(hit_ship, game.user_map, game.map_position_y, game.map_position_x)) {
                    game.state_array[game.map_position_y][game.map_position_x] = WAS_KILL;
                } else {
                    game.state_array[game.map_position_y][game.map_position_x] = WAS_HIT;
                }
            } else {
                game.user_map[game.map_position_y][game.map_position_x] = MISHIT;
                game.state_array[game.map_position_y][game.map_position_x] = WAS_MISHIT;
            }
        }
    } while(game.state_array[game.map_position_y][game.map_position_x] == WAS_HIT or
            game.state_array[game.map_position_y][game.map_position_x] == WAS_KILL);
}