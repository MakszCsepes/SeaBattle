#include <ctime>
#include "ai.h"
#include <iostream>
#include <fstream>


using namespace std;

coordinate get_rand_from_list(list<coordinate> &l) {
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
void delete_by_value(list<coordinate> &l, coordinate val) {
    auto iter = l.begin();
    for(coordinate v : l) {
        if(v.i == val.i && v.j == val.j) {
            l.erase(iter);
            return;
        }
        iter++;
    }
}
void get_rand_coords(int &cursor_row, int &cursor_col, list<coordinate> &l) {

    coordinate rand_coord = get_rand_from_list(l);
    delete_by_value(l, rand_coord);
    cursor_row = rand_coord.i;
    cursor_col = rand_coord.j;
}

void init_computer (world *game) {
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

LuaRef get_table_from_stateMap(int **state_map, lua_State *L) {
    LuaRef table = newTable(L);

    for(int i = 0 ; i < MARGIN_HEIGHT; i++) {
        table[i] = newTable(L);
        for(int j = 0 ; j < MARGIN_WIDTH ; j++) {
            table[i][j] = state_map[i][j];
        }
    }

    return table;
}
int **get_stateMap_from_table(LuaRef &table) {
    int **map = new int*[MARGIN_HEIGHT];
    create_map(map);

    for(int i = 0 ; i < MARGIN_HEIGHT ; i++) {
        for(int j = 0 ; j < MARGIN_WIDTH ; j++) {
            map[i][j] = table[i][j];
        }
    }

    return map;
}

void do_ai_hit (world &game) {
    LuaRef players_map = newTable(game.lua_state);
    players_map = get_table_from_stateMap(game.user_map, game.lua_state);

    LuaRef state_map = newTable(game.lua_state);
    state_map = get_table_from_stateMap(game.state_array, game.lua_state);

    LuaRef get_coords = getGlobal(game.lua_state, "get_coords");
    LuaRef coords = nullptr;
    coords = get_coords(state_map);

    game.map_position_y = coords["i"];
    game.map_position_x = coords["j"];

    if (game.state_array[game.map_position_y][game.map_position_x] == NO_HIT_HERE) {
        if (if_any_ship_damaged_on_position(game.map_position_x, game.map_position_y, game.user.ships_list_head)) {

            game.user_map[game.map_position_y][game.map_position_x] = HIT;
            game.state_array[game.map_position_y][game.map_position_x] = WAS_HIT;
        } else {

            game.user_map[game.map_position_y][game.map_position_x] = MISHIT;
            game.state_array[game.map_position_y][game.map_position_x] = WAS_MISHIT;
        }
    } else {

    }
}
