#ifndef SEABATTLE_PLAYER_H
#define SEABATTLE_PLAYER_H

#include "ship.h"
#include <cstring>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

#include <LuaBridge/LuaBridge.h>

using namespace luabridge;
using namespace std;

#define USER_TURN true
#define AI_TURN false

class CPlayer : public IDrawable {
    char name[15];

    int points;
    int ship_quantity;

    bool got_the_aim;
    bool inited;

    CShip* get_battleship();
    CShip* get_destroyer();
    CShip* get_cruiser();
    CShip* get_submarine();
public:
    CShip* ships;
    CShip* current_ship;
    CMap map;

    CPlayer() {
        strcpy(name, "undefined");

        points = 0;

        got_the_aim = false;
        inited = false;

        ship_quantity = 0;
        ships = new CShip[ship_quantity];
    }
    CPlayer(char name[], CMap& Map) {
        set_name(this->name);

        points = 0;
        ship_quantity = 0;

        got_the_aim = false;
        inited = false;
        this->map = Map;

        current_ship = get_new_ship();
        ships = new CShip[ship_quantity];
    }

    void add_ship_to_player_array();
    void set_name(char*);
    void increase_points();
    void change_inited();

    bool get_init_status();
    bool get_aim_status(); // todo RENAME (get true, if player hit last time, false, if did not);
    int get_points();
    CShip* get_new_ship();
    CShip* get_new_extended_ship_array(CShip* added_ship);

    bool check_collision (CShip&, CShip&);
    bool can_put_ship();
    bool was_ever_hit_on_the_position(CMap& enemy_map, int& pos_x, int& pos_y);
    void do_hit(CPlayer& enemy);
    CPlayer& operator =(const CPlayer& player_source) {
        strcpy(name, player_source.name);

        got_the_aim = player_source.got_the_aim;
        map = player_source.map;
        points = player_source.points;

        current_ship = player_source.current_ship;
        ship_quantity = player_source.ship_quantity;

        for(int i = 0 ; i < ship_quantity ; i++) {
            ships[i] = player_source.ships[i];
        }

        return *this;
    }

    void draw(SDL_Renderer*) override;
    int** generate_state_map(const CPlayer&);

    LuaRef convert_array_to_table(int** array_map, lua_State* luaState) {
        LuaRef table_for_lua = newTable(luaState);

        for(int i = 0 ; i < MAP_CELL_HEIGHT ; i++) {
            table_for_lua[i] = newTable(luaState);
            for(int j = 0 ; j < MAP_CELL_WIDTH ; j++) {
                table_for_lua[i][j] = array_map[i][j];
            }
        }

        return table_for_lua;
    }
    void assign_new_hit_coords_from(lua_State* luaState, const CPlayer& enemy) {
        LuaRef get_coords = getGlobal(luaState , "get_coords");

        LuaRef map_table = convert_array_to_table(generate_state_map(enemy), luaState);
        LuaRef new_coords = get_coords(map_table);

        map.cursor.position_x = new_coords['j'];
        map.cursor.position_y = new_coords['i'];
    }
};

#endif //SEABATTLE_PLAYER_H
