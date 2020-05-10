#ifndef SEABATTLE_CWORLD_H
#define SEABATTLE_CWORLD_H

#include "CPlayer.h"
#include <cstring>
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

    // SDL gameT
    SDL_Event event;
    int loop_done;
    SDL_Rect mouseRect;
    point2DT mousePoint;
    point2DT mapScroll2Dpos;
    int mapScrollSpeed;
    isoEngineT isoEngine;
    int lastTiledClick;


    int frame_count;
    int old_frame_count;
    double FPS;

    int time_stamp;

    CWorld() {
//        todo default constr;
    }
    CWorld(const CPlayer& User, const CPlayer& AI, int game_state, lua_State* lua, const list<coordinate>& list) {
        this->user = User;
        this->ai = AI;

        this->frame_count = 0;
        this->old_frame_count = 0;
        this->time_stamp = 0;
        this->FPS = 0;
        this->game_state = game_state;

        this->lua_state = lua;

        this->l = list;

        this->turn = USER_TURN;
    }
    CWorld(const CWorld& world) {
        this->l = world.l;
        this->user = world.user;
        this->ai = world.ai;
        this->lua_state = world.lua_state;
        this->turn = world.turn;
        this->game_state = world.game_state;
        this->time_stamp = 0;
        this->FPS = world.FPS;
    }
    CWorld(CWorld* world) {
        this->l = world->l;
        this->user = world->user;
        this->ai = world->ai;
        this->lua_state = world->lua_state;
        this->turn = world->turn;
        this->game_state = world->game_state;
        this->time_stamp = 0;
    }

    void draw(isoEngineT*);
    void change_turn();
    void init_ai();


    Uint32 get_timestamp_now();

    CWorld& operator =(const CWorld* obj) {
        user = obj->user;
        ai = obj->ai;

        game_state = obj->game_state;

        lua_state = obj->lua_state;

        l = obj->l;

        turn = obj->turn;
    }
};

#endif //SEABATTLE_CWORLD_H
