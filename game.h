#include <list>
#include <iostream>

#define MARGIN_WIDTH 10
#define MARGIN_HEIGHT 10

#define WATER_AREA 0
#define MISHIT  1
#define HIT 2

#define PUT_SHIPS 0
#define PLAY_GAME 1
#define ENDGAME 2

#define SHIP_QUANTITY 10
#define SUBMARINE_QUANTITY 4
#define DESTROYER_QUANTITY 3
#define CRUISER_QUANTITY 2

#define USER_MAP_OFFSET 0
#define AI_MAP_OFFSET 425

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

#include <LuaBridge/LuaBridge.h>

using namespace luabridge;
using namespace std;

struct coordinate{
    int i;
    int j;
};

struct ship {
    int coord_x; // logical coordinate 0-9
    int coord_y; // logical coordinate 0-9
    int size;
    int damage;
    bool inverse;

    ship *next;
};

struct player {
    bool inited;
    int points;
    int offset;

    ship *ships_list_head;
};

struct world {
    player user;
    player ai;

    int **user_map;
    int **ai_map;

    int map_position_x; // 0-9
    int map_position_y; // 0-9

    int game_state; // PUT_SHIPS, PLAY_GAME, ENDGAME

    ship *current_ship;

    int **state_array;
    lua_State* lua_state; // state for Lua script
    list<coordinate> l;
};

void create_map (int **map);

void delete_map (int **map);

bool can_put_ship (ship *head, ship ship1);

bool can_move (const ship *moved_ship, const int &cursor_x, const int &cursor_y);

bool check_inverse(ship *head, ship *ship1);

bool check_neighbours_down(ship *head, ship ship1);

bool check_inversion (ship ship1);

void make_inverse (ship *head, ship *current_ship);

void add_ship_to_list(ship *&dest_head, ship *ship1);

ship *create_ship (const ship *head);

bool if_any_ship_damaged_on_position (int &position_x, int &position_y, ship *head);

void destroyed (ship *head_A, ship *head_B, int **map_A, int **map_B);

bool was_hit_on_position (int position_y, int position_x, int **map );
