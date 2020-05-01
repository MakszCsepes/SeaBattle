#ifndef SEABATTLE_WORLD_H
#define SEABATTLE_WORLD_H

#include "player.h"
#include <list>
#include <SDL2/SDL_ttf.h>


#define PUT_SHIPS 0
#define PLAY_GAME 1
#define ENDGAME 2

using namespace std;

struct coordinate{
    int i;
    int j;
};

class CWorld : public IDrawable {
    list<coordinate> l;
public:
    CPlayer user;
    CPlayer ai;

    lua_State* lua_state; // state for Lua script
    int game_state; // PUT_SHIPS, PLAY_GAME, ENDGAME
    bool turn;

    CWorld() {
//        todo default constr;
    }
    CWorld(const CPlayer& User, const CPlayer& AI, int game_state, lua_State* lua, const list<coordinate>& list) {
        this->user = User;
        this->ai = AI;

        this->game_state = game_state;

        lua_state = lua;

        l = list;

        turn = USER_TURN;
    }
    CWorld(const CWorld& world) {
        l = world.l;
        user = world.user;
        ai = world.ai;
        lua_state = world.lua_state;
        turn = world.turn;
        game_state = world.game_state;
    }
    CWorld(CWorld* world) {
        l = world->l;
        user = world->user;
        ai = world->ai;
        lua_state = world->lua_state;
        turn = world->turn;
        game_state = world->game_state;
    }

    void draw(isoEngineT*);
    void change_turn();
    void init_ai();


    CWorld& operator =(const CWorld* obj) {
        user = obj->user;
        ai = obj->ai;

        game_state = obj->game_state;

        lua_state = obj->lua_state;

        l = obj->l;

        turn = obj->turn;
    }
};

#endif //SEABATTLE_WORLD_H
